#define OTTO_GENERATED_CODE
#include "otto.h"
#include "packages/otto/core/events/ComponentAddedEvent.hpp"
#include "packages/otto/core/events/ComponentRemovedEvent.hpp"

using namespace otto;

#include "C:\dev\Otto\Client\components\TestComponent.hpp"
#include "C:\dev\Otto\Client\components\TestComponent2.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\ui\components\UIComponent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\2D\components\Transform2DComponent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\2D\components\Renderer2DComponent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\2D\components\Camera2DComponent.hpp"
#include "C:\dev\Otto\Client\events\TestEvent.hpp"
#include "C:\dev\Otto\Client\events\TestEvent2.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\KeyPressedEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\KeyReleasedEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\MouseButtonPressedEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\MouseButtonReleasedEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\MouseMovedEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\MouseDraggedEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\MouseScrolledEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\WindowClosedEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\WindowResizedEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\WindowGainedFocusEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\window\events\WindowLostFocusEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\core\events\InitEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\core\events\UpdateEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\core\events\RenderEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\core\events\RebufferEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\ui\events\UIRendererRebufferEvent.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\ui\events\UIClickedEvent.hpp"

template<>
OTTO_DLL_FUNC void Scene::addComponent<TestComponent>(Entity entity, const TestComponent& component);
template<>
OTTO_DLL_FUNC void Scene::removeComponent<TestComponent>(Entity entity);
template<>
OTTO_DLL_FUNC TestComponent& Scene::getComponent<TestComponent>(Entity entity);
template<>
OTTO_DLL_FUNC bool8 Scene::hasComponent<TestComponent>(Entity entity);
template<>
OTTO_DLL_FUNC void Scene::addComponent<TestComponent2>(Entity entity, const TestComponent2& component);
template<>
OTTO_DLL_FUNC void Scene::removeComponent<TestComponent2>(Entity entity);
template<>
OTTO_DLL_FUNC TestComponent2& Scene::getComponent<TestComponent2>(Entity entity);
template<>
OTTO_DLL_FUNC bool8 Scene::hasComponent<TestComponent2>(Entity entity);
template<>
OTTO_DLL_FUNC void Scene::addComponent<UIComponent>(Entity entity, const UIComponent& component);
template<>
OTTO_DLL_FUNC void Scene::removeComponent<UIComponent>(Entity entity);
template<>
OTTO_DLL_FUNC UIComponent& Scene::getComponent<UIComponent>(Entity entity);
template<>
OTTO_DLL_FUNC bool8 Scene::hasComponent<UIComponent>(Entity entity);
template<>
OTTO_DLL_FUNC void Scene::addComponent<Transform2DComponent>(Entity entity, const Transform2DComponent& component);
template<>
OTTO_DLL_FUNC void Scene::removeComponent<Transform2DComponent>(Entity entity);
template<>
OTTO_DLL_FUNC Transform2DComponent& Scene::getComponent<Transform2DComponent>(Entity entity);
template<>
OTTO_DLL_FUNC bool8 Scene::hasComponent<Transform2DComponent>(Entity entity);
template<>
OTTO_DLL_FUNC void Scene::addComponent<Renderer2DComponent>(Entity entity, const Renderer2DComponent& component);
template<>
OTTO_DLL_FUNC void Scene::removeComponent<Renderer2DComponent>(Entity entity);
template<>
OTTO_DLL_FUNC Renderer2DComponent& Scene::getComponent<Renderer2DComponent>(Entity entity);
template<>
OTTO_DLL_FUNC bool8 Scene::hasComponent<Renderer2DComponent>(Entity entity);
template<>
OTTO_DLL_FUNC void Scene::addComponent<Camera2DComponent>(Entity entity, const Camera2DComponent& component);
template<>
OTTO_DLL_FUNC void Scene::removeComponent<Camera2DComponent>(Entity entity);
template<>
OTTO_DLL_FUNC Camera2DComponent& Scene::getComponent<Camera2DComponent>(Entity entity);
template<>
OTTO_DLL_FUNC bool8 Scene::hasComponent<Camera2DComponent>(Entity entity);

