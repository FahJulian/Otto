#include "scene_loader.h"

#include "otto/util/file.h"
#include "otto/math/math.h"
#include "otto/util/optional.h"
#include "otto/util/platform/file_utils.h"
#include "otto/serialization/serializer.h"

namespace otto
{
    namespace
    {
        struct _Function
        {
            String name;
            String returnType;
            DynamicArray<String> argumentTypes;

            bool operator==(const _Function& other)
            {
                if (name != other.name)
                    return false;

                if (returnType != other.returnType)
                    return false;

                if (argumentTypes.getSize() != other.argumentTypes.getSize())
                    return false;

                for (uint64 i = 0; i < argumentTypes.getSize(); i++)
                {
                    if (argumentTypes[i] != other.argumentTypes[i])
                        return false;
                }

                return true;
            }
        };

        struct SystemInitArgument
        {
            enum Type
            {
                SCENE,
                VIEW,
                MULTIVIEW
            };

            Type type;

            union
            {
                String viewType;

                struct
                {
                    String viewType1;
                    String viewType2;
                };
            };

            SystemInitArgument()
                : viewType1(), viewType2()
            {
            }

            SystemInitArgument(const SystemInitArgument& other)
                :type(other.type), viewType1(other.viewType1), viewType2(other.viewType2)
            {
            }

            ~SystemInitArgument()
            {
                viewType1.~String();
                viewType2.~String();
            }
        };

        struct _Event
        {
            FilePath relativeFilePath;
            String name;

            bool operator==(const _Event& other) { return relativeFilePath == other.relativeFilePath; }
        };

        struct _Component
        {
            FilePath relativeFilePath;
            String name;
            bool hasSerializationFunction;
            bool hasDeserializationFunction;
        };

        struct _Behaviour
        {
            FilePath relativeFilePath;
            String name;
            DynamicArray<_Function> memberFunctions;
            bool hasSerializationFunction;
            bool hasDeserializationFunction;
            bool hasInitFunction;
            bool hasUpdateFunction;
            DynamicArray<_Event> eventFunctions;
        };

        struct _System
        {
            FilePath relativeFilePath;
            String name;
            DynamicArray<_Function> memberFunctions;
            bool hasInitFunction;
            DynamicArray<SystemInitArgument> initArguments;
            bool hasUpdateFunction;
            DynamicArray<_Event> eventFunctions;
        };

        void _removeComments(String& code)
        {
            uint64 index;
            while ((index = code.findFirstOf("/*")) != code.getSize())
                code.replace(index, code.findFirstOf("*/", index) + 2, "");

            while ((index = code.findFirstOf("//")) != code.getSize())
                code.replace(index, code.findFirstOf('\n', index) + 1, "");
        }

