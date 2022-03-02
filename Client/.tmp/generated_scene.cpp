#define OTTO_GENERATED_CODE
#include "otto/base.h"
#include "otto/scene/scene.h"
#include "otto/event/event_dispatcher.h"

using namespace otto;

#include "otto\components\TransformComponent.hpp"
#include "components\TestComponent.hpp"
#include "components\TestComponent2.hpp"
#include "events\TestEvent.hpp"
#include "events\TestEvent2.hpp"
#include "otto\events\key\KeyPressedEvent.hpp"
#include "otto\events\key\KeyReleasedEvent.hpp"
#include "otto\events\mouse\MouseButtonPressedEvent.hpp"
#include "otto\events\mouse\MouseButtonReleasedEvent.hpp"
#include "otto\events\mouse\MouseMovedEvent.hpp"
#include "otto\events\mouse\MouseDraggedEvent.hpp"
#include "otto\events\mouse\MouseScrolledEvent.hpp"
#include "otto\events\window\WindowClosedEvent.hpp"
#include "otto\events\window\WindowResizedEvent.hpp"
#include "otto\events\window\WindowGainedFocusEvent.hpp"
#include "otto\events\window\WindowLostFocusEvent.hpp"

template<>
OTTO_RCR_API void Scene::addComponent<TransformComponent>(Entity entity, const TransformComponent& component);
template<>
OTTO_RCR_API void Scene::removeComponent<TransformComponent>(Entity entity);
template<>
OTTO_RCR_API TransformComponent& Scene::getComponent<TransformComponent>(Entity entity);
template<>
OTTO_RCR_API bool Scene::hasComponent<TransformComponent>(Entity entity);
template<>
OTTO_RCR_API void Scene::addComponent<TestComponent>(Entity entity, const TestComponent& component);
template<>
OTTO_RCR_API void Scene::removeComponent<TestComponent>(Entity entity);
template<>
OTTO_RCR_API TestComponent& Scene::getComponent<TestComponent>(Entity entity);
template<>
OTTO_RCR_API bool Scene::hasComponent<TestComponent>(Entity entity);
template<>
OTTO_RCR_API void Scene::addComponent<TestComponent2>(Entity entity, const TestComponent2& component);
template<>
OTTO_RCR_API void Scene::removeComponent<TestComponent2>(Entity entity);
template<>
OTTO_RCR_API TestComponent2& Scene::getComponent<TestComponent2>(Entity entity);
template<>
OTTO_RCR_API bool Scene::hasComponent<TestComponent2>(Entity entity);

