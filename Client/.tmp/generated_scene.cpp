#define OTTO_GENERATED_CODE
#include "otto/base.h"
#include "otto/scene/scene.h"
#include "otto/event/event_dispatcher.h"
#include "otto/events/ComponentAddedEvent.hpp"
#include "otto/events/ComponentRemovedEvent.hpp"

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
#include "otto\events\InitEvent.hpp"
#include "otto\events\UpdateEvent.hpp"
#include "otto\events\RebufferEvent.hpp"
#include "otto\events\RenderEvent.hpp"

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
template<>
OTTO_RCR_API void Scene::addEventListener<InitEvent>(const EventListener<InitEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<InitEvent>(const EventListener<InitEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<InitEvent>(const InitEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<UpdateEvent>(const EventListener<UpdateEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<UpdateEvent>(const EventListener<UpdateEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<UpdateEvent>(const UpdateEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<RebufferEvent>(const EventListener<RebufferEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<RebufferEvent>(const EventListener<RebufferEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<RebufferEvent>(const RebufferEvent& e);
template<>
OTTO_RCR_API void Scene::addEventListener<RenderEvent>(const EventListener<RenderEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::removeEventListener<RenderEvent>(const EventListener<RenderEvent>& eventListener);
template<>
OTTO_RCR_API void Scene::dispatchEvent<RenderEvent>(const RenderEvent& e);

#include "behaviours\TestBehaviour.hpp"
#include "behaviours\TestBehaviour2.hpp"
#include "systems\TestSystem.hpp"
#include "systems\TestSystem2.hpp"

namespace otto
{
    struct SceneData
    {
        Entity nextEntity = 1;

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
        EventDispatcher<InitEvent> initEventDispatcher;
        EventDispatcher<UpdateEvent> updateEventDispatcher;
        EventDispatcher<RebufferEvent> rebufferEventDispatcher;
        EventDispatcher<RenderEvent> renderEventDispatcher;

        View<TestBehaviour> testBehaviourView = View<TestBehaviour>(&testBehaviourPool); 
        View<TestBehaviour2> testBehaviour2View = View<TestBehaviour2>(&testBehaviour2Pool); 
        View<TestComponent> testComponentView = View<TestComponent>(&testComponentPool); 
        View<TransformComponent> transformComponentView = View<TransformComponent>(&transformComponentPool); 

        MultiView<TestComponent, TransformComponent> testComponent_transformComponentView = MultiView<TestComponent, TransformComponent>(&testComponentPool, &transformComponentPool);
        TestSystem testSystem = TestSystem(&testComponent_transformComponentView, &testComponentView, &transformComponentView);
        TestSystem2 testSystem2 = TestSystem2();
        Map<Entity, DynamicArray<uint16>> entityComponentMap;    };

    template<typename C>
    static uint16 getID()
    {
        static_assert(false);
    }
    template<>
    static uint16 getID<TransformComponent>()
    {
        return 0;
    }

    template<>
    static uint16 getID<TestComponent>()
    {
        return 1;
    }

    template<>
    static uint16 getID<TestComponent2>()
    {
        return 2;
    }

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
        Entity entity = mData->nextEntity++;
        mData->entityComponentMap.insert(entity, DynamicArray<uint16>());
        return entity;
    }

    OTTO_RCR_API void Scene::removeEntity(Entity entity)
    {
        for (uint16 componentID : mData->entityComponentMap[entity])
        {
            switch(componentID)
            {
            case 0: removeComponent<TransformComponent>(entity); break;
            case 1: removeComponent<TestComponent>(entity); break;
            case 2: removeComponent<TestComponent2>(entity); break;
            }
        }

        mData->entityComponentMap.remove(entity);
    }

    OTTO_RCR_API void Scene::init()
    {
        mData->testSystem.mScene = this;
        mData->testSystem2.mScene = this;
        for (auto [entity, behaviour] : mData->testBehaviourView)
        {
            behaviour.mScene = this;
            behaviour.mEntity = entity;
        }
        for (auto [entity, behaviour] : mData->testBehaviour2View)
        {
            behaviour.mScene = this;
            behaviour.mEntity = entity;
        }
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

    template<>
    OTTO_RCR_API void Scene::addEventListener<InitEvent>(const EventListener<InitEvent>& eventListener)
    {
        mData->initEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<UpdateEvent>(const EventListener<UpdateEvent>& eventListener)
    {
        mData->updateEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<RebufferEvent>(const EventListener<RebufferEvent>& eventListener)
    {
        mData->rebufferEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::addEventListener<RenderEvent>(const EventListener<RenderEvent>& eventListener)
    {
        mData->renderEventDispatcher.addListener(eventListener);
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

    template<>
    OTTO_RCR_API void Scene::removeEventListener<InitEvent>(const EventListener<InitEvent>& eventListener)
    {
        mData->initEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<UpdateEvent>(const EventListener<UpdateEvent>& eventListener)
    {
        mData->updateEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<RebufferEvent>(const EventListener<RebufferEvent>& eventListener)
    {
        mData->rebufferEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_RCR_API void Scene::removeEventListener<RenderEvent>(const EventListener<RenderEvent>& eventListener)
    {
        mData->renderEventDispatcher.removeListener(eventListener);
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

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<InitEvent>(const InitEvent& e)
    {
        mData->testSystem.onEvent(e);
        mData->testSystem2.onEvent(e);
        for (auto [entity, behaviour] : mData->testBehaviourView)
            behaviour.onEvent(e);
        for (auto [entity, behaviour] : mData->testBehaviour2View)
            behaviour.onEvent(e);
        mData->initEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<UpdateEvent>(const UpdateEvent& e)
    {
        mData->testSystem.onEvent(e);
        mData->testSystem2.onEvent(e);
        for (auto [entity, behaviour] : mData->testBehaviourView)
            behaviour.onEvent(e);
        mData->updateEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<RebufferEvent>(const RebufferEvent& e)
    {
        mData->rebufferEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_RCR_API void Scene::dispatchEvent<RenderEvent>(const RenderEvent& e)
    {
        mData->renderEventDispatcher.dispatchEvent(e);
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
        mData->entityComponentMap[entity].add(getID<TransformComponent>());
    }

    template<>
    OTTO_RCR_API void Scene::addComponent<TestComponent>(Entity entity, const TestComponent& component)
    {
        mData->testComponentPool.addComponent(entity, component);
        mData->testComponent_transformComponentView.onComponent1Added(entity);
        mData->testSystem.onEvent(ComponentAddedEvent<TestComponent>(entity));
        mData->entityComponentMap[entity].add(getID<TestComponent>());
    }

    template<>
    OTTO_RCR_API void Scene::addComponent<TestComponent2>(Entity entity, const TestComponent2& component)
    {
        mData->testComponent2Pool.addComponent(entity, component);
        mData->entityComponentMap[entity].add(getID<TestComponent2>());
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
        mData->testComponent_transformComponentView.onComponent2Removed(entity);
        mData->entityComponentMap[entity].remove(mData->entityComponentMap[entity].indexOf(getID<TransformComponent>()));
    }

    template<>
    OTTO_RCR_API void Scene::removeComponent<TestComponent>(Entity entity)
    {
        mData->testComponentPool.removeComponent(entity);
        mData->testComponent_transformComponentView.onComponent1Removed(entity);
        mData->entityComponentMap[entity].remove(mData->entityComponentMap[entity].indexOf(getID<TestComponent>()));
    }

    template<>
    OTTO_RCR_API void Scene::removeComponent<TestComponent2>(Entity entity)
    {
        mData->testComponent2Pool.removeComponent(entity);
        mData->testSystem.onEvent(ComponentRemovedEvent<TestComponent2>(entity));
        mData->entityComponentMap[entity].remove(mData->entityComponentMap[entity].indexOf(getID<TestComponent2>()));
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
        return mData->entityComponentMap[entity].contains(getID<TransformComponent>());
    }

    template<>
    OTTO_RCR_API bool Scene::hasComponent<TestComponent>(Entity entity)
    {
        return mData->entityComponentMap[entity].contains(getID<TestComponent>());
    }

    template<>
    OTTO_RCR_API bool Scene::hasComponent<TestComponent2>(Entity entity)
    {
        return mData->entityComponentMap[entity].contains(getID<TestComponent2>());
    }

} // namespace otto