        Optional<_Function> _extractFunction(const String& code, uint64 index)
        {
            uint64 nameEnd = code.findLastNotOfWhiteSpace(index);
            uint64 nameBegin = code.findLastOfWhiteSpace(nameEnd);

            if (nameBegin == code.getSize() || nameEnd == code.getSize())
                return NULLOPT;

            uint64 j = 0;
            uint64 returnTypeEnd = code.findLastNotOfWhiteSpace(nameBegin);
            uint64 returnTypeBegin = max({
                code.findLastOf('{', returnTypeEnd),
                (j = code.findLastOf(';', returnTypeEnd)) != code.getSize() ? j : 0,
                (j = code.findLastOf('}', returnTypeEnd)) != code.getSize() ? j : 0,
                (j = code.findLastOf(':', returnTypeEnd)) != code.getSize() ? j : 0,
                }) + 1;

            uint64 argumentsBegin = index + uint64(1);
            uint64 argumentsEnd = code.findFirstOf(')', argumentsBegin) - 1;

            String name = String::subString(code, nameBegin, nameEnd + 1).trim();
            String returnType = String::subString(code, returnTypeBegin, returnTypeEnd + 1)
                .replaceAll("\n", " ")
                .replaceAll(",", ", ")
                .replaceAll("*", " * ")
                .replaceAll("&", " & ")
                .trim();

            while ((j = returnType.findFirstOf("  ")) != returnType.getSize())
                returnType.replace(j, j + 2, " ");

            String arguments = String::subString(code, argumentsBegin, argumentsEnd + 1).trim();

            DynamicArray<String> argumentTypes;

            uint64 templateLevel = 0;
            uint64 lastArgumentEnd = argumentsBegin - 1;

            if (code[code.findFirstNotOfWhiteSpace(argumentsBegin)] != ')')
            {
                for (uint64 k = argumentsBegin; k <= argumentsEnd + 1; k++)
                {
                    if (code[k] == '<')
                        templateLevel++;
                    else if (code[k] == '>')
                        templateLevel--;

                    if (templateLevel == 0)
                    {
                        if (code[k] == ',' || code[k] == ')')
                        {
                            uint64 argumentNameEnd = code.findLastNotOfWhiteSpace(k);
                            String argumentType = String::subString(code, lastArgumentEnd + 1, code.findLastOfWhiteSpace(argumentNameEnd))
                                .replaceAll("\n", " ")
                                .replaceAll(",", ", ")
                                .replaceAll("*", " * ")
                                .replaceAll("&", " & ")
                                .trim();

                            while ((j = argumentType.findFirstOf("  ")) != argumentType.getSize())
                                argumentType.replace(j, j + 2, " ");

                            argumentTypes.add(argumentType);
                            lastArgumentEnd = k;
                        }
                    }
                }
            }

            return _Function{ name, returnType, argumentTypes };
        }

        Optional<DynamicArray<_Function>> _findMemberFunctions(const FilePath& filePath, const String& className)
        {
            String code = File(filePath).read();

            _removeComments(code);

            code.replaceAll('\n', ' ');
            code.replaceAll("  ", " ");

            uint64 begin = code.findFirstOf("class " + className);
            if (begin == code.getSize())
                return NULLOPT;

            begin = code.findFirstOf('{', begin) + 1;

            DynamicArray<_Function> functions;

            uint64 level = 1;
            for (uint64 i = begin; i < code.getSize(); i++)
            {
                if (code[i] == '{')
                    level++;
                else if (code[i] == '}')
                    level--;

                if (level == 0)
                    break;

                if (level == 1 && code[i] == '(')
                {
                    auto function = _extractFunction(code, i);

                    if (function.hasValue() && !function.getValue().returnType.startsWith("static "))
                        functions.add(function.getValue());
                }
            }

            return functions;
        }

        bool _hasDeserializationFunction(const FilePath& file, const String& className)
        {
            String code = File(file).read();

            _removeComments(code);

            uint64 level = 0;
            for (uint64 i = 0; i < code.getSize(); i++)
            {
                if (code[i] == '{')
                {
                    uint64 potentialNamespaceNameBegin = code.findLastOfWhiteSpace(code.findLastNotOfWhiteSpace(i)) + 1;
                    uint64 potentialNamespaceEnd = code.findLastNotOfWhiteSpace(potentialNamespaceNameBegin);
                    if (String::subString(code, code.findLastOfWhiteSpace(potentialNamespaceEnd) + 1, potentialNamespaceEnd + 1) != "namespace")
                        level++;
                }
                else if (code[i] == '}' && level != 0)
                    level--;
                else if (level == 0 && code[i] == '(')
                {
                    auto function = _extractFunction(code, i);

                    if (function.hasValue() && 
                        (function.getValue() == _Function{ "deserializeComponent", "template<> " + className, {"const Serialized &", "const Map<String, Entity> &"}} ||
                        function.getValue() == _Function{ "otto::deserializeComponent", "template<> " + className, { "const Serialized &", "const Map<String, Entity> &" } }))
                    {
                        return true;
                    }
                }
            }

            return false;
        }

    } // namespace

