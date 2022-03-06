#include "scene_loader.h"

#include <iostream>

#include "otto/util/file.h"
#include "otto/math/math.h"
#include "otto/util/optional.h"
#include "otto/core/application.h"
#include "otto/util/platform/file_utils.h"
#include "otto/serialization/serializer.h"
#include "otto/window/window.h"

namespace otto
{
    namespace
    {
#ifdef OTTO_DYNAMIC

        struct _SystemInitArg
        {
            bool8 isMultiView;
            String view1Type;
            String view2Type;
        };

        struct _Component
        {
            String name;
            FilePath relativeFilePath;

            bool8 hasSerializationFunction;
            bool8 hasDeserializationFunction;

            bool8 operator==(const _Component& other) { return relativeFilePath == other.relativeFilePath; }
        };

        struct _Event
        {
            String name;
            FilePath relativeFilePath;

            bool8 operator==(const _Event& other) { return relativeFilePath == other.relativeFilePath; }
        };

        struct _Behaviour
        {
            String name;
            FilePath relativeFilePath;

            bool8 hasSerializationFunction;
            bool8 hasDeserializationFunction;

            DynamicArray<_Event> eventFunctions;
        };

        struct _System
        {
            String name;
            FilePath relativeFilePath;
            DynamicArray<_SystemInitArg> constructorArgs;
            DynamicArray<_Event> eventFunctions;
            DynamicArray<_Component> componentAddedListeners;
            DynamicArray<_Component> componentRemovedListeners;
        };

        struct _Package
        {
            DynamicArray<_Component> components;
            DynamicArray<_Event> events;
            DynamicArray<_Behaviour> behaviours;
            DynamicArray<_System> systems;
            DynamicArray<String> views;
            DynamicArray<Pair<String, String>> multiViews;
        };

        struct _Function
        {
            String name;
            String returnType;
            DynamicArray<String> argumentTypes;

            bool8 operator==(const _Function& other)
            {
                return name == other.name && returnType == other.returnType && argumentTypes == other.argumentTypes;
            }
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

        void _hasSerializationFunctions(const FilePath& file, const String& className, bool8* hasSerializationFunction, bool8* hasDeserializationFunction)
        {
            *hasSerializationFunction = false;
            *hasDeserializationFunction = false;

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

                    if (function.hasValue())
                    {
                        if (function.getValue() == _Function{ "deserializeComponent", "template<> " + className, {"const Serialized &", "const Map<String, Entity> &"} } ||
                                function.getValue() == _Function{ "otto::deserializeComponent", "template<> " + className, { "const Serialized &", "const Map<String, Entity> &" } })
                        {
                            *hasDeserializationFunction = true;
                        }
                        else if (function.getValue() == _Function{ "serializeComponent", "template<> Serialized", {"const " + className + " &", "const Map<String, Entity> &"} } ||
                            function.getValue() == _Function{ "otto::serializeComponent", "template<> Serialized", { "const " + className + " &", "const Map<String, Entity> &" } })
                        {
                            *hasSerializationFunction = true;
                        }
                    }   
                }
            }
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


                    if (function.hasValue() && function.getValue().name == className)
                        function.getValue().returnType = "";

