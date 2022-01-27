#include "scene_loader.h"

#include "otto/util/file.h"
#include "otto/util/platform/file_utils.h"

namespace otto
{
    static bool sInitialized = false;

#ifdef OTTO_DYNAMIC
    static DynamicArray<String> sComponents;
    static DynamicArray<String> sBehaviours;
    static DynamicArray<String> sSystems;
    static DynamicArray<String> sEvents;
#endif

    void SceneLoader::init(const DynamicArray<String>& components, const DynamicArray<String>& behaviours,
        const DynamicArray<String>& systems, const DynamicArray<String>& events)
    {
        if (sInitialized)
            return;

#ifdef OTTO_DYNAMIC
        sComponents = components;
        sBehaviours = behaviours;
        sSystems = systems;
        sEvents = events;
#endif

        sInitialized = true;
    }

    Result<Scene*, Serializer::ParsingError> SceneLoader::loadScene(const FilePath& filePath)
    {
        //Scene* scene = Helper().createScene();
        Scene* scene = nullptr;

        auto file = Serializer::deserialize(filePath);

        if (file.hasError())
            return file.getError();

        Map<String, Entity> entities;
        for (auto& [entityName, components] : file.getResult().getDictionary())
        {
            Entity entity = scene->addEntity();
            entities.insert(entityName, entity);

            for (auto& [componentName, args] : components.getDictionary())
            {
                scene->addComponent(componentName, args.getDictionary(), entities);
            }
        }

        return scene;
    }