    static bool sInitialized = false;

#ifdef OTTO_DYNAMIC
    static DynamicArray<_Component> sComponents;
    static DynamicArray<_Behaviour> sBehaviours;
    static DynamicArray<_System> sSystems;
    static DynamicArray<_Event> sEvents;
    static DynamicArray<String> sRequiredViews;
    static DynamicArray<Pair<String, String>> sRequiredMultiViews;
#endif

    void SceneLoader::init(const DynamicArray<String>& components, const DynamicArray<String>& behaviours,
        const DynamicArray<String>& systems, const DynamicArray<String>& events)
    {
        if (sInitialized)
            return;

#ifdef OTTO_DYNAMIC
        for (auto& component : components)
        {
            uint64 lastSeperator = component.findLastOf('/');
            String componentName = lastSeperator != component.getSize() ? String::subString(component, lastSeperator + 1) : component;
            FilePath filePath = component.startsWith("otto/") ? "C:/dev/Otto/Otto/src/" + component + ".hpp" : Application::getRootDirectory() + component + ".hpp";

            sComponents.add({ component, componentName, false, _hasDeserializationFunction(filePath, componentName) });
        }

        for (auto& e : events)
        {
            uint64 lastSeperator = e.findLastOf('/');
            String eventName = lastSeperator != e.getSize() ? String::subString(e, lastSeperator + 1) : e;

            sEvents.add({ e, eventName });
        }

        for (auto& behaviour : behaviours)
        {
            uint64 lastSeperator = behaviour.findLastOf('/');
            String behaviourName = lastSeperator != behaviour.getSize() ? String::subString(behaviour, lastSeperator + 1) : behaviour;
            FilePath filePath = behaviour.startsWith("otto/") ? "C:/dev/Otto/Otto/src/" + behaviour + ".hpp" : Application::getRootDirectory() + behaviour + ".hpp";

            auto functions = _findMemberFunctions(filePath, behaviourName);
            bool hasInitFunction;
            bool hasUpdateFunction;
            DynamicArray<_Event> eventFunctions;

            if (!functions.hasValue())
            {
                Log::error("Error with behaviour file ", filePath);
                continue;
            }

            if (functions.getValue().contains(_Function("onInit", "void", {})))
                hasInitFunction = true;
            if (functions.getValue().contains(_Function("onUpdate", "void", { "float32" })))
                hasUpdateFunction = true;

            for (auto& function : functions.getValue())
            {
                if (function.returnType == "void" && function.name == "onEvent" && function.argumentTypes.getSize() == 1)
                {
                    for (auto& e : sEvents)
                    {
                        if (function.argumentTypes[0].contains(" " + e.name + " ") || function.argumentTypes[0].contains(" " + e.name + "& "))
                            eventFunctions.add(e);
                    }
                }
            }

            if (hasInitFunction || hasUpdateFunction || eventFunctions.getSize() != 0)
            {
                if (!sRequiredViews.contains(behaviourName))
                    sRequiredViews.add(behaviourName);
            }

            sBehaviours.add({ behaviour, behaviourName, functions.getValue(), false, _hasDeserializationFunction(filePath, behaviourName), hasInitFunction, hasUpdateFunction, eventFunctions });
        }

        for (auto& system : systems)
        {
            uint64 lastSeperator = system.findLastOf('/');
            String systemName = lastSeperator != system.getSize() ? String::subString(system, lastSeperator + 1) : system;
            FilePath filePath = system.startsWith("otto/") ? "C:/dev/Otto/Otto/src/" + system + ".hpp" : Application::getRootDirectory() + system + ".hpp";

            auto functions = _findMemberFunctions(filePath, systemName);

            if (!functions.hasValue())
            {
                Log::error("Error with system file ", filePath);
                continue;
            }

            bool hasInitFunction = false;
            bool hasUpdateFunction = false;
            DynamicArray<SystemInitArgument> initArguments;
            DynamicArray<_Event> eventFunctions;

            for (auto function : functions.getValue())
            {
                if (function.returnType == "void" && function.name == "onUpdate" && function.argumentTypes.equals({ "float32" }))
                {
                    if (hasUpdateFunction)
                    {
                        Log::warn("System ", system, " has multiple update functions.");
                        continue;
                    }

                    hasUpdateFunction = true;
                }
                else if (function.returnType == "void" && function.name == "onEvent" && function.argumentTypes.getSize() == 1)
                {
                    for (auto& e : sEvents)
                    {
                        if (function.argumentTypes[0].contains(" " + e.name + " ") || function.argumentTypes[0].contains(" " + e.name + "& "))
                            eventFunctions.add(e);
                    }
                }
                else if (function.returnType == "void" && function.name == "onInit")
                {
                    if (hasInitFunction == true)
                    {
                        Log::warn("System ", system, " has multiple init functions.");
                        continue;
                    }

                    hasInitFunction = true;

                    for (auto& argumentType : function.argumentTypes)
                    {
                        SystemInitArgument argument = SystemInitArgument();

                        if (argumentType == "Shared<Scene>")
                        {
                            argument.type = SystemInitArgument::SCENE;
                        }
                        else if (argumentType.startsWith("MultiView"))
                        {
                            argument.type = SystemInitArgument::MULTIVIEW;

                            uint64 templateStart = argumentType.findFirstOf('<');
                            uint64 templateEnd = argumentType.findLastOf('>');

                            if (templateStart == argumentType.getSize() || templateEnd == argumentType.getSize() ||
                                argumentType.findFirstOf('<', templateStart + 1) != argumentType.getSize() || argumentType.findLastOf('>', templateEnd) != argumentType.getSize())
                            {
                                Log::warn("  SystemInitFunction for System " + system);
                                hasInitFunction = false;
                                break;
                            }

                            uint64 commaIndex = argumentType.findFirstOf(',');
                            if (commaIndex == argumentType.getSize() || argumentType.findFirstOf(',', commaIndex + 1) != argumentType.getSize())
                            {
                                Log::warn("Could not parse SystemInitFunction for System " + system);
                                hasInitFunction = false;
                                break;
                            }

                            argument.viewType1 = String::subString(argumentType, templateStart + 1, commaIndex).trim();
                            argument.viewType2 = String::subString(argumentType, commaIndex + 1, templateEnd).trim();

                            if (!sRequiredMultiViews.contains(Pair<String, String>{ argument.viewType1, argument.viewType2 }))
                            {
                                if (sRequiredMultiViews.contains(Pair<String, String>{ argument.viewType2, argument.viewType1 }))
                                {
                                    Log::warn("System ", system, " required MutliView <", argument.viewType1, ", ", argument.viewType2, ">, but MultiView <",
                                        argument.viewType2, ", ", argument.viewType1, "> is already used by another System. Consider switching the order "
                                        "of the components to improve performance.");
                                }

                                sRequiredMultiViews.add({ argument.viewType1, argument.viewType2 });
                            }
                        }
                        else if (argumentType.startsWith("View"))
                        {
                            argument.type = SystemInitArgument::VIEW;

                            uint64 templateStart = argumentType.findFirstOf('<');
                            uint64 templateEnd = argumentType.findLastOf('>');

                            if (templateStart == argumentType.getSize() || templateEnd == argumentType.getSize() ||
                                argumentType.findFirstOf('<', templateStart + 1) != argumentType.getSize() || 
                                argumentType.findLastOf('>', templateEnd) != argumentType.getSize() ||
                                argumentType.findFirstOf(',') != argumentType.getSize())
                            {
                                Log::warn("Could not parse SystemInitFunction for System " + system);
                                hasInitFunction = false;
                                break;
                            }

                            argument.viewType = String::subString(argumentType, templateStart + 1, templateEnd).trim();

                            if (!sRequiredViews.contains(argument.viewType))
                                sRequiredViews.add(argument.viewType);
                        }
                        else
                        {
                            Log::warn("Could not parse SystemInitFunction for System " + system);
                            hasInitFunction = false;
                            break;
                        }

                        initArguments.add(argument);
                    }
                }
            }

            sSystems.add({ system, systemName, functions.getValue(), hasInitFunction, initArguments, hasUpdateFunction, eventFunctions });
        }
#endif

        sInitialized = true;
    }

