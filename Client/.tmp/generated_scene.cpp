#define OTTO_GENERATED_CODE
#include "C:/dev/otto/otto/src/otto/base.h"
#include "otto/scene/scene.h"
#include "otto/event/event_dispatcher.h"
#include <iostream>

#include "otto\components\TransformComponent.hpp"
#include "components\TestComponent.hpp"
#include "components\TestComponent2.hpp"
#include "behaviours\TestBehaviour.hpp"
#include "behaviours\TestBehaviour2.hpp"
#include "systems\TestSystem.hpp"
#include "systems\TestSystem2.hpp"
#include "events\TestEvent.hpp"
#include "events\TestEvent2.hpp"

namespace otto
{
    struct SceneData
    {
        Entity nextEntity = 0;

        ComponentPool<TransformComponent> transformComponentPool;
        ComponentPool<TestComponent> testComponentPool;
        ComponentPool<TestComponent2> testComponent2Pool;

        ComponentPool<TestBehaviour> testBehaviourPool;
        ComponentPool<TestBehaviour2> testBehaviour2Pool;

        EventDispatcher<TestEvent> testEventDispatcher;
        EventDispatcher<TestEvent2> testEvent2Dispatcher;

        TestSystem testSystem;
        TestSystem2 testSystem2;

        View<TestBehaviour> testBehaviourView = View<TestBehaviour>(&testBehaviourPool);
        View<TestBehaviour2> testBehaviour2View = View<TestBehaviour2>(&testBehaviour2Pool);
        View<TestComponent> testComponentView = View<TestComponent>(&testComponentPool);

        MultiView<TestComponent, TestComponent2> testComponent_testComponent2View = MultiView<TestComponent, TestComponent2>(&testComponentPool, &testComponent2Pool);
    };

    template<typename C>
    C deserializeComponentOrBehaviour(const Serialized& args, const Map<String, Entity>& entities)
    {
        return C();
    }

    template<>
    TransformComponent deserializeComponentOrBehaviour<TransformComponent>(const Serialized& args, const Map<String, Entity>& entities)
    {
        return deserializeComponent<TransformComponent>(args, entities);
    }

    template<>
    TestComponent deserializeComponentOrBehaviour<TestComponent>(const Serialized& args, const Map<String, Entity>& entities)
    {
        return deserializeComponent<TestComponent>(args, entities);
    }

    OTTO_RCR_API Shared<Scene> _SceneInitializer::createScene()
    {
        return new Scene(new SceneData());
    }

    OTTO_RCR_API void _SceneInitializer::initClientLog(Log* mainLog)
    {
        Log::init(mainLog);
    }

    OTTO_RCR_API void Scene::init()
    {
        mData->testSystem.onInit(this, &mData->testComponent_testComponent2View, &mData->testComponentView);
        for (auto [entity, behaviour] : mData->testBehaviourView)
            behaviour.onInit();
        for (auto [entity, behaviour] : mData->testBehaviour2View)
            behaviour.onInit();
    }

    OTTO_RCR_API void Scene::update(float32 delta)
    {
        mData->testSystem.onUpdate(delta);
        mData->testSystem2.onUpdate(delta);
        for (auto [entity, behaviour] : mData->testBehaviourView)
            behaviour.onUpdate(delta);
        for (auto [entity, behaviour] : mData->testBehaviour2View)
            behaviour.onUpdate(delta);
    }

    OTTO_RCR_API Entity Scene::addEntity()
    {
        return mData->nextEntity++;
    }

    OTTO_RCR_API void Scene::addComponent(Entity entity, const String& componentName, const Serialized& args, const EntityMap& entities)
    {
        if (componentName == "TransformComponent")
            mData->transformComponentPool.addComponent(entity, deserializeComponentOrBehaviour<TransformComponent>(args, entities));
        if (componentName == "TestComponent")
            mData->testComponentPool.addComponent(entity, deserializeComponentOrBehaviour<TestComponent>(args, entities));
        if (componentName == "TestComponent2")
            mData->testComponent2Pool.addComponent(entity, deserializeComponentOrBehaviour<TestComponent2>(args, entities));
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
        mData->testSystem.onEvent(e);
        for (auto [entity, behaviour] : mData->testBehaviourView)
            behaviour.onEvent(e);
        mData->testEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<TestEvent2>(const TestEvent2& e)
    {
        mData->testSystem.onEvent(e);
        mData->testEvent2Dispatcher.dispatchEvent(e);
    }

    template<typename C>
    OTTO_RCR_API void Scene::addComponent(Entity entity, const C& component)
    {
        OTTO_ASSERT(false, "Component is not added.")
    }

    template<>
    OTTO_RCR_API void Scene::addComponent<TransformComponent>(Entity entity, const TransformComponent& component)
    {
        mData->transformComponentPool.addComponent(entity, component);
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
    OTTO_RCR_API void Scene::removeComponent<TransformComponent>(Entity entity)
    {
        mData->transformComponentPool.removeComponent(entity);
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
    OTTO_RCR_API TransformComponent& Scene::getComponent<TransformComponent>(Entity entity)
    {
        return mData->transformComponentPool.getComponent(entity);
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
    OTTO_RCR_API bool Scene::hasComponent<TransformComponent>(Entity entity)
    {
        return mData->transformComponentPool.hasComponent(entity);
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