template<>
OTTO_RCR_API void Scene::addEventListener<TestEvent>(const EventListener<TestEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<TestEvent>(const EventListener<TestEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<TestEvent>(const TestEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<TestEvent2>(const EventListener<TestEvent2>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<TestEvent2>(const EventListener<TestEvent2>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<TestEvent2>(const TestEvent2& e);
template<>
OTTO_RCR_API void Scene::addEventListener<KeyPressedEvent>(const EventListener<KeyPressedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<KeyPressedEvent>(const EventListener<KeyPressedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<KeyPressedEvent>(const KeyPressedEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<KeyReleasedEvent>(const EventListener<KeyReleasedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<KeyReleasedEvent>(const EventListener<KeyReleasedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<KeyReleasedEvent>(const KeyReleasedEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<MouseButtonPressedEvent>(const EventListener<MouseButtonPressedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<MouseButtonPressedEvent>(const EventListener<MouseButtonPressedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<MouseButtonPressedEvent>(const MouseButtonPressedEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<MouseButtonReleasedEvent>(const EventListener<MouseButtonReleasedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<MouseButtonReleasedEvent>(const EventListener<MouseButtonReleasedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<MouseButtonReleasedEvent>(const MouseButtonReleasedEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<MouseMovedEvent>(const EventListener<MouseMovedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<MouseMovedEvent>(const EventListener<MouseMovedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<MouseMovedEvent>(const MouseMovedEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<MouseDraggedEvent>(const EventListener<MouseDraggedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<MouseDraggedEvent>(const EventListener<MouseDraggedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<MouseDraggedEvent>(const MouseDraggedEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<MouseScrolledEvent>(const EventListener<MouseScrolledEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<MouseScrolledEvent>(const EventListener<MouseScrolledEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<MouseScrolledEvent>(const MouseScrolledEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<WindowClosedEvent>(const EventListener<WindowClosedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<WindowClosedEvent>(const EventListener<WindowClosedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<WindowClosedEvent>(const WindowClosedEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<WindowResizedEvent>(const EventListener<WindowResizedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<WindowResizedEvent>(const EventListener<WindowResizedEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<WindowResizedEvent>(const WindowResizedEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<WindowGainedFocusEvent>(const EventListener<WindowGainedFocusEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<WindowGainedFocusEvent>(const EventListener<WindowGainedFocusEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<WindowGainedFocusEvent>(const WindowGainedFocusEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<WindowLostFocusEvent>(const EventListener<WindowLostFocusEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<WindowLostFocusEvent>(const EventListener<WindowLostFocusEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<WindowLostFocusEvent>(const WindowLostFocusEvent& e);

#include "behaviours\TestBehaviour.hpp"
#include "behaviours\TestBehaviour2.hpp"
#include "systems\TestSystem.hpp"
#include "systems\TestSystem2.hpp"

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
        EventDispatcher<KeyPressedEvent> keyPressedEventDispatcher;
        EventDispatcher<KeyReleasedEvent> keyReleasedEventDispatcher;
        EventDispatcher<MouseButtonPressedEvent> mouseButtonPressedEventDispatcher;
        EventDispatcher<MouseButtonReleasedEvent> mouseButtonReleasedEventDispatcher;
        EventDispatcher<MouseMovedEvent> mouseMovedEventDispatcher;
        EventDispatcher<MouseDraggedEvent> mouseDraggedEventDispatcher;
        EventDispatcher<MouseScrolledEvent> mouseScrolledEventDispatcher;
        EventDispatcher<WindowClosedEvent> windowClosedEventDispatcher;
        EventDispatcher<WindowResizedEvent> windowResizedEventDispatcher;
        EventDispatcher<WindowGainedFocusEvent> windowGainedFocusEventDispatcher;
        EventDispatcher<WindowLostFocusEvent> windowLostFocusEventDispatcher;

        TestSystem testSystem;
        TestSystem2 testSystem2;

        View<TestBehaviour> testBehaviourView = View<TestBehaviour>(&testBehaviourPool);
        View<TestBehaviour2> testBehaviour2View = View<TestBehaviour2>(&testBehaviour2Pool);
        View<TestComponent> testComponentView = View<TestComponent>(&testComponentPool);
        View<TransformComponent> transformComponentView = View<TransformComponent>(&transformComponentPool);

        MultiView<TestComponent, TransformComponent> testComponent_transformComponentView = MultiView<TestComponent, TransformComponent>(&testComponentPool, &transformComponentPool);
    };

    template<typename C>
    Serialized serializeComponentOrBehaviour(const C& component, const Map<String, Entity>& entities)
    {
        return C();
    }

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

    OTTO_RCR_API Entity Scene::addEntity()
    {
        return mData->nextEntity++;
    }

    OTTO_RCR_API void Scene::init()
    {
        mData->testSystem.mScene = this;
        mData->testSystem.onInit(&mData->testComponent_transformComponentView, &mData->testComponentView, &mData->transformComponentView);
        mData->testSystem2.mScene = this;
        mData->testSystem2.onInit();
        for (auto [entity, behaviour] : mData->testBehaviourView)
        {
            behaviour.mScene = this;
            behaviour.mEntity = entity;
            behaviour.onInit();
        }
        for (auto [entity, behaviour] : mData->testBehaviour2View)
        {
            behaviour.mScene = this;
            behaviour.mEntity = entity;
            behaviour.onInit();
        }
    }

    OTTO_RCR_API void Scene::update(float32 delta32, float64 delta64)
    {
        mData->testSystem.onUpdate(delta32);
        mData->testSystem2.onUpdate(delta32);
        for (auto [entity, behaviour] : mData->testBehaviourView)
            behaviour.onUpdate(delta32);
        for (auto [entity, behaviour] : mData->testBehaviour2View)
            behaviour.onUpdate(delta32);
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

    template<>
    OTTO_RCR_API void Scene::addEventListener<KeyPressedEvent>(const EventListener<KeyPressedEvent>& eventListener)
    {
        mData->keyPressedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<KeyReleasedEvent>(const EventListener<KeyReleasedEvent>& eventListener)
    {
        mData->keyReleasedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<MouseButtonPressedEvent>(const EventListener<MouseButtonPressedEvent>& eventListener)
    {
        mData->mouseButtonPressedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<MouseButtonReleasedEvent>(const EventListener<MouseButtonReleasedEvent>& eventListener)
    {
        mData->mouseButtonReleasedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<MouseMovedEvent>(const EventListener<MouseMovedEvent>& eventListener)
    {
        mData->mouseMovedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<MouseDraggedEvent>(const EventListener<MouseDraggedEvent>& eventListener)
    {
        mData->mouseDraggedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<MouseScrolledEvent>(const EventListener<MouseScrolledEvent>& eventListener)
    {
        mData->mouseScrolledEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<WindowClosedEvent>(const EventListener<WindowClosedEvent>& eventListener)
    {
        mData->windowClosedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<WindowResizedEvent>(const EventListener<WindowResizedEvent>& eventListener)
    {
        mData->windowResizedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<WindowGainedFocusEvent>(const EventListener<WindowGainedFocusEvent>& eventListener)
    {
        mData->windowGainedFocusEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<WindowLostFocusEvent>(const EventListener<WindowLostFocusEvent>& eventListener)
    {
        mData->windowLostFocusEventDispatcher.addListener(eventListener);
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

    template<>
    OTTO_RCR_API void Scene::removeEventListener<KeyPressedEvent>(const EventListener<KeyPressedEvent>& eventListener)
    {
        mData->keyPressedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<KeyReleasedEvent>(const EventListener<KeyReleasedEvent>& eventListener)
    {
        mData->keyReleasedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<MouseButtonPressedEvent>(const EventListener<MouseButtonPressedEvent>& eventListener)
    {
        mData->mouseButtonPressedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<MouseButtonReleasedEvent>(const EventListener<MouseButtonReleasedEvent>& eventListener)
    {
        mData->mouseButtonReleasedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<MouseMovedEvent>(const EventListener<MouseMovedEvent>& eventListener)
    {
        mData->mouseMovedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<MouseDraggedEvent>(const EventListener<MouseDraggedEvent>& eventListener)
    {
        mData->mouseDraggedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<MouseScrolledEvent>(const EventListener<MouseScrolledEvent>& eventListener)
    {
        mData->mouseScrolledEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<WindowClosedEvent>(const EventListener<WindowClosedEvent>& eventListener)
    {
        mData->windowClosedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<WindowResizedEvent>(const EventListener<WindowResizedEvent>& eventListener)
    {
        mData->windowResizedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<WindowGainedFocusEvent>(const EventListener<WindowGainedFocusEvent>& eventListener)
    {
        mData->windowGainedFocusEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<WindowLostFocusEvent>(const EventListener<WindowLostFocusEvent>& eventListener)
    {
        mData->windowLostFocusEventDispatcher.removeListener(eventListener);
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
        mData->testEvent2Dispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<KeyPressedEvent>(const KeyPressedEvent& e)
    {
        mData->keyPressedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<KeyReleasedEvent>(const KeyReleasedEvent& e)
    {
        mData->keyReleasedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<MouseButtonPressedEvent>(const MouseButtonPressedEvent& e)
    {
        mData->mouseButtonPressedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<MouseButtonReleasedEvent>(const MouseButtonReleasedEvent& e)
    {
        mData->mouseButtonReleasedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<MouseMovedEvent>(const MouseMovedEvent& e)
    {
        mData->testSystem.onEvent(e);
        mData->mouseMovedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<MouseDraggedEvent>(const MouseDraggedEvent& e)
    {
        mData->mouseDraggedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<MouseScrolledEvent>(const MouseScrolledEvent& e)
    {
        mData->mouseScrolledEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<WindowClosedEvent>(const WindowClosedEvent& e)
    {
        mData->windowClosedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<WindowResizedEvent>(const WindowResizedEvent& e)
    {
        mData->windowResizedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<WindowGainedFocusEvent>(const WindowGainedFocusEvent& e)
    {
        mData->windowGainedFocusEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<WindowLostFocusEvent>(const WindowLostFocusEvent& e)
    {
        mData->windowLostFocusEventDispatcher.dispatchEvent(e);
    }

    OTTO_RCR_API void Scene::addComponent(Entity entity, const String& componentName, const Serialized& args, const EntityMap& entities)
    {
        if (componentName == "TransformComponent")
            addComponent(entity, deserializeComponentOrBehaviour<TransformComponent>(args, entities));
        if (componentName == "TestComponent")
            addComponent(entity, deserializeComponentOrBehaviour<TestComponent>(args, entities));
        if (componentName == "TestComponent2")
            addComponent(entity, deserializeComponentOrBehaviour<TestComponent2>(args, entities));
        if (componentName == "TestBehaviour")
            mData->testBehaviourPool.addComponent(entity, deserializeComponentOrBehaviour<TestBehaviour>(args, entities));
        if (componentName == "TestBehaviour2")
            mData->testBehaviour2Pool.addComponent(entity, deserializeComponentOrBehaviour<TestBehaviour2>(args, entities));
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
        mData->testComponent_transformComponentView.onComponent2Added(entity);
    }

    template<>
    OTTO_RCR_API void Scene::addComponent<TestComponent>(Entity entity, const TestComponent& component)
    {
        mData->testComponentPool.addComponent(entity, component);
        mData->testComponent_transformComponentView.onComponent1Added(entity);
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