    void SceneLoader::reloadDll()
    {
        _createSceneFile(Application::getRootDirectory() + ".tmp/generated_scene.cpp");

        DllReloader::reloadDll(Application::getRootDirectory() + ".tmp/client.dll", true, {
            Application::getRootDirectory() + ".tmp/generated_scene.cpp" }, { "OTTO_DEBUG" });

        _SceneInitializer initializer;

        Log* l = Log::getInstance();
        initializer.initClientLog(l);
        Scene* scene = initializer.createScene();
        scene->update(3);
        Entity entity1 = scene->addEntity();
        Entity entity2 = scene->addEntity();
    }

#ifdef OTTO_DYNAMIC
    void SceneLoader::_createSceneFile(const FilePath& filePath)
    {
        String code;

        code.append("#include \"C:/dev/otto/otto/src/otto/base.h\"\n");
        code.append("#include \"otto/scene/scene.h\"\n");
        code.append("#include \"otto/event/event_dispatcher.h\"\n");
        code.append("#include <iostream>\n");
        code.append('\n');

        for (auto& component : sComponents)
            code.append("#include \"" + component + ".hpp\"\n");
        for (auto& behaviour : sBehaviours)
            code.append("#include \"" + behaviour + ".hpp\"\n");
        for (auto& system : sSystems)
            code.append("#include \"" + system + ".hpp\"\n");
        for (auto& e : sEvents)
            code.append("#include \"" + e + ".hpp\"\n");

        code.append('\n');

        code.append("namespace otto\n");
        code.append("{\n");

        code.append("    struct SceneData\n");
        code.append("    {\n");

        code.append("        Entity nextEntity = 0;\n");
        code.append('\n');

        for (auto& component : sComponents)
        {
            uint64 lastSeperator = component.findLastOf('/');
            String componentName = lastSeperator != component.getSize() ? String::subString(component, lastSeperator + 1) : component;
            code.append("        ComponentPool<" + componentName + "> " + String::untitle(componentName) + "Pool;\n");
        }

        code.append('\n');

        for (auto& behaviour : sBehaviours)
        {
            uint64 lastSeperator = behaviour.findLastOf('/');
            String behaviourName = lastSeperator != behaviour.getSize() ? String::subString(behaviour, lastSeperator + 1) : behaviour;
            code.append("        ComponentPool<" + behaviourName + "> " + String::untitle(behaviourName) + "Pool;\n");
        }

        code.append('\n');

        for (auto& e : sEvents)
        {
            uint64 lastSeperator = e.findLastOf('/');
            String eventName = lastSeperator != e.getSize() ? String::subString(e, lastSeperator + 1) : e;
            code.append("        EventDispatcher<" + eventName + "> " + String::untitle(eventName) + "Dispatcher;\n");
        }

        code.append('\n');

        for (auto& system : sSystems)
        {
            uint64 lastSeperator = system.findLastOf('/');
            String systemName = lastSeperator != system.getSize() ? String::subString(system, lastSeperator + 1) : system;
            code.append("        " + systemName + ' ' + String::untitle(systemName) + ";\n");
        }

        code.append("    };\n");

        code.append('\n');

        code.append("    OTTO_RCR_API Scene* _SceneInitializer::createScene()\n"
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

        code.append("    OTTO_RCR_API void Scene::init()\n"
            "    {\n"
            "    }\n"
        );

        code.append('\n');

        code.append("    OTTO_RCR_API void Scene::update(float32 delta)\n"
            "    {\n"
            "    }\n"
        );

        code.append('\n');

        code.append("    OTTO_RCR_API Entity Scene::addEntity()\n"
            "    {\n"
            "        return mData->nextEntity++;\n"
            "    }\n"
        );

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
            uint64 lastSeperator = e.findLastOf('/');
            String eventName = lastSeperator != e.getSize() ? String::subString(e, lastSeperator + 1) : e;

            code.append("    template<>\n");
            code.append("    OTTO_RCR_API void Scene::addEventListener<" + eventName + ">(const EventListener<" + eventName + ">& eventListener)\n");
            code.append("    {\n");
            code.append("        mData->" + String::untitle(eventName) + "Dispatcher.addListener(eventListener);\n");
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
            uint64 lastSeperator = e.findLastOf('/');
            String eventName = lastSeperator != e.getSize() ? String::subString(e, lastSeperator + 1) : e;

            code.append("    template<>\n");
            code.append("    OTTO_RCR_API void Scene::removeEventListener<" + eventName + ">(const EventListener<" + eventName + ">& eventListener)\n");
            code.append("    {\n");
            code.append("        mData->" + String::untitle(eventName) + "Dispatcher.removeListener(eventListener);\n");
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
            uint64 lastSeperator = e.findLastOf('/');
            String eventName = lastSeperator != e.getSize() ? String::subString(e, lastSeperator + 1) : e;

            code.append("    template<>\n");
            code.append("    OTTO_RCR_API void Scene::dispatchEvent<" + eventName + ">(const " + eventName + "& e)\n");
            code.append("    {\n");
            code.append("        mData->" + String::untitle(eventName) + "Dispatcher.dispatchEvent(e);\n");
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
            uint64 lastSeperator = component.findLastOf('/');
            String componentName = lastSeperator != component.getSize() ? String::subString(component, lastSeperator + 1) : component;

            code.append("    template<>\n");
            code.append("    OTTO_RCR_API void Scene::addComponent<" + componentName + ">(Entity entity, const " + componentName + "& component)\n");
            code.append("    {\n");
            code.append("        mData->" + String::untitle(componentName) + "Pool.addComponent(entity, component);\n");
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
            uint64 lastSeperator = component.findLastOf('/');
            String componentName = lastSeperator != component.getSize() ? String::subString(component, lastSeperator + 1) : component;

            code.append("    template<>\n");
            code.append("    OTTO_RCR_API void Scene::removeComponent<" + componentName + ">(Entity entity)\n");
            code.append("    {\n");
            code.append("        mData->" + String::untitle(componentName) + "Pool.removeComponent(entity);\n");
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
            uint64 lastSeperator = component.findLastOf('/');
            String componentName = lastSeperator != component.getSize() ? String::subString(component, lastSeperator + 1) : component;

            code.append("    template<>\n");
            code.append("    OTTO_RCR_API " + componentName + "& Scene::getComponent<" + componentName + ">(Entity entity)\n");
            code.append("    {\n");
            code.append("        return mData->" + String::untitle(componentName) + "Pool.getComponent(entity);\n");
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
            uint64 lastSeperator = component.findLastOf('/');
            String componentName = lastSeperator != component.getSize() ? String::subString(component, lastSeperator + 1) : component;

            code.append("    template<>\n");
            code.append("    OTTO_RCR_API bool Scene::hasComponent<" + componentName + ">(Entity entity)\n");
            code.append("    {\n");
            code.append("        return mData->" + String::untitle(componentName) + "Pool.hasComponent(entity);\n");
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