    Result<Shared<Scene>, SceneLoader::SceneLoadingError> SceneLoader::loadScene(const FilePath& filePath)
    {
        Shared<Scene> scene = _SceneInitializer().createScene();

        auto file = Serializer::deserialize(filePath);

        if (file.hasError())
            return SYNTAX_ERROR;

        Map<String, Entity> entities;
        for (auto& [entity, _] : file.getResult().getDictionary())
        {
            if (entities.containsKey(entity))
            {
                Log::error("Cannot load scene ", filePath, ": Entity ", entity, " duplicate.");
                return SceneLoadingError::DUPLICATE_ENTITY;
            }

            entities.insert(entity, scene->addEntity());
        }

        for (auto& [entityName, components] : file.getResult().getDictionary())
        {
            for (auto& [componentName, args] : components.getDictionary())
                scene->addComponent(entities.get(entityName), componentName, args, entities);
        }

        return scene;
    }

    bool SceneLoader::reloadDll()
    {
        _createSceneFile(Application::getRootDirectory() + ".tmp/generated_scene.cpp");

        bool success = DllReloader::reloadDll(Application::getRootDirectory() + ".tmp/client.dll", true, {
            Application::getRootDirectory() + ".tmp/generated_scene.cpp" }, { "OTTO_DEBUG", "OTTO_DYNAMIC"});

        if (!success)
            return false;

        _SceneInitializer initializer;
        initializer.initClientLog(Log::getInstance());

        return true;
    }

#ifdef OTTO_DYNAMIC
    void SceneLoader::_createSceneFile(const FilePath& filePath)
    {
        String code;

        code.append("#define OTTO_GENERATED_CODE\n");
        code.append("#include \"C:/dev/otto/otto/src/otto/base.h\"\n");
        code.append("#include \"otto/scene/scene.h\"\n");
        code.append("#include \"otto/event/event_dispatcher.h\"\n");
        code.append('\n');

        for (auto& component : sComponents)
            code.append("#include \"" + component.relativeFilePath.toString() + ".hpp\"\n");
        for (auto& e : sEvents)
            code.append("#include \"" + e.relativeFilePath.toString() + ".hpp\"\n");

        code.append("using namespace otto;\n\n");

        code.append('\n');

        for (auto& component : sComponents)
        {
            code.append("template<>\n"
                "OTTO_RCR_API void Scene::addComponent<" + component.name + ">(Entity entity, const " + component.name + "& component);\n");
            code.append("template<>\n"
                "OTTO_RCR_API void Scene::removeComponent<" + component.name + ">(Entity entity);\n");
            code.append("template<>\n"
                "OTTO_RCR_API " + component.name + "& Scene::getComponent<" + component.name + ">(Entity entity);\n");
            code.append("template<>\n"
                "OTTO_RCR_API bool Scene::hasComponent<" + component.name + ">(Entity entity);\n");
        }

        code.append('\n');

        for (auto& e : sEvents)
        {
            code.append("template<>\n"
                "OTTO_RCR_API void Scene::addEventListener<" + e.name + ">(const EventListener<" + e.name + ">& eventListener);\n");
            code.append("template<>\n"
                "OTTO_RCR_API void Scene::removeEventListener<" + e.name + ">(const EventListener<" + e.name + ">& eventListener);\n");
            code.append("template<>\n"
                "OTTO_RCR_API void Scene::dispatchEvent<" + e.name + ">(const " + e.name + "& e);\n");
        }

        code.append('\n');

        for (auto& behaviour : sBehaviours)
            code.append("#include \"" + behaviour.relativeFilePath.toString() + ".hpp\"\n");
        for (auto& system : sSystems)
            code.append("#include \"" + system.relativeFilePath.toString() + ".hpp\"\n");

        code.append('\n');

        code.append("namespace otto\n");
        code.append("{\n");

        code.append("    struct SceneData\n");
        code.append("    {\n");

        code.append("        Entity nextEntity = 0;\n");
        code.append('\n');

        for (auto& component : sComponents)
            code.append("        ComponentPool<" + component.name + "> " + String::untitle(component.name) + "Pool;\n");

        code.append('\n');

        for (auto& behaviour: sBehaviours)
            code.append("        ComponentPool<" + behaviour.name + "> " + String::untitle(behaviour.name) + "Pool;\n");

        code.append('\n');

        for (auto& e : sEvents)
            code.append("        EventDispatcher<" + e.name + "> " + String::untitle(e.name) + "Dispatcher;\n");

        code.append('\n');

        for (auto& system : sSystems)
            code.append("        " + system.name + ' ' + String::untitle(system.name) + ";\n");

        code.append('\n');

        for (auto& viewType : sRequiredViews)
            code.append("        View<" + viewType + "> " + String::untitle(viewType) + "View = View<" + viewType + ">(&" + String::untitle(viewType) + "Pool);\n");

        code.append('\n');

        for (auto& [type1, type2]: sRequiredMultiViews)
            code.append("        MultiView<" + type1 + ", " + type2 + "> " + String::untitle(type1) + "_" + String::untitle(type2) + "View = "
                "MultiView<" + type1 + ", " + type2 + ">(&" + String::untitle(type1) + "Pool, &" + String::untitle(type2) + "Pool);\n");

        code.append("    };\n");

        code.append('\n');

        code.append("    template<typename C>\n"
            "    C deserializeComponentOrBehaviour(const Serialized& args, const Map<String, Entity>& entities)\n"
            "    {\n"
            "        return C();\n"
            "    }\n"
        );

        code.append('\n');

        for (auto& component : sComponents)
        {
            if (component.hasDeserializationFunction)
            {
                code.append("    template<>\n");
                code.append("    " + component.name + " deserializeComponentOrBehaviour<" + component.name + ">(const Serialized& args, const Map<String, Entity>& entities)\n");
                code.append("    {\n");
                code.append("        return deserializeComponent<" + component.name + ">(args, entities);\n");
                code.append("    }\n");

                code.append('\n');
            }
        }

        for (auto& behaviour : sBehaviours)
        {
            if (behaviour.hasDeserializationFunction)
            {
                code.append("    template<>\n");
                code.append("    " + behaviour.name + "deserializeComponentOrBehaviour<" + behaviour.name + ">(const Serialized& args, const Map<String, Entity>& entities)\n");
                code.append("    {\n");
                code.append("        return deserializeComponent<" + behaviour.name + ">(args, entities);\n");
                code.append("    }\n");

                code.append('\n');
            }
        }

        code.append("    OTTO_RCR_API Shared<Scene> _SceneInitializer::createScene()\n"
            "    {\n"
            "        return new Scene(new SceneData());\n"
            "    }\n"
        );

        code.append('\n');

        code.append("    OTTO_RCR_API void _SceneInitializer::initClientLog(Log* mainLog)\n"
            "    {\n"
            "        Log::init(mainLog);\n"
            "    }\n"
        );

        code.append('\n');

        code.append("    OTTO_RCR_API void Scene::init()\n");
        code.append("    {\n");

        for (auto& system : sSystems)
        {
            if (system.hasInitFunction)
            {
                code.append("        mData->" + String::untitle(system.name) + ".onInit(");

                bool first = true;
                for (auto& argument : system.initArguments)
                {
                    if (!first)
                        code.append(", ");

                    if (argument.type == SystemInitArgument::Type::SCENE)
                        code.append("this");
                    else if (argument.type == SystemInitArgument::Type::VIEW)
                        code.append("&mData->" + String::untitle(argument.viewType) + "View");
                    else
                        code.append("&mData->" + String::untitle(argument.viewType1) + "_" + String::untitle(argument.viewType2) + "View");

                    first = false;
                }

                code.append(");\n");
            }
        }

        for (auto& behaviour : sBehaviours)
        {
            if (behaviour.hasInitFunction)
            {
                code.append("        for (auto [entity, behaviour] : mData->" + String::untitle(behaviour.name) + "View)\n");
                code.append("            behaviour.onInit();\n");
            }
        }

        code.append("    }\n");

        code.append('\n');

        code.append("    OTTO_RCR_API void Scene::update(float32 delta)\n");
        code.append("    {\n");

        for (auto& system : sSystems)
        {
            if (system.hasUpdateFunction)
                code.append("        mData->" + String::untitle(system.name) + ".onUpdate(delta);\n");
        }

        for (auto& behaviour : sBehaviours)
        {
            if (behaviour.hasUpdateFunction)
            {
                code.append("        for (auto [entity, behaviour] : mData->" + String::untitle(behaviour.name) + "View)\n");
                code.append("            behaviour.onUpdate(delta);\n");
            }
        }

        code.append("    }\n");

        code.append('\n');

        code.append("    OTTO_RCR_API Entity Scene::addEntity()\n"
            "    {\n"
            "        return mData->nextEntity++;\n"
            "    }\n"
        );

        code.append('\n');

        code.append("    OTTO_RCR_API void Scene::addComponent(Entity entity, const String& componentName, const Serialized& args, const EntityMap& entities)\n");
        code.append("    {\n");

        for (auto& component : sComponents)
        {
            code.append("        if (componentName == \"" + component.name + "\")\n");
            code.append("            addComponent(entity, deserializeComponentOrBehaviour<" + component.name + ">(args, entities));\n");
        }

        for (auto& behaviour : sBehaviours)
        {
            code.append("        if (componentName == \"" + behaviour.name + "\")\n");
            code.append("            mData->" + String::untitle(behaviour.name) + "Pool.addComponent(entity, deserializeComponentOrBehaviour<" + behaviour.name + ">(args, entities));\n");
        }

        code.append("    }\n");

        code.append('\n');

        code.append("    template<typename E>\n"
            "    OTTO_RCR_API void Scene::addEventListener(const EventListener<E>& eventListener)\n"
            "    {\n"
            "        OTTO_ASSERT(false, \"Event is not added.\")\n"
            "    }\n"
        );

        code.append('\n');

        for (auto& e : sEvents)
        {
            code.append("    template<>\n");
            code.append("    OTTO_RCR_API void Scene::addEventListener<" + e.name + ">(const EventListener<" + e.name + ">& eventListener)\n");
            code.append("    {\n");
            code.append("        mData->" + String::untitle(e.name) + "Dispatcher.addListener(eventListener);\n");
            code.append("    }\n");

            code.append('\n');
        }

        code.append("    template<typename E>\n"
            "    OTTO_RCR_API void Scene::removeEventListener(const EventListener<E>& eventListener)\n"
            "    {\n"
            "        OTTO_ASSERT(false, \"Event is not added.\")\n"
            "    }\n"
        );

        code.append('\n');

        for (auto& e : sEvents)
        {
            code.append("    template<>\n");
            code.append("    OTTO_RCR_API void Scene::removeEventListener<" + e.name + ">(const EventListener<" + e.name + ">& eventListener)\n");
            code.append("    {\n");
            code.append("        mData->" + String::untitle(e.name) + "Dispatcher.removeListener(eventListener);\n");
            code.append("    }\n");

            code.append('\n');
        }

        code.append("    template<typename E>\n"
            "    OTTO_RCR_API void Scene::dispatchEvent(const E& e)\n"
            "    {\n"
            "        OTTO_ASSERT(false, \"Event is not added.\")\n"
            "    }\n"
        );

        code.append('\n');

        for (auto& e : sEvents)
        {
            code.append("    template<>\n");
            code.append("    OTTO_RCR_API void Scene::dispatchEvent<" + e.name + ">(const " + e.name + "& e)\n");
            code.append("    {\n");

            for (auto& system : sSystems)
            {
                if (system.eventFunctions.contains(e))
                    code.append("        mData->" + String::untitle(system.name) + ".onEvent(e);\n");
            }

            for (auto& behaviour : sBehaviours)
            {
                if (behaviour.eventFunctions.contains(e))
                {
                    code.append("        for (auto [entity, behaviour] : mData->" + String::untitle(behaviour.name) + "View)\n");
                    code.append("            behaviour.onEvent(e);\n");
                }
            }
            

            code.append("        mData->" + String::untitle(e.name) + "Dispatcher.dispatchEvent(e);\n");
            code.append("    }\n");

            code.append('\n');
        }

        code.append("    template<typename C>\n"
            "    OTTO_RCR_API void Scene::addComponent(Entity entity, const C& component)\n"
            "    {\n"
            "        OTTO_ASSERT(false, \"Component is not added.\")\n"
            "    }\n"
        );

        code.append('\n');

        for (auto& component : sComponents)
        {
            code.append("    template<>\n");
            code.append("    OTTO_RCR_API void Scene::addComponent<" + component.name + ">(Entity entity, const " + component.name + "& component)\n");
            code.append("    {\n");
            code.append("        mData->" + String::untitle(component.name) + "Pool.addComponent(entity, component);\n");

            for (auto& [type1, type2] : sRequiredMultiViews)
            {
                if (type1 == component.name)
                    code.append("        mData->" + String::untitle(type1) + "_" + String::untitle(type2) + "View.onComponent1Added(entity);\n");
                else if (type2 == component.name)
                    code.append("        mData->" + String::untitle(type1) + "_" + String::untitle(type2) + "View.onComponent2Added(entity);\n");
            }
            code.append("    }\n");

            code.append('\n');
        }

        code.append("    template<typename C>\n"
            "    OTTO_RCR_API void Scene::removeComponent(Entity entity)\n"
            "    {\n"
            "        OTTO_ASSERT(false, \"Component is not added.\")\n"
            "    }\n"
        );

        code.append('\n');

        for (auto& component : sComponents)
        {
            code.append("    template<>\n");
            code.append("    OTTO_RCR_API void Scene::removeComponent<" + component.name + ">(Entity entity)\n");
            code.append("    {\n");
            code.append("        mData->" + String::untitle(component.name) + "Pool.removeComponent(entity);\n");
            code.append("    }\n");

            code.append('\n');
        }

        code.append("    template<typename C>\n"
            "    OTTO_RCR_API C& Scene::getComponent(Entity entity)\n"
            "    {\n"
            "        OTTO_ASSERT(false, \"Component is not added.\")\n"
            "    }\n"
        );

        code.append('\n');

        for (auto& component : sComponents)
        {
            code.append("    template<>\n");
            code.append("    OTTO_RCR_API " + component.name + "& Scene::getComponent<" + component.name + ">(Entity entity)\n");
            code.append("    {\n");
            code.append("        return mData->" + String::untitle(component.name) + "Pool.getComponent(entity);\n");
            code.append("    }\n");

            code.append('\n');
        }

        code.append("    template<typename C>\n"
            "    OTTO_RCR_API bool Scene::hasComponent(Entity entity)\n"
            "    {\n"
            "        OTTO_ASSERT(false, \"Component is not added.\")\n"
            "    }\n"
        );

        code.append('\n');

        for (auto& component : sComponents)
        {
            code.append("    template<>\n");
            code.append("    OTTO_RCR_API bool Scene::hasComponent<" + component.name + ">(Entity entity)\n");
            code.append("    {\n");
            code.append("        return mData->" + String::untitle(component.name) + "Pool.hasComponent(entity);\n");
            code.append("    }\n");

            code.append('\n');
        }

        //code.append('\n');

        code.append("} // namespace otto\n");

        FileUtils::createDirectoryRecursively(filePath.getParentDirectory());
        File file = File(filePath);
        file.write(code);
    }
#endif

} // namespace otto
