#include "otto/scene/scene.h"

#include "./behaviours/TestBehaviour.hpp"
#include "./components/TestComponent.hpp"

#define OTTO_DLL_EXPORT

namespace otto
{
    struct SceneData
    {
        EventListener<TestEvent> testEventListener;

        ComponentPool<TestComponent> testComponentPool;
    };

    enum SceneType : uint16
    {
        TEST_SCENE,
    };

    OTTO_RCR_API String getClientDllPath()
    {
        return "C:/ProgramData/Otto/Client/Client.dll";
    }

    OTTO_RCR_API void initClientLog(Log* mainLog)
    {
        otto::Log::init(mainLog);
    }

    OTTO_RCR_API Scene* Scene::createScene(const String& sceneName)
    {
        return new Scene(new SceneData());
    }

    OTTO_RCR_API void Scene::init()
    {

    }

    OTTO_RCR_API void Scene::update(float32 delta)
    {

    }

    template<typename E>
    OTTO_RCR_API void Scene::addEventListener(const EventListener<E>& listener)
    {
        OTTO_ASSERT(false, "Event is not registered.");
    }

    template<typename E>
    OTTO_RCR_API void Scene::removeEventListener(const EventListener<E>& listener)
    {
        OTTO_ASSERT(false, "Event is not registered.");
    }

    template<typename E>
    OTTO_RCR_API void Scene::dispatchEvent(const E& e)
    {
        OTTO_ASSERT(false, "Event is not registered.");
    }

    template<typename C>
    OTTO_RCR_API void Scene::addComponent(const C& component)
    {
        OTTO_ASSERT(false, "Component is not registered.");
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener(const EventListener<TestEvent>& listener)
    {
        mData->testEventListener.addListener(listener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener(const EventListener<TestEvent>& listener)
    {
        mData->testEventListener.removeListener(listener);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent(const TestEvent& e)
    {
        mData->testEventListener.dispatch(e);
    }
    
    template<>
    OTTO_RCR_API void Scene::addComponent(const TestComponent& component)
    {
        mData->testComponentPool.add(component);
    }
}
