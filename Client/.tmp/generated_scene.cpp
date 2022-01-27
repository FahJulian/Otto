#include "C:/dev/otto/otto/src/otto/base.h"
#include "otto/scene/scene.h"
#include "otto/event/event_dispatcher.h"
#include <iostream>

#include "components/TestComponent.hpp"
#include "components/TestComponent2.hpp"
#include "behaviours/TestBehaviour.hpp"
#include "behaviours/TestBehaviour2.hpp"
#include "systems/TestSystem.hpp"
#include "systems/TestSystem2.hpp"
#include "events/TestEvent.hpp"
#include "events/TestEvent2.hpp"

namespace otto
{
    struct SceneData
    {
        Entity nextEntity = 0;

        ComponentPool<TestComponent> testComponentPool;
        ComponentPool<TestComponent2> testComponent2Pool;

        ComponentPool<TestBehaviour> testBehaviourPool;
        ComponentPool<TestBehaviour2> testBehaviour2Pool;

        EventDispatcher<TestEvent> testEventDispatcher;
        EventDispatcher<TestEvent2> testEvent2Dispatcher;

        TestSystem testSystem;
        TestSystem2 testSystem2;
    };

    OTTO_RCR_API Scene* _SceneInitializer::createScene()
    {
        return new Scene(new SceneData());
    }

    OTTO_RCR_API void _SceneInitializer::initClientLog(Log* mainLog)
    {
        Log::init(mainLog);
    }

    OTTO_RCR_API void Scene::init()
    {
    }

    OTTO_RCR_API void Scene::update(float32 delta)
    {
    }

    OTTO_RCR_API Entity Scene::addEntity()
    {
        return mData->nextEntity++;
    }

    template<typename E>
    OTTO_RCR_API void Scene::addEventListener(const EventListener<E>& eventListener)
    {
        OTTO_ASSERT(false, "Event is not added.")
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<TestEvent>(const EventListener<TestEvent>& eventListener)
    {
        mData->testEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<TestEvent2>(const EventListener<TestEvent2>& eventListener)
    {
        mData->testEvent2Dispatcher.addListener(eventListener);
    }

    template<typename E>
    OTTO_RCR_API void Scene::removeEventListener(const EventListener<E>& eventListener)
    {
        OTTO_ASSERT(false, "Event is not added.")
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<TestEvent>(const EventListener<TestEvent>& eventListener)
    {
        mData->testEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<TestEvent2>(const EventListener<TestEvent2>& eventListener)
    {
        mData->testEvent2Dispatcher.removeListener(eventListener);
    }

    template<typename E>
    OTTO_RCR_API void Scene::dispatchEvent(const E& e)
    {
        OTTO_ASSERT(false, "Event is not added.")
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<TestEvent>(const TestEvent& e)
    {
        mData->testEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<TestEvent2>(const TestEvent2& e)
    {
        mData->testEvent2Dispatcher.dispatchEvent(e);
    }

    template<typename C>
    OTTO_RCR_API void Scene::addComponent(Entity entity, const C& component)
    {
        OTTO_ASSERT(false, "Component is not added.")
    }

    template<>
    OTTO_RCR_API void Scene::addComponent<TestComponent>(Entity entity, const TestComponent& component)
    {
        mData->testComponentPool.addComponent(entity, component);
    }

    template<>
    OTTO_RCR_API void Scene::addComponent<TestComponent2>(Entity entity, const TestComponent2& component)
    {
        mData->testComponent2Pool.addComponent(entity, component);
    }

    template<typename C>
    OTTO_RCR_API void Scene::removeComponent(Entity entity)
    {
        OTTO_ASSERT(false, "Component is not added.")
    }

    template<>
    OTTO_RCR_API void Scene::removeComponent<TestComponent>(Entity entity)
    {
        mData->testComponentPool.removeComponent(entity);
    }

    template<>
    OTTO_RCR_API void Scene::removeComponent<TestComponent2>(Entity entity)
    {
        mData->testComponent2Pool.removeComponent(entity);
    }

    template<typename C>
    OTTO_RCR_API C& Scene::getComponent(Entity entity)
    {
        OTTO_ASSERT(false, "Component is not added.")
    }

    template<>
    OTTO_RCR_API TestComponent& Scene::getComponent<TestComponent>(Entity entity)
    {
        return mData->testComponentPool.getComponent(entity);
    }

    template<>
    OTTO_RCR_API TestComponent2& Scene::getComponent<TestComponent2>(Entity entity)
    {
        return mData->testComponent2Pool.getComponent(entity);
    }

    template<typename C>
    OTTO_RCR_API bool Scene::hasComponent(Entity entity)
    {
        OTTO_ASSERT(false, "Component is not added.")
    }

    template<>
    OTTO_RCR_API bool Scene::hasComponent<TestComponent>(Entity entity)
    {
        return mData->testComponentPool.hasComponent(entity);
    }

    template<>
    OTTO_RCR_API bool Scene::hasComponent<TestComponent2>(Entity entity)
    {
        return mData->testComponent2Pool.hasComponent(entity);
    }

} // namespace otto
