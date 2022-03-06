#pragma once

#include "otto/base.h"
#include "otto/core/events.h"
#include "otto/util/shared.h"
#include "otto/scene/entity.h"
#include "otto/window/events.h"
#include "otto/window/window.h"
#include "otto/debug/log/log.h"
#include "otto/graphics/color.h"
#include "otto/core/application.h"
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
		OTTO_DLL_FUNC bool8 hasComponent(Entity entity);

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

		OTTO_DLL_FUNC void update(float32 delta);
		OTTO_DLL_FUNC void rebuffer();
		OTTO_DLL_FUNC void render();

		OTTO_DLL_FUNC static Shared<Scene> _createScene();
		OTTO_DLL_FUNC static void _initClient(Application* mainApplication, Window* mainWindow, Log* mainLog, const Color& clearColor);

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
