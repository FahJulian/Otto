#pragma once

#include "otto/base.h"
#include "otto/core/events.h"
#include "otto/util/shared.h"
#include "otto/scene/entity.h"
#include "otto/window/events.h"
#include "otto/debug/log/log.h"
#include "otto/event/event_listener.h"
#include "otto/serialization/serialized.h"
#include "otto/util/dll_reloading/dll_reloader.h"

#pragma warning(disable: 4005)

namespace otto
{
	struct SceneData;

    class Scene
    {
	public:
		OTTO_DLL_FUNC void init();

		OTTO_DLL_FUNC Entity addEntity();

		OTTO_DLL_FUNC void removeEntity(Entity entity);

		template<typename C>
		OTTO_DLL_FUNC void addComponent(Entity entity, const C& component);

		template<typename C>
		OTTO_DLL_FUNC void removeComponent(Entity entity);

		template<typename C>
		OTTO_DLL_FUNC bool hasComponent(Entity entity);

		template<typename C>
		OTTO_DLL_FUNC C& getComponent(Entity entity);

		template<typename E>
		OTTO_DLL_FUNC void addEventListener(const EventListener<E>& eventListener);

		template<typename E>
		OTTO_DLL_FUNC void removeEventListener(const EventListener<E>& eventListener);

		template<typename E>
		OTTO_DLL_FUNC void dispatchEvent(const E& e);

	private:
		OTTO_DLL_FUNC void addComponent(Entity entity, const String& componentName, const Serialized& args, const Map<String, Entity>& entities);

		OTTO_DLL_FUNC void _onKeyPressed(const _KeyPressedEvent& e);
		OTTO_DLL_FUNC void _onKeyReleased(const _KeyReleasedEvent& e);
		OTTO_DLL_FUNC void _onMouseButtonPressed(const _MouseButtonPressedEvent& e);
		OTTO_DLL_FUNC void _onMouseButtonReleased(const _MouseButtonReleasedEvent& e);
		OTTO_DLL_FUNC void _onMouseMoved(const _MouseMovedEvent& e);
		OTTO_DLL_FUNC void _onMouseDragged(const _MouseDraggedEvent& e);
		OTTO_DLL_FUNC void _onMouseScrolled(const _MouseScrolledEvent& e);
		OTTO_DLL_FUNC void _onWindowClosed(const _WindowClosedEvent& e);
		OTTO_DLL_FUNC void _onWindowResized(const _WindowResizedEvent& e);
		OTTO_DLL_FUNC void _onWindowGainedFocus(const _WindowGainedFocusEvent& e);
		OTTO_DLL_FUNC void _onWindowLostFocus(const _WindowLostFocusEvent& e);
		OTTO_DLL_FUNC void _onInit(const _InitEvent& e);
		OTTO_DLL_FUNC void _onUpdate(const _UpdateEvent& e);
		OTTO_DLL_FUNC void _onRebuffer(const _RebufferEvent& e);
		OTTO_DLL_FUNC void _onRender(const _RenderEvent& e);

		OTTO_DLL_FUNC static Shared<Scene> _createScene();
		OTTO_DLL_FUNC static void _initClientLog(Log* mainLog);

		Scene(SceneData* data)
			: mData(data)
		{
		}

		SceneData* mData;

		friend class SceneLoader;
		friend class Application;
    };

} // namespace otto

#pragma warning(default : 4005)