template<>
OTTO_DLL_FUNC void Scene::addEventListener<TestEvent>(const EventListener<TestEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<TestEvent>(const EventListener<TestEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<TestEvent>(const TestEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<TestEvent2>(const EventListener<TestEvent2>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<TestEvent2>(const EventListener<TestEvent2>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<TestEvent2>(const TestEvent2& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<KeyPressedEvent>(const EventListener<KeyPressedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<KeyPressedEvent>(const EventListener<KeyPressedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<KeyPressedEvent>(const KeyPressedEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<KeyReleasedEvent>(const EventListener<KeyReleasedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<KeyReleasedEvent>(const EventListener<KeyReleasedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<KeyReleasedEvent>(const KeyReleasedEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<MouseButtonPressedEvent>(const EventListener<MouseButtonPressedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<MouseButtonPressedEvent>(const EventListener<MouseButtonPressedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<MouseButtonPressedEvent>(const MouseButtonPressedEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<MouseButtonReleasedEvent>(const EventListener<MouseButtonReleasedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<MouseButtonReleasedEvent>(const EventListener<MouseButtonReleasedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<MouseButtonReleasedEvent>(const MouseButtonReleasedEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<MouseMovedEvent>(const EventListener<MouseMovedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<MouseMovedEvent>(const EventListener<MouseMovedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<MouseMovedEvent>(const MouseMovedEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<MouseDraggedEvent>(const EventListener<MouseDraggedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<MouseDraggedEvent>(const EventListener<MouseDraggedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<MouseDraggedEvent>(const MouseDraggedEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<MouseScrolledEvent>(const EventListener<MouseScrolledEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<MouseScrolledEvent>(const EventListener<MouseScrolledEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<MouseScrolledEvent>(const MouseScrolledEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<WindowClosedEvent>(const EventListener<WindowClosedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<WindowClosedEvent>(const EventListener<WindowClosedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<WindowClosedEvent>(const WindowClosedEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<WindowResizedEvent>(const EventListener<WindowResizedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<WindowResizedEvent>(const EventListener<WindowResizedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<WindowResizedEvent>(const WindowResizedEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<WindowGainedFocusEvent>(const EventListener<WindowGainedFocusEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<WindowGainedFocusEvent>(const EventListener<WindowGainedFocusEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<WindowGainedFocusEvent>(const WindowGainedFocusEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<WindowLostFocusEvent>(const EventListener<WindowLostFocusEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<WindowLostFocusEvent>(const EventListener<WindowLostFocusEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<WindowLostFocusEvent>(const WindowLostFocusEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<InitEvent>(const EventListener<InitEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<InitEvent>(const EventListener<InitEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<InitEvent>(const InitEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<UpdateEvent>(const EventListener<UpdateEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<UpdateEvent>(const EventListener<UpdateEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<UpdateEvent>(const UpdateEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<RenderEvent>(const EventListener<RenderEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<RenderEvent>(const EventListener<RenderEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<RenderEvent>(const RenderEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<RebufferEvent>(const EventListener<RebufferEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<RebufferEvent>(const EventListener<RebufferEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<RebufferEvent>(const RebufferEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<UIRendererRebufferEvent>(const EventListener<UIRendererRebufferEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<UIRendererRebufferEvent>(const EventListener<UIRendererRebufferEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<UIRendererRebufferEvent>(const UIRendererRebufferEvent& e);
template<>
OTTO_DLL_FUNC void Scene::addEventListener<UIClickedEvent>(const EventListener<UIClickedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::removeEventListener<UIClickedEvent>(const EventListener<UIClickedEvent>& eventListener);
template<>
OTTO_DLL_FUNC void Scene::dispatchEvent<UIClickedEvent>(const UIClickedEvent& e);

#include "C:\dev\Otto\Client\behaviours\TestBehaviour.hpp"
#include "C:\dev\Otto\Client\behaviours\TestBehaviour2.hpp"
#include "C:\dev\Otto\Client\systems\TestSystem.hpp"
#include "C:\dev\Otto\Client\systems\TestSystem2.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\ui\systems\UIRenderer.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\ui\systems\UIHandler.hpp"
#include "C:\dev\Otto\Otto\src\packages\otto\2D\systems\Renderer2D.hpp"

namespace otto
{
    struct SceneData
    {
        Entity nextEntity = 1;

        ComponentPool<TestComponent> testComponentPool;
        ComponentPool<TestComponent2> testComponent2Pool;
        ComponentPool<UIComponent> uIComponentPool;
        ComponentPool<Transform2DComponent> transform2DComponentPool;
        ComponentPool<Renderer2DComponent> renderer2DComponentPool;
        ComponentPool<Camera2DComponent> camera2DComponentPool;

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
        EventDispatcher<RenderEvent> renderEventDispatcher;
        EventDispatcher<RebufferEvent> rebufferEventDispatcher;
        EventDispatcher<UIRendererRebufferEvent> uIRendererRebufferEventDispatcher;
        EventDispatcher<UIClickedEvent> uIClickedEventDispatcher;

        View<TestBehaviour> testBehaviourView = View<TestBehaviour>(&testBehaviourPool); 
        View<TestBehaviour2> testBehaviour2View = View<TestBehaviour2>(&testBehaviour2Pool); 
        View<TestComponent> testComponentView = View<TestComponent>(&testComponentPool); 
        View<Transform2DComponent> transform2DComponentView = View<Transform2DComponent>(&transform2DComponentPool); 
        View<UIComponent> uIComponentView = View<UIComponent>(&uIComponentPool); 
        View<Camera2DComponent> camera2DComponentView = View<Camera2DComponent>(&camera2DComponentPool); 

        MultiView<Renderer2DComponent, Transform2DComponent> renderer2DComponent_transform2DComponentView = MultiView<Renderer2DComponent, Transform2DComponent>(&renderer2DComponentPool, &transform2DComponentPool);
        MultiView<Transform2DComponent, Renderer2DComponent> transform2DComponent_renderer2DComponentView = MultiView<Transform2DComponent, Renderer2DComponent>(&transform2DComponentPool, &renderer2DComponentPool);
        TestSystem testSystem = TestSystem(&renderer2DComponent_transform2DComponentView, &testComponentView, &transform2DComponentView);
        TestSystem2 testSystem2 = TestSystem2();
        UIRenderer uIRenderer = UIRenderer(&uIComponentView);
        UIHandler uIHandler = UIHandler(&uIComponentView);
        Renderer2D renderer2D = Renderer2D(&transform2DComponent_renderer2DComponentView, &camera2DComponentView);
        Map<Entity, DynamicArray<uint16>> entityComponentMap;    };

    template<typename C>
    static uint16 getID()
    {
        static_assert(false);
    }
    template<>
    static uint16 getID<TestComponent>()
    {
        return 0;
    }

    template<>
    static uint16 getID<TestComponent2>()
    {
        return 1;
    }

    template<>
    static uint16 getID<UIComponent>()
    {
        return 2;
    }

    template<>
    static uint16 getID<Transform2DComponent>()
    {
        return 3;
    }

    template<>
    static uint16 getID<Renderer2DComponent>()
    {
        return 4;
    }

    template<>
    static uint16 getID<Camera2DComponent>()
    {
        return 5;
    }

    template<typename C>
    Serialized serializeComponentOrBehaviour(const C& component, const Map<String, Entity>& entities)
    {
        return C();
    }

    template<>
    Serialized serializeComponentOrBehaviour<Renderer2DComponent>(const Renderer2DComponent& component, const Map<String, Entity>& entities)
    {
        return serializeComponent<Renderer2DComponent>(component, entities);
    }

    template<typename C>
    C deserializeComponentOrBehaviour(const Serialized& args, const Map<String, Entity>& entities)
    {
        return C();
    }

    template<>
    TestComponent deserializeComponentOrBehaviour<TestComponent>(const Serialized& args, const Map<String, Entity>& entities)
    {
        return deserializeComponent<TestComponent>(args, entities);
    }

    template<>
    UIComponent deserializeComponentOrBehaviour<UIComponent>(const Serialized& args, const Map<String, Entity>& entities)
    {
        return deserializeComponent<UIComponent>(args, entities);
    }

    template<>
    Transform2DComponent deserializeComponentOrBehaviour<Transform2DComponent>(const Serialized& args, const Map<String, Entity>& entities)
    {
        return deserializeComponent<Transform2DComponent>(args, entities);
    }

    template<>
    Renderer2DComponent deserializeComponentOrBehaviour<Renderer2DComponent>(const Serialized& args, const Map<String, Entity>& entities)
    {
        return deserializeComponent<Renderer2DComponent>(args, entities);
    }

    template<>
    Camera2DComponent deserializeComponentOrBehaviour<Camera2DComponent>(const Serialized& args, const Map<String, Entity>& entities)
    {
        return deserializeComponent<Camera2DComponent>(args, entities);
    }

    OTTO_DLL_FUNC Shared<Scene> Scene::_createScene()
    {
        return new Scene(new SceneData());
    }

    OTTO_DLL_FUNC void Scene::_initClient(Application* mainApplication, Window* mainWindow, Log* mainLog, const Color& clearColor)
    {
        Application::init(mainApplication, mainWindow, mainLog);
        Window::setClearColor(clearColor);
    }

    OTTO_DLL_FUNC Entity Scene::addEntity()
    {
        Entity entity = mData->nextEntity++;
        mData->entityComponentMap.insert(entity, DynamicArray<uint16>());
        return entity;
    }

    OTTO_DLL_FUNC void Scene::removeEntity(Entity entity)
    {
        for (uint16 componentID : mData->entityComponentMap[entity])
        {
            switch(componentID)
            {
            case 0: removeComponent<TestComponent>(entity); break;
            case 1: removeComponent<TestComponent2>(entity); break;
            case 2: removeComponent<UIComponent>(entity); break;
            case 3: removeComponent<Transform2DComponent>(entity); break;
            case 4: removeComponent<Renderer2DComponent>(entity); break;
            case 5: removeComponent<Camera2DComponent>(entity); break;
            }
        }

        mData->entityComponentMap.remove(entity);
    }

    OTTO_DLL_FUNC void Scene::init()
    {
        mData->testSystem.mScene = this;
        mData->testSystem2.mScene = this;
        mData->uIRenderer.mScene = this;
        mData->uIHandler.mScene = this;
        mData->renderer2D.mScene = this;
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

        dispatchEvent(_InitEvent());
    }

    OTTO_DLL_FUNC void Scene::update(float32 delta)
    {
        dispatchEvent(UpdateEvent(delta));
    }

    OTTO_DLL_FUNC void Scene::rebuffer()
    {
        GraphicsAPI::setViewport(0, 0, uint32(Window::getWidth()), uint32(Window::getHeight()));

        dispatchEvent(RebufferEvent());
    }

    OTTO_DLL_FUNC void Scene::render()
    {
        Window::clear();
        dispatchEvent(RenderEvent());
        Window::swapBuffers();
    }

    template<typename E>
    OTTO_DLL_FUNC void Scene::addEventListener(const EventListener<E>& eventListener)
    {
        OTTO_ASSERT(false, "Event is not added.")
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<TestEvent>(const EventListener<TestEvent>& eventListener)
    {
        mData->testEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<TestEvent2>(const EventListener<TestEvent2>& eventListener)
    {
        mData->testEvent2Dispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<KeyPressedEvent>(const EventListener<KeyPressedEvent>& eventListener)
    {
        mData->keyPressedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<KeyReleasedEvent>(const EventListener<KeyReleasedEvent>& eventListener)
    {
        mData->keyReleasedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<MouseButtonPressedEvent>(const EventListener<MouseButtonPressedEvent>& eventListener)
    {
        mData->mouseButtonPressedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<MouseButtonReleasedEvent>(const EventListener<MouseButtonReleasedEvent>& eventListener)
    {
        mData->mouseButtonReleasedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<MouseMovedEvent>(const EventListener<MouseMovedEvent>& eventListener)
    {
        mData->mouseMovedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<MouseDraggedEvent>(const EventListener<MouseDraggedEvent>& eventListener)
    {
        mData->mouseDraggedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<MouseScrolledEvent>(const EventListener<MouseScrolledEvent>& eventListener)
    {
        mData->mouseScrolledEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<WindowClosedEvent>(const EventListener<WindowClosedEvent>& eventListener)
    {
        mData->windowClosedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<WindowResizedEvent>(const EventListener<WindowResizedEvent>& eventListener)
    {
        mData->windowResizedEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<WindowGainedFocusEvent>(const EventListener<WindowGainedFocusEvent>& eventListener)
    {
        mData->windowGainedFocusEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<WindowLostFocusEvent>(const EventListener<WindowLostFocusEvent>& eventListener)
    {
        mData->windowLostFocusEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<InitEvent>(const EventListener<InitEvent>& eventListener)
    {
        mData->initEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<UpdateEvent>(const EventListener<UpdateEvent>& eventListener)
    {
        mData->updateEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<RenderEvent>(const EventListener<RenderEvent>& eventListener)
    {
        mData->renderEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<RebufferEvent>(const EventListener<RebufferEvent>& eventListener)
    {
        mData->rebufferEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<UIRendererRebufferEvent>(const EventListener<UIRendererRebufferEvent>& eventListener)
    {
        mData->uIRendererRebufferEventDispatcher.addListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addEventListener<UIClickedEvent>(const EventListener<UIClickedEvent>& eventListener)
    {
        mData->uIClickedEventDispatcher.addListener(eventListener);
    }

    template<typename E>
    OTTO_DLL_FUNC void Scene::removeEventListener(const EventListener<E>& eventListener)
    {
        OTTO_ASSERT(false, "Event is not added.")
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<TestEvent>(const EventListener<TestEvent>& eventListener)
    {
        mData->testEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<TestEvent2>(const EventListener<TestEvent2>& eventListener)
    {
        mData->testEvent2Dispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<KeyPressedEvent>(const EventListener<KeyPressedEvent>& eventListener)
    {
        mData->keyPressedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<KeyReleasedEvent>(const EventListener<KeyReleasedEvent>& eventListener)
    {
        mData->keyReleasedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<MouseButtonPressedEvent>(const EventListener<MouseButtonPressedEvent>& eventListener)
    {
        mData->mouseButtonPressedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<MouseButtonReleasedEvent>(const EventListener<MouseButtonReleasedEvent>& eventListener)
    {
        mData->mouseButtonReleasedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<MouseMovedEvent>(const EventListener<MouseMovedEvent>& eventListener)
    {
        mData->mouseMovedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<MouseDraggedEvent>(const EventListener<MouseDraggedEvent>& eventListener)
    {
        mData->mouseDraggedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<MouseScrolledEvent>(const EventListener<MouseScrolledEvent>& eventListener)
    {
        mData->mouseScrolledEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<WindowClosedEvent>(const EventListener<WindowClosedEvent>& eventListener)
    {
        mData->windowClosedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<WindowResizedEvent>(const EventListener<WindowResizedEvent>& eventListener)
    {
        mData->windowResizedEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<WindowGainedFocusEvent>(const EventListener<WindowGainedFocusEvent>& eventListener)
    {
        mData->windowGainedFocusEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<WindowLostFocusEvent>(const EventListener<WindowLostFocusEvent>& eventListener)
    {
        mData->windowLostFocusEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<InitEvent>(const EventListener<InitEvent>& eventListener)
    {
        mData->initEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<UpdateEvent>(const EventListener<UpdateEvent>& eventListener)
    {
        mData->updateEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<RenderEvent>(const EventListener<RenderEvent>& eventListener)
    {
        mData->renderEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<RebufferEvent>(const EventListener<RebufferEvent>& eventListener)
    {
        mData->rebufferEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<UIRendererRebufferEvent>(const EventListener<UIRendererRebufferEvent>& eventListener)
    {
        mData->uIRendererRebufferEventDispatcher.removeListener(eventListener);
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeEventListener<UIClickedEvent>(const EventListener<UIClickedEvent>& eventListener)
    {
        mData->uIClickedEventDispatcher.removeListener(eventListener);
    }

    template<typename E>
    OTTO_DLL_FUNC void Scene::dispatchEvent(const E& e)
    {
        OTTO_ASSERT(false, "Event is not added.")
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<TestEvent>(const TestEvent& e)
    {
        mData->testSystem.onEvent(e);
        for (auto [entity, behaviour] : mData->testBehaviourView)
            behaviour.onEvent(e);
        mData->testEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<TestEvent2>(const TestEvent2& e)
    {
        mData->testEvent2Dispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<KeyPressedEvent>(const KeyPressedEvent& e)
    {
        mData->keyPressedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<KeyReleasedEvent>(const KeyReleasedEvent& e)
    {
        mData->keyReleasedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<MouseButtonPressedEvent>(const MouseButtonPressedEvent& e)
    {
        mData->mouseButtonPressedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<MouseButtonReleasedEvent>(const MouseButtonReleasedEvent& e)
    {
        mData->mouseButtonReleasedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<MouseMovedEvent>(const MouseMovedEvent& e)
    {
        mData->testSystem.onEvent(e);
        mData->uIHandler.onEvent(e);
        mData->mouseMovedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<MouseDraggedEvent>(const MouseDraggedEvent& e)
    {
        mData->mouseDraggedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<MouseScrolledEvent>(const MouseScrolledEvent& e)
    {
        mData->mouseScrolledEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<WindowClosedEvent>(const WindowClosedEvent& e)
    {
        mData->windowClosedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<WindowResizedEvent>(const WindowResizedEvent& e)
    {
        mData->uIRenderer.onEvent(e);
        mData->renderer2D.onEvent(e);
        mData->windowResizedEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<WindowGainedFocusEvent>(const WindowGainedFocusEvent& e)
    {
        mData->windowGainedFocusEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<WindowLostFocusEvent>(const WindowLostFocusEvent& e)
    {
        mData->windowLostFocusEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<InitEvent>(const InitEvent& e)
    {
        mData->testSystem.onEvent(e);
        mData->testSystem2.onEvent(e);
        mData->uIRenderer.onEvent(e);
        mData->uIHandler.onEvent(e);
        mData->renderer2D.onEvent(e);
        for (auto [entity, behaviour] : mData->testBehaviourView)
            behaviour.onEvent(e);
        for (auto [entity, behaviour] : mData->testBehaviour2View)
            behaviour.onEvent(e);
        mData->initEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<UpdateEvent>(const UpdateEvent& e)
    {
        mData->testSystem.onEvent(e);
        mData->testSystem2.onEvent(e);
        mData->uIHandler.onEvent(e);
        for (auto [entity, behaviour] : mData->testBehaviourView)
            behaviour.onEvent(e);
        mData->updateEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<RenderEvent>(const RenderEvent& e)
    {
        mData->uIRenderer.onEvent(e);
        mData->renderer2D.onEvent(e);
        mData->renderEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<RebufferEvent>(const RebufferEvent& e)
    {
        mData->uIRenderer.onEvent(e);
        mData->renderer2D.onEvent(e);
        mData->rebufferEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<UIRendererRebufferEvent>(const UIRendererRebufferEvent& e)
    {
        mData->uIRenderer.onEvent(e);
        mData->uIRendererRebufferEventDispatcher.dispatchEvent(e);
    }

    template<>
    OTTO_DLL_FUNC void Scene::dispatchEvent<UIClickedEvent>(const UIClickedEvent& e)
    {
        mData->uIClickedEventDispatcher.dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::addComponent(Entity entity, const String& componentName, const Serialized& args, const Map<String, Entity>& entities)
    {
        if (componentName == "TestComponent")
        {
            mData->testComponentPool.addComponent(entity, deserializeComponentOrBehaviour<TestComponent>(args, entities));
            mData->entityComponentMap[entity].add(getID<TestComponent>());
        }
        if (componentName == "TestComponent2")
        {
            mData->testComponent2Pool.addComponent(entity, deserializeComponentOrBehaviour<TestComponent2>(args, entities));
            mData->entityComponentMap[entity].add(getID<TestComponent2>());
        }
        if (componentName == "UIComponent")
        {
            mData->uIComponentPool.addComponent(entity, deserializeComponentOrBehaviour<UIComponent>(args, entities));
            mData->entityComponentMap[entity].add(getID<UIComponent>());
        }
        if (componentName == "Transform2DComponent")
        {
            mData->transform2DComponentPool.addComponent(entity, deserializeComponentOrBehaviour<Transform2DComponent>(args, entities));
            mData->entityComponentMap[entity].add(getID<Transform2DComponent>());
            mData->renderer2DComponent_transform2DComponentView.onComponent2Added(entity);
            mData->transform2DComponent_renderer2DComponentView.onComponent1Added(entity);
        }
        if (componentName == "Renderer2DComponent")
        {
            mData->renderer2DComponentPool.addComponent(entity, deserializeComponentOrBehaviour<Renderer2DComponent>(args, entities));
            mData->entityComponentMap[entity].add(getID<Renderer2DComponent>());
            mData->renderer2DComponent_transform2DComponentView.onComponent1Added(entity);
            mData->transform2DComponent_renderer2DComponentView.onComponent2Added(entity);
        }
        if (componentName == "Camera2DComponent")
        {
            mData->camera2DComponentPool.addComponent(entity, deserializeComponentOrBehaviour<Camera2DComponent>(args, entities));
            mData->entityComponentMap[entity].add(getID<Camera2DComponent>());
        }
        if (componentName == "TestBehaviour")
            mData->testBehaviourPool.addComponent(entity, deserializeComponentOrBehaviour<TestBehaviour>(args, entities));
        if (componentName == "TestBehaviour2")
            mData->testBehaviour2Pool.addComponent(entity, deserializeComponentOrBehaviour<TestBehaviour2>(args, entities));
    }

    template<typename C>
    OTTO_DLL_FUNC void Scene::addComponent(Entity entity, const C& component)
    {
        OTTO_ASSERT(false, "Component is not added.")
    }

    template<>
    OTTO_DLL_FUNC void Scene::addComponent<TestComponent>(Entity entity, const TestComponent& component)
    {
        mData->testComponentPool.addComponent(entity, component);
        mData->entityComponentMap[entity].add(getID<TestComponent>());
        mData->testSystem.onEvent(ComponentAddedEvent<TestComponent>(entity));
    }

    template<>
    OTTO_DLL_FUNC void Scene::addComponent<TestComponent2>(Entity entity, const TestComponent2& component)
    {
        mData->testComponent2Pool.addComponent(entity, component);
        mData->entityComponentMap[entity].add(getID<TestComponent2>());
    }

    template<>
    OTTO_DLL_FUNC void Scene::addComponent<UIComponent>(Entity entity, const UIComponent& component)
    {
        mData->uIComponentPool.addComponent(entity, component);
        mData->entityComponentMap[entity].add(getID<UIComponent>());
        mData->uIRenderer.onEvent(ComponentAddedEvent<UIComponent>(entity));
        mData->uIHandler.onEvent(ComponentAddedEvent<UIComponent>(entity));
    }

    template<>
    OTTO_DLL_FUNC void Scene::addComponent<Transform2DComponent>(Entity entity, const Transform2DComponent& component)
    {
        mData->transform2DComponentPool.addComponent(entity, component);
        mData->entityComponentMap[entity].add(getID<Transform2DComponent>());
        mData->renderer2DComponent_transform2DComponentView.onComponent2Added(entity);
        mData->transform2DComponent_renderer2DComponentView.onComponent1Added(entity);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addComponent<Renderer2DComponent>(Entity entity, const Renderer2DComponent& component)
    {
        mData->renderer2DComponentPool.addComponent(entity, component);
        mData->entityComponentMap[entity].add(getID<Renderer2DComponent>());
        mData->renderer2DComponent_transform2DComponentView.onComponent1Added(entity);
        mData->transform2DComponent_renderer2DComponentView.onComponent2Added(entity);
    }

    template<>
    OTTO_DLL_FUNC void Scene::addComponent<Camera2DComponent>(Entity entity, const Camera2DComponent& component)
    {
        mData->camera2DComponentPool.addComponent(entity, component);
        mData->entityComponentMap[entity].add(getID<Camera2DComponent>());
    }

    template<typename C>
    OTTO_DLL_FUNC void Scene::removeComponent(Entity entity)
    {
        OTTO_ASSERT(false, "Component is not added.")
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeComponent<TestComponent>(Entity entity)
    {
        mData->testComponentPool.removeComponent(entity);
        mData->entityComponentMap[entity].remove(mData->entityComponentMap[entity].indexOf(getID<TestComponent>()));
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeComponent<TestComponent2>(Entity entity)
    {
        mData->testComponent2Pool.removeComponent(entity);
        mData->testSystem.onEvent(ComponentRemovedEvent<TestComponent2>(entity));
        mData->entityComponentMap[entity].remove(mData->entityComponentMap[entity].indexOf(getID<TestComponent2>()));
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeComponent<UIComponent>(Entity entity)
    {
        mData->uIComponentPool.removeComponent(entity);
        mData->uIRenderer.onEvent(ComponentRemovedEvent<UIComponent>(entity));
        mData->entityComponentMap[entity].remove(mData->entityComponentMap[entity].indexOf(getID<UIComponent>()));
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeComponent<Transform2DComponent>(Entity entity)
    {
        mData->transform2DComponentPool.removeComponent(entity);
        mData->renderer2DComponent_transform2DComponentView.onComponent2Removed(entity);
        mData->transform2DComponent_renderer2DComponentView.onComponent1Removed(entity);
        mData->entityComponentMap[entity].remove(mData->entityComponentMap[entity].indexOf(getID<Transform2DComponent>()));
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeComponent<Renderer2DComponent>(Entity entity)
    {
        mData->renderer2DComponentPool.removeComponent(entity);
        mData->renderer2DComponent_transform2DComponentView.onComponent1Removed(entity);
        mData->transform2DComponent_renderer2DComponentView.onComponent2Removed(entity);
        mData->entityComponentMap[entity].remove(mData->entityComponentMap[entity].indexOf(getID<Renderer2DComponent>()));
    }

    template<>
    OTTO_DLL_FUNC void Scene::removeComponent<Camera2DComponent>(Entity entity)
    {
        mData->camera2DComponentPool.removeComponent(entity);
        mData->entityComponentMap[entity].remove(mData->entityComponentMap[entity].indexOf(getID<Camera2DComponent>()));
    }

    template<typename C>
    OTTO_DLL_FUNC C& Scene::getComponent(Entity entity)
    {
        OTTO_ASSERT(false, "Component is not added.")
    }

    template<>
    OTTO_DLL_FUNC TestComponent& Scene::getComponent<TestComponent>(Entity entity)
    {
        return mData->testComponentPool.getComponent(entity);
    }

    template<>
    OTTO_DLL_FUNC TestComponent2& Scene::getComponent<TestComponent2>(Entity entity)
    {
        return mData->testComponent2Pool.getComponent(entity);
    }

    template<>
    OTTO_DLL_FUNC UIComponent& Scene::getComponent<UIComponent>(Entity entity)
    {
        return mData->uIComponentPool.getComponent(entity);
    }

    template<>
    OTTO_DLL_FUNC Transform2DComponent& Scene::getComponent<Transform2DComponent>(Entity entity)
    {
        return mData->transform2DComponentPool.getComponent(entity);
    }

    template<>
    OTTO_DLL_FUNC Renderer2DComponent& Scene::getComponent<Renderer2DComponent>(Entity entity)
    {
        return mData->renderer2DComponentPool.getComponent(entity);
    }

    template<>
    OTTO_DLL_FUNC Camera2DComponent& Scene::getComponent<Camera2DComponent>(Entity entity)
    {
        return mData->camera2DComponentPool.getComponent(entity);
    }

    template<typename C>
    OTTO_DLL_FUNC bool8 Scene::hasComponent(Entity entity)
    {
        OTTO_ASSERT(false, "Component is not added.")
    }

    template<>
    OTTO_DLL_FUNC bool8 Scene::hasComponent<TestComponent>(Entity entity)
    {
        return mData->entityComponentMap[entity].contains(getID<TestComponent>());
    }

    template<>
    OTTO_DLL_FUNC bool8 Scene::hasComponent<TestComponent2>(Entity entity)
    {
        return mData->entityComponentMap[entity].contains(getID<TestComponent2>());
    }

    template<>
    OTTO_DLL_FUNC bool8 Scene::hasComponent<UIComponent>(Entity entity)
    {
        return mData->entityComponentMap[entity].contains(getID<UIComponent>());
    }

    template<>
    OTTO_DLL_FUNC bool8 Scene::hasComponent<Transform2DComponent>(Entity entity)
    {
        return mData->entityComponentMap[entity].contains(getID<Transform2DComponent>());
    }

    template<>
    OTTO_DLL_FUNC bool8 Scene::hasComponent<Renderer2DComponent>(Entity entity)
    {
        return mData->entityComponentMap[entity].contains(getID<Renderer2DComponent>());
    }

    template<>
    OTTO_DLL_FUNC bool8 Scene::hasComponent<Camera2DComponent>(Entity entity)
    {
        return mData->entityComponentMap[entity].contains(getID<Camera2DComponent>());
    }

    OTTO_DLL_FUNC void Scene::_onKeyPressed(const _KeyPressedEvent& e)
    {
        dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::_onKeyReleased(const _KeyReleasedEvent& e)
    {
        dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::_onMouseButtonPressed(const _MouseButtonPressedEvent& e)
    {
        dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::_onMouseButtonReleased(const _MouseButtonReleasedEvent& e)
    {
        dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::_onMouseMoved(const _MouseMovedEvent& e)
    {
        dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::_onMouseDragged(const _MouseDraggedEvent& e)
    {
        dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::_onMouseScrolled(const _MouseScrolledEvent& e)
    {
        dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::_onWindowClosed(const _WindowClosedEvent& e)
    {
        dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::_onWindowResized(const _WindowResizedEvent& e)
    {
        dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::_onWindowGainedFocus(const _WindowGainedFocusEvent& e)
    {
        dispatchEvent(e);
    }

    OTTO_DLL_FUNC void Scene::_onWindowLostFocus(const _WindowLostFocusEvent& e)
    {
        dispatchEvent(e);
    }

} // namespace otto