                    if (function.hasValue() && !function.getValue().returnType.startsWith("static "))
                        functions.add(function.getValue());
                }
            }

            return functions;
        }

        DynamicArray<_Event> _findEventFunctions(const DynamicArray<_Function> functions, DynamicArray<_Event> loadedEvents)
        {
            DynamicArray<_Event> eventFunctions;

            for (auto& function : functions)
            {
                if (function.returnType == "void" && function.name == "onEvent" && function.argumentTypes.getSize() == 1)
                {
                    for (auto& e : loadedEvents)
                    {
                        if (function.argumentTypes[0].contains(" " + e.name + " ") || function.argumentTypes[0].contains(" " + e.name + "& "))
                            eventFunctions.add(e);
                    }
                }
            }

            return eventFunctions;
        }

        DynamicArray<_SystemInitArg> _findConstructorArguments(const String& className, const DynamicArray<_Function> functions)
        {
            DynamicArray<_SystemInitArg> constructorArgs;

            for (auto function : functions)
            {
                if (function.returnType == "" && function.name == className)
                {
                    for (auto& argumentType : function.argumentTypes)
                    {
                        if (argumentType.startsWith("MultiView"))
                        {
                            _SystemInitArg arg;
                            arg.isMultiView = true;

                            uint64 templateStart = argumentType.findFirstOf('<');
                            uint64 templateEnd = argumentType.findLastOf('>');
                            uint64 commaIndex = argumentType.findFirstOf(',');

                            if (commaIndex == argumentType.getSize() ||
                                templateEnd == argumentType.getSize() ||
                                templateStart == argumentType.getSize() || 
                                argumentType.findLastOf('>', templateEnd) != argumentType.getSize() ||
                                argumentType.findFirstOf(',', commaIndex + 1) != argumentType.getSize()||
                                argumentType.findFirstOf('<', templateStart + 1) != argumentType.getSize())
                            {
                                constructorArgs.clear();
                                break;
                            }

                            arg.view1Type = String::subString(argumentType, templateStart + 1, commaIndex).trim();
                            arg.view2Type = String::subString(argumentType, commaIndex + 1, templateEnd).trim();

                            constructorArgs.add(arg);
                        }
                        else if (argumentType.startsWith("View"))
                        {
                            _SystemInitArg arg;
                            arg.isMultiView = false;

                            uint64 templateStart = argumentType.findFirstOf('<');
                            uint64 templateEnd = argumentType.findLastOf('>');

                            if (templateStart == argumentType.getSize() || templateEnd == argumentType.getSize() ||
                                argumentType.findFirstOf('<', templateStart + 1) != argumentType.getSize() ||
                                argumentType.findLastOf('>', templateEnd) != argumentType.getSize() ||
                                argumentType.findFirstOf(',') != argumentType.getSize())
                            {
                                constructorArgs.clear();
                                break;
                            }

                            arg.view1Type = String::subString(argumentType, templateStart + 1, templateEnd).trim();

                            constructorArgs.add(arg);
                        }
                        else
                        {
                            constructorArgs.clear();
                            break;
                        }
                    }
                }
            }

            return constructorArgs;
        }

        DynamicArray<_Component> _findComponentAddedListeners(const DynamicArray<_Function> functions, 
            const DynamicArray<_Component> loadedComponents)
        {
            DynamicArray<_Component> componentAddedListeners;

            for (auto& function : functions)
            {
                if (function.returnType == "void" && function.name == "onEvent" && 
                    function.argumentTypes.getSize() == 1 && function.argumentTypes[0].contains("ComponentAddedEvent"))
                {
                    for (auto& component : loadedComponents)
                    {
                        if (function.argumentTypes[0].contains(" ComponentAddedEvent<" + component.name + "> ") ||
                            function.argumentTypes[0].contains(" ComponentAddedEvent<" + component.name + ">& "))
                        {
                            componentAddedListeners.add(component);
                            break;
                        }
                    }
                }
            }

            return componentAddedListeners;
        }

        DynamicArray<_Component> _findComponentRemovedListeners(const DynamicArray<_Function> functions,
            const DynamicArray<_Component> loadedComponents)
        {
            DynamicArray<_Component> componentRemovedListeners;

            for (auto& function : functions)
            {
                if (function.returnType == "void" && function.name == "onEvent" &&
                    function.argumentTypes.getSize() == 1 && function.argumentTypes[0].contains("ComponentRemovedEvent"))
                {
                    for (auto& component : loadedComponents)
                    {
                        if (function.argumentTypes[0].contains(" ComponentRemovedEvent<" + component.name + "> ") ||
                            function.argumentTypes[0].contains(" ComponentRemovedEvent<" + component.name + ">& "))
                        {
                            componentRemovedListeners.add(component);
                            break;
                        }
                    }
                }
            }

            return componentRemovedListeners;
        }

        DynamicArray<_Component> _loadComponents(const DynamicArray<String> components)
        {
            DynamicArray<_Component> loadedComponents;

            for (auto& component : components)
            {
                _Component loadedComponent;

                uint64 lastSeperator = component.findLastOf('/');
                FilePath filePath = component.startsWith("otto/") ? "C:/dev/Otto/Otto/src/" + component + ".hpp" : Application::getRootDirectory() + component + ".hpp";

                loadedComponent.relativeFilePath = component;
                loadedComponent.name = lastSeperator != component.getSize() ? String::subString(component, lastSeperator + 1) : component;
                _hasSerializationFunctions(filePath, loadedComponent.name, &loadedComponent.hasSerializationFunction, &loadedComponent.hasDeserializationFunction);

                loadedComponents.add(loadedComponent);
            }

            return loadedComponents;
        }

        DynamicArray<_Event> _loadEvents(const DynamicArray<String> events)
        {
            DynamicArray<_Event> loadedEvents;

            for (auto& e : events)
            {
                _Event loadedEvent;

                uint64 lastSeperator = e.findLastOf('/');
                loadedEvent.name = lastSeperator != e.getSize() ? String::subString(e, lastSeperator + 1) : e;
                loadedEvent.relativeFilePath = e;

                loadedEvents.add(loadedEvent);
            }

            return loadedEvents;
        }

        DynamicArray<_Behaviour> _loadBehaviours(const DynamicArray<String> behaviours, const DynamicArray<_Event>& loadedEvents)
        {
            DynamicArray<_Behaviour> loadedBehaviours;

            for (auto& behaviour : behaviours)
            {
                _Behaviour loadedBehaviour;

                uint64 lastSeperator = behaviour.findLastOf('/');
                FilePath filePath = behaviour.startsWith("otto/") ? "C:/dev/Otto/Otto/src/" + behaviour + ".hpp" : Application::getRootDirectory() + behaviour + ".hpp";

                loadedBehaviour.name = lastSeperator != behaviour.getSize() ? String::subString(behaviour, lastSeperator + 1) : behaviour;
                loadedBehaviour.relativeFilePath = behaviour;

                auto functions = _findMemberFunctions(filePath, loadedBehaviour.name);
                if (!functions.hasValue())
                {
                    Log::error("Error in behaviour file ", filePath, ": File does not contain class ", loadedBehaviour.name);
                    continue;
                }

                _hasSerializationFunctions(filePath, loadedBehaviour.name, &loadedBehaviour.hasSerializationFunction, &loadedBehaviour.hasDeserializationFunction);
                loadedBehaviour.eventFunctions = _findEventFunctions(functions.getValue(), loadedEvents);

                loadedBehaviours.add(loadedBehaviour);
            }

            return loadedBehaviours;
        }

        DynamicArray<_System> _loadSystems(const DynamicArray<String>& systems, 
            const DynamicArray<_Component>& loadedComponents, const DynamicArray<_Event>& loadedEvents)
        {
            DynamicArray<_System> loadedSystems;

            for (auto& system : systems)
            {
                _System loadedSystem;

                uint64 lastSeperator = system.findLastOf('/');
                FilePath filePath = system.startsWith("otto/") ? "C:/dev/Otto/Otto/src/" + system + ".hpp" : Application::getRootDirectory() + system + ".hpp";

                loadedSystem.name = lastSeperator != system.getSize() ? String::subString(system, lastSeperator + 1) : system;
                loadedSystem.relativeFilePath = system;

                auto functions = _findMemberFunctions(filePath, loadedSystem.name);

                loadedSystem.constructorArgs = _findConstructorArguments(loadedSystem.name, functions.getValue());
                loadedSystem.eventFunctions = _findEventFunctions(functions.getValue(), loadedEvents);
                loadedSystem.componentAddedListeners = _findComponentAddedListeners(functions.getValue(), loadedComponents);
                loadedSystem.componentRemovedListeners = _findComponentRemovedListeners(functions.getValue(), loadedComponents);

                loadedSystems.add(loadedSystem);
            }

            return loadedSystems;
        }

        _Package _loadPackage(const Package& package)
        {
            _Package loadedPackage;

            loadedPackage.components = _loadComponents(package.components);
            loadedPackage.events = _loadEvents(package.events);
            loadedPackage.behaviours = _loadBehaviours(package.behaviours, loadedPackage.events);
            loadedPackage.systems = _loadSystems(package.systems, loadedPackage.components, loadedPackage.events);

            for (auto& behaviour : loadedPackage.behaviours)
            {
                if (!loadedPackage.views.contains(behaviour.name))
                    loadedPackage.views.add(behaviour.name);
            }

            for (auto& system : loadedPackage.systems)
            {
                for (auto& arg : system.constructorArgs)
                {
                    if (arg.isMultiView)
                    {
                        if (!loadedPackage.multiViews.contains({ arg.view1Type, arg.view2Type }))
                            loadedPackage.multiViews.add({ arg.view1Type, arg.view2Type });
                    }
                    else if (!loadedPackage.views.contains(arg.view1Type))
                        loadedPackage.views.add(arg.view1Type);
                }
            }

            return loadedPackage;
        }

        void _addComponentAndEventIncludes(String& code, const _Package& package)
        {
            for (auto& component : package.components)
                code.append("#include \"" + component.relativeFilePath.toString() + ".hpp\"\n");
            for (auto& e : package.events)
                code.append("#include \"" + e.relativeFilePath.toString() + ".hpp\"\n");

            code.append('\n');
        }

        void _addComponentAndEventFuncDefinitions(String& code, const _Package& package)
        {
            for (auto& component : package.components)
            {
                code.append("template<>\n"
                    "OTTO_DLL_FUNC void Scene::addComponent<" + component.name + ">(Entity entity, const " + component.name + "& component);\n");
                code.append("template<>\n"
                    "OTTO_DLL_FUNC void Scene::removeComponent<" + component.name + ">(Entity entity);\n");
                code.append("template<>\n"
                    "OTTO_DLL_FUNC " + component.name + "& Scene::getComponent<" + component.name + ">(Entity entity);\n");
                code.append("template<>\n"
                    "OTTO_DLL_FUNC bool8 Scene::hasComponent<" + component.name + ">(Entity entity);\n");
            }

            code.append('\n');

            for (auto& e : package.events)
            {
                code.append("template<>\n"
                    "OTTO_DLL_FUNC void Scene::addEventListener<" + e.name + ">(const EventListener<" + e.name + ">& eventListener);\n");
                code.append("template<>\n"
                    "OTTO_DLL_FUNC void Scene::removeEventListener<" + e.name + ">(const EventListener<" + e.name + ">& eventListener);\n");
                code.append("template<>\n"
                    "OTTO_DLL_FUNC void Scene::dispatchEvent<" + e.name + ">(const " + e.name + "& e);\n");
            }

            code.append('\n');
        }

        void _addBehaviourAndSystemIncludes(String& code, const _Package& package)
        {
            for (auto& behaviour : package.behaviours)
                code.append("#include \"" + behaviour.relativeFilePath.toString() + ".hpp\"\n");
            for (auto& system : package.systems)
                code.append("#include \"" + system.relativeFilePath.toString() + ".hpp\"\n");

            code.append('\n');
        }

        void _addSceneDataStruct(String& code, const _Package& package)
        {
            code.append("    struct SceneData\n");
            code.append("    {\n");

            code.append("        Entity nextEntity = 1;\n");
            code.append('\n');

            for (auto& component : package.components)
                code.append("        ComponentPool<" + component.name + "> " + String::untitle(component.name) + "Pool;\n");

            code.append('\n');

            for (auto& behaviour : package.behaviours)
                code.append("        ComponentPool<" + behaviour.name + "> " + String::untitle(behaviour.name) + "Pool;\n");

            code.append('\n');

            for (auto& e : package.events)
                code.append("        EventDispatcher<" + e.name + "> " + String::untitle(e.name) + "Dispatcher;\n");

            code.append('\n');

            for (auto& viewType : package.views)
                code.append("        View<" + viewType + "> " + String::untitle(viewType) + "View = " 
                    "View<" + viewType + ">(&" + String::untitle(viewType) + "Pool); \n");

            code.append('\n');

            for (auto& [type1, type2] : package.multiViews)
                code.append("        MultiView<" + type1 + ", " + type2 + "> " + String::untitle(type1) + "_" + String::untitle(type2) + "View = "
                    "MultiView<" + type1 + ", " + type2 + ">(&" + String::untitle(type1) + "Pool, &" + String::untitle(type2) + "Pool);\n");

            for (auto& system : package.systems)
            {
                code.append("        " + system.name + ' ' + String::untitle(system.name) + " = " + system.name + "(");

                bool8 first = true;
                for (auto& argument : system.constructorArgs)
                {
                    if (!first)
                        code.append(", ");

                    if (argument.isMultiView)
                        code.append("&" + String::untitle(argument.view1Type) + "_" + String::untitle(argument.view2Type) + "View");
                    else
                        code.append("&" + String::untitle(argument.view1Type) + "View");

                    first = false;
                }

                code.append(");\n");
            }

            code.append("        Map<Entity, DynamicArray<uint16>> entityComponentMap;");

            code.append("    };\n");

            code.append('\n');
        }

        void _addComponentIDFunctions(String& code, const _Package& package)
        {
            code.append("    template<typename C>\n"
                "    static uint16 getID()\n"
                "    {\n"
                "        static_assert(false);\n"
                "    }\n"
            );

            for (uint64 i = 0; i < package.components.getSize(); i++)
            {
                code.append("    template<>\n");
                code.append("    static uint16 getID<" + package.components[i].name + ">()\n");
                code.append("    {\n");
                code.append("        return " + String::valueOf(i) + ";\n");
                code.append("    }\n");

                code.append('\n');
            }
        }

        void _addSerializationFunctions(String& code, const _Package& package)
        {
            code.append("    template<typename C>\n"
                "    Serialized serializeComponentOrBehaviour(const C& component, const Map<String, Entity>& entities)\n"
                "    {\n"
                "        return C();\n"
                "    }\n"
            );

            code.append('\n');

            for (auto& component : package.components)
            {
                if (component.hasSerializationFunction)
                {
                    code.append("    template<>\n");
                    code.append("    Serialized serializeComponentOrBehaviour<" + component.name + ">(const " + component.name + "& component, const Map<String, Entity>& entities)\n");
                    code.append("    {\n");
                    code.append("        return serializeComponent<" + component.name + ">(component, entities);\n");
                    code.append("    }\n");

                    code.append('\n');
                }
            }

            for (auto& behaviour : package.behaviours)
            {
                if (behaviour.hasSerializationFunction)
                {
                    code.append("    template<>\n");
                    code.append("    Serialized serializeComponentOrBehaviour<" + behaviour.name + ">(const " + behaviour.name + "& behaviour, const Map<String, Entity>& entities)\n");
                    code.append("    {\n");
                    code.append("        return serializeComponent<" + behaviour.name + ">(behaviour, entities);\n");
                    code.append("    }\n");

                    code.append('\n');
                }
            }
        }

        void _addDeserializationFunctions(String& code, const _Package& package)
        {
            code.append("    template<typename C>\n"
                "    C deserializeComponentOrBehaviour(const Serialized& args, const Map<String, Entity>& entities)\n"
                "    {\n"
                "        return C();\n"
                "    }\n"
            );

            code.append('\n');

            for (auto& component : package.components)
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

            for (auto& behaviour : package.behaviours)
            {
                if (behaviour.hasDeserializationFunction)
                {
                    code.append("    template<>\n");
                    code.append("    " + behaviour.name + " deserializeComponentOrBehaviour<" + behaviour.name + ">(const Serialized& args, const Map<String, Entity>& entities)\n");
                    code.append("    {\n");
                    code.append("        return deserializeComponent<" + behaviour.name + ">(args, entities);\n");
                    code.append("    }\n");

                    code.append('\n');
                }
            }
        }

        void _addOtherSceneFunctions(String& code)
        {
            code.append("    OTTO_DLL_FUNC Shared<Scene> Scene::_createScene()\n"
                "    {\n"
                "        return new Scene(new SceneData());\n"
                "    }\n"
            );

            code.append('\n');

            code.append("    OTTO_DLL_FUNC void Scene::_initClient(Application* mainApplication, Window* mainWindow, Log* mainLog, const Color& clearColor)\n"
                "    {\n"
                "        Application::init(mainApplication, mainWindow, mainLog);\n"
                "        Window::setClearColor(clearColor);\n"
                "    }\n"
            );

            code.append('\n');

            code.append("    OTTO_DLL_FUNC Entity Scene::addEntity()\n"
                "    {\n"
                "        Entity entity = mData->nextEntity++;\n"
                "        mData->entityComponentMap.insert(entity, DynamicArray<uint16>());\n"
                "        return entity;\n"
                "    }\n"
            );

            code.append('\n');

            code.append("    OTTO_DLL_FUNC void Scene::update(float32 delta)\n"
                "    {\n"
                "        dispatchEvent(_UpdateEvent(delta));\n"
                "    }\n"
            );

            code.append('\n');

            code.append("    OTTO_DLL_FUNC void Scene::rebuffer()\n"
                "    {\n"
                "        dispatchEvent(_RebufferEvent());\n"
                "    }\n"
            );

            code.append('\n');

            code.append("        OTTO_DLL_FUNC void Scene::render()\n"
                "    {\n"
                "        Window::clear();\n"
                "        dispatchEvent(_RenderEvent());\n"
                "        Window::swapBuffers();\n"
                "    }\n"
            );

            code.append('\n');
        }

        void _addRemoveEntityFunction(String& code, const _Package& package)
        {
            code.append("    OTTO_DLL_FUNC void Scene::removeEntity(Entity entity)\n");
            code.append("    {\n");
            code.append("        for (uint16 componentID : mData->entityComponentMap[entity])\n");
            code.append("        {\n");
            code.append("            switch(componentID)\n");
            code.append("            {\n");

            for (uint64 i = 0; i < package.components.getSize(); i++)
                code.append("            case " + String::valueOf(i) + ": removeComponent<" + package.components[i].name + ">(entity); break;\n");

            code.append("            }\n");
            code.append("        }\n\n");
            code.append("        mData->entityComponentMap.remove(entity);\n");
            code.append("    }\n");

            code.append('\n');
        }

        void _addSceneInitFunction(String& code, const _Package& package)
        {
            code.append("    OTTO_DLL_FUNC void Scene::init()\n");
            code.append("    {\n");

            for (auto& system : package.systems)
                code.append("        mData->" + String::untitle(system.name) + ".mScene = this;\n");

            for (auto& behaviour : package.behaviours)
            {
                code.append("        for (auto [entity, behaviour] : mData->" + String::untitle(behaviour.name) + "View)\n");
                code.append("        {\n");
                code.append("            behaviour.mScene = this;\n");
                code.append("            behaviour.mEntity = entity;\n");
                code.append("        }\n");
            }

            code.append('\n');
            code.append("        dispatchEvent(_InitEvent());\n");

            code.append("    }\n");

            code.append('\n');
        }

        void _addAddEventListenerFunctions(String& code, const _Package& package)
        {
            code.append("    template<typename E>\n"
                "    OTTO_DLL_FUNC void Scene::addEventListener(const EventListener<E>& eventListener)\n"
                "    {\n"
                "        OTTO_ASSERT(false, \"Event is not added.\")\n"
                "    }\n"
            );

            code.append('\n');

            for (auto& e : package.events)
            {
                code.append("    template<>\n");
                code.append("    OTTO_DLL_FUNC void Scene::addEventListener<" + e.name + ">(const EventListener<" + e.name + ">& eventListener)\n");
                code.append("    {\n");
                code.append("        mData->" + String::untitle(e.name) + "Dispatcher.addListener(eventListener);\n");
                code.append("    }\n");

                code.append('\n');
            }
        }

        void _addRemoveEventListenerFunctions(String& code, const _Package& package)
        {
            code.append("    template<typename E>\n"
                "    OTTO_DLL_FUNC void Scene::removeEventListener(const EventListener<E>& eventListener)\n"
                "    {\n"
                "        OTTO_ASSERT(false, \"Event is not added.\")\n"
                "    }\n"
            );

            code.append('\n');

            for (auto& e : package.events)
            {
                code.append("    template<>\n");
                code.append("    OTTO_DLL_FUNC void Scene::removeEventListener<" + e.name + ">(const EventListener<" + e.name + ">& eventListener)\n");
                code.append("    {\n");
                code.append("        mData->" + String::untitle(e.name) + "Dispatcher.removeListener(eventListener);\n");
                code.append("    }\n");

                code.append('\n');
            }
        }

        void _addDispatchEventFunctions(String& code, const _Package& package)
        {
            code.append("    template<typename E>\n"
                "    OTTO_DLL_FUNC void Scene::dispatchEvent(const E& e)\n"
                "    {\n"
                "        OTTO_ASSERT(false, \"Event is not added.\")\n"
                "    }\n"
            );

            code.append('\n');

            for (auto& e : package.events)
            {
                code.append("    template<>\n");
                code.append("    OTTO_DLL_FUNC void Scene::dispatchEvent<" + e.name + ">(const " + e.name + "& e)\n");
                code.append("    {\n");

                for (auto& system : package.systems)
                {
                    if (system.eventFunctions.contains(e))
                        code.append("        mData->" + String::untitle(system.name) + ".onEvent(e);\n");
                }

                for (auto& behaviour : package.behaviours)
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
        }

        void _addAddComponentFunctions(String& code, const _Package& package)
        {
            code.append("    OTTO_DLL_FUNC void Scene::addComponent(Entity entity, const String& componentName, const Serialized& args, const Map<String, Entity>& entities)\n");
            code.append("    {\n");

            for (auto& component : package.components)
            {
                code.append("        if (componentName == \"" + component.name + "\")\n");
                code.append("            addComponent(entity, deserializeComponentOrBehaviour<" + component.name + ">(args, entities));\n");
            }

            for (auto& behaviour : package.behaviours)
            {
                code.append("        if (componentName == \"" + behaviour.name + "\")\n");
                code.append("            mData->" + String::untitle(behaviour.name) + "Pool.addComponent(entity, deserializeComponentOrBehaviour<" + behaviour.name + ">(args, entities));\n");
            }

            code.append("    }\n");

            code.append('\n');

            code.append("    template<typename C>\n"
                "    OTTO_DLL_FUNC void Scene::addComponent(Entity entity, const C& component)\n"
                "    {\n"
                "        OTTO_ASSERT(false, \"Component is not added.\")\n"
                "    }\n"
            );

            code.append('\n');

            for (auto& component : package.components)
            {
                code.append("    template<>\n");
                code.append("    OTTO_DLL_FUNC void Scene::addComponent<" + component.name + ">(Entity entity, const " + component.name + "& component)\n");
                code.append("    {\n");
                code.append("        mData->" + String::untitle(component.name) + "Pool.addComponent(entity, component);\n");

                for (auto& [type1, type2] : package.multiViews)
                {
                    if (type1 == component.name)
                        code.append("        mData->" + String::untitle(type1) + "_" + String::untitle(type2) + "View.onComponent1Added(entity);\n");
                    else if (type2 == component.name)
                        code.append("        mData->" + String::untitle(type1) + "_" + String::untitle(type2) + "View.onComponent2Added(entity);\n");
                }

                for (auto& system : package.systems)
                {
                    if (system.componentAddedListeners.contains(component))
                        code.append("        mData->" + String::untitle(system.name) + ".onEvent(ComponentAddedEvent<" + component.name + ">(entity));\n");
                }

                code.append("        mData->entityComponentMap[entity].add(getID<" + component.name + ">());\n");

                code.append("    }\n");

                code.append('\n');
            }
        }

        void _addRemoveComponentFunctions(String& code, const _Package& package)
        {
            code.append("    template<typename C>\n"
                "    OTTO_DLL_FUNC void Scene::removeComponent(Entity entity)\n"
                "    {\n"
                "        OTTO_ASSERT(false, \"Component is not added.\")\n"
                "    }\n"
            );

            code.append('\n');

            for (auto& component : package.components)
            {
                code.append("    template<>\n");
                code.append("    OTTO_DLL_FUNC void Scene::removeComponent<" + component.name + ">(Entity entity)\n");
                code.append("    {\n");
                code.append("        mData->" + String::untitle(component.name) + "Pool.removeComponent(entity);\n");

                for (auto& [type1, type2] : package.multiViews)
                {
                    if (type1 == component.name)
                        code.append("        mData->" + String::untitle(type1) + "_" + String::untitle(type2) + "View.onComponent1Removed(entity);\n");
                    else if (type2 == component.name)
                        code.append("        mData->" + String::untitle(type1) + "_" + String::untitle(type2) + "View.onComponent2Removed(entity);\n");
                }

                for (auto& system : package.systems)
                {
                    if (system.componentRemovedListeners.contains(component))
                        code.append("        mData->" + String::untitle(system.name) + ".onEvent(ComponentRemovedEvent<" + component.name + ">(entity));\n");
                }

                code.append("        mData->entityComponentMap[entity].remove(mData->entityComponentMap[entity].indexOf(getID<" + component.name + ">()));\n");

                code.append("    }\n");

                code.append('\n');
            }
        }

        void _addGetComponentFunctions(String& code, const _Package& package)
        {
            code.append("    template<typename C>\n"
                "    OTTO_DLL_FUNC C& Scene::getComponent(Entity entity)\n"
                "    {\n"
                "        OTTO_ASSERT(false, \"Component is not added.\")\n"
                "    }\n"
            );

            code.append('\n');

            for (auto& component : package.components)
            {
                code.append("    template<>\n");
                code.append("    OTTO_DLL_FUNC " + component.name + "& Scene::getComponent<" + component.name + ">(Entity entity)\n");
                code.append("    {\n");
                code.append("        return mData->" + String::untitle(component.name) + "Pool.getComponent(entity);\n");
                code.append("    }\n");

                code.append('\n');
            }
        }

        void _addHasComponentFunctions(String& code, const _Package& package)
        {
            code.append("    template<typename C>\n"
                "    OTTO_DLL_FUNC bool8 Scene::hasComponent(Entity entity)\n"
                "    {\n"
                "        OTTO_ASSERT(false, \"Component is not added.\")\n"
                "    }\n"
            );

            code.append('\n');

            for (auto& component : package.components)
            {
                code.append("    template<>\n");
                code.append("    OTTO_DLL_FUNC bool8 Scene::hasComponent<" + component.name + ">(Entity entity)\n");
                code.append("    {\n");
                code.append("        return mData->entityComponentMap[entity].contains(getID<" + component.name + ">());\n");
                code.append("    }\n");

                code.append('\n');
            }
        }

        void _createSceneFile(const _Package& package, const FilePath& filePath)
        {
            String code;

            code.append("#define OTTO_GENERATED_CODE\n");
            code.append("#include \"otto.h\"\n");
            code.append('\n');

            code.append("using namespace otto;\n\n");

            _addComponentAndEventIncludes(code, package);
            _addComponentAndEventFuncDefinitions(code, package);
            _addBehaviourAndSystemIncludes(code, package);


            code.append("namespace otto\n{\n");

            _addSceneDataStruct(code, package);
            _addComponentIDFunctions(code, package);
            _addSerializationFunctions(code, package);
            _addDeserializationFunctions(code, package);
            _addOtherSceneFunctions(code);
            _addRemoveEntityFunction(code, package);
            _addSceneInitFunction(code, package);
            _addAddEventListenerFunctions(code, package);
            _addRemoveEventListenerFunctions(code, package);
            _addDispatchEventFunctions(code, package);
            _addAddComponentFunctions(code, package);
            _addRemoveComponentFunctions(code, package);
            _addGetComponentFunctions(code, package);
            _addHasComponentFunctions(code, package);

            code.append("} // namespace otto\n");

            FileUtils::createDirectoryRecursively(filePath.getParentDirectory());
            File(filePath).write(code);
        }
#endif

    } // namespace

    Result<Shared<Scene>, SceneLoader::SceneLoadingError> SceneLoader::loadScene(const FilePath& filePath)
    {
        OTTO_DECLARE_DLL_FUNCTION_HANDLE(createSceneHandle, Application::_getClientDllPath(), Shared<Scene>, otto::Scene::_createScene);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneAddEntityHandle, Application::_getClientDllPath(), Scene, Entity, addEntity);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneAddComponentHandle, Application::_getClientDllPath(), Scene, void, addComponent, 
            Entity, const String&, const Serialized&, const Map<String, Entity>&);

        Shared<Scene> scene = OTTO_CALL_DLL_FUNCTION(createSceneHandle);

        auto file = Serializer::load(filePath);
        if (file.hasError())
        {
            Log::error("Failed to load Scene ", filePath, ": Syntax error");
            return SYNTAX_ERROR;
        }

        Map<String, Entity> entities;
        for (auto& [entity, _] : file.getResult().getDictionary())
        {
            if (entities.containsKey(entity))
            {
                Log::error("Failed to load scene ", filePath, ": duplicate Entity ", entity);
                return SceneLoadingError::DUPLICATE_ENTITY;
            }

            entities.insert(entity, OTTO_CALL_DLL_MEMBER_FUNCTION(*scene.get(), sceneAddEntityHandle));
        }

        for (auto& [entityName, components] : file.getResult().getDictionary())
        {
            for (auto& [componentName, args] : components.getDictionary())
                OTTO_CALL_DLL_MEMBER_FUNCTION(*scene.get(), sceneAddComponentHandle, entities.get(entityName), componentName, args, entities);
        }

        return scene;
    }

#ifdef OTTO_DYNAMIC
    bool8 SceneLoader::reloadDll(const Package& package)
    {
        Log::trace("Generating scene code...");
        _createSceneFile(_loadPackage(package), Application::getRootDirectory() + ".tmp/generated_scene.cpp");

        bool8 dllReloaded = DllReloader::reloadDll(Application::getRootDirectory() + ".tmp/client.dll", true, {
            Application::getRootDirectory() + ".tmp/generated_scene.cpp" }, { "GLEW_STATIC", "OTTO_DEBUG", "OTTO_DYNAMIC", "OTTO_EXPORT" }, 
            { }, { "/LTCG", "/ignore:4075", "/ignore:4098" });

        if (!dllReloaded)
            return false;

        return true;
    }

    void SceneLoader::initClient(Application* mainApplication, Window* mainWindow, Log* mainLog, const Color& clearColor)
    {
        OTTO_DECLARE_DLL_FUNCTION_HANDLE(initClientHandle, Application::_getClientDllPath(), void, 
            otto::Scene::_initClient, Application*, Window*, Log*, const Color&);

        OTTO_CALL_DLL_FUNCTION(initClientHandle, mainApplication, mainWindow, mainLog, clearColor);
    }
#endif

} // namespace otto
