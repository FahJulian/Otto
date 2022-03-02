#pragma once

#include "otto/base.h"
#include "otto/scene/view.h"
#include "otto/util/shared.h"
#include "otto/scene/entity.h"
#include "otto/debug/log/log.h"
#include "otto/core/application.h"
#include "otto/event/event_listener.h"
#include "otto/scene/component_pool.h"
#include "otto/serialization/serialized.h"

#pragma warning(disable: 4005)

#ifdef OTTO_DYNAMIC

#define OTTO_RCR_DLL_PATH Application::_getClientDllPath()

#include "otto/util/dll_reloading/dll_reloading.h"

#endif


#define OTTO_RCR_NAMESPACE "otto"
namespace otto
{
	struct SceneData;

    class Scene
    {
#ifdef OTTO_DYNAMIC

		using EntityMap = Map<String, Entity>;

	public:
        //OTTO_RCR_MEMBER_FUNCTION_1_ARGS(void, load, SceneLoadData& sceneLoadData);
		OTTO_RCR_MEMBER_FUNCTION_0_ARGS(void, init);
		OTTO_RCR_MEMBER_FUNCTION_2_ARGS(void, update, float32, delta32, float64, delta64);

		OTTO_RCR_MEMBER_FUNCTION_0_ARGS(Entity, addEntity);

		template<typename E>
		OTTO_RCR_MEMBER_FUNCTION_1_ARGS(void, addEventListener, const EventListener<E>&, eventListener);

		template<typename E>
		OTTO_RCR_MEMBER_FUNCTION_1_ARGS(void, removeEventListener, const EventListener<E>&, eventListener);

		template<typename E>
		OTTO_RCR_MEMBER_FUNCTION_1_ARGS(void, dispatchEvent, const E&, e, E);

		template<typename C>
		OTTO_RCR_MEMBER_FUNCTION_2_ARGS(void, addComponent, Entity, entity, const C&, component, C);

		template<typename C>
		OTTO_RCR_MEMBER_FUNCTION_1_ARGS(void, removeComponent, Entity, entity, C);

		template<typename C>
		OTTO_RCR_MEMBER_FUNCTION_1_ARGS(C&, getComponent, Entity, entity, C);

		template<typename C>
		OTTO_RCR_MEMBER_FUNCTION_1_ARGS(bool, hasComponent, Entity, entity, C);

	private:
		OTTO_RCR_MEMBER_FUNCTION_4_ARGS(void, addComponent, Entity, entity, const String&, componentName, const Serialized&, args, const EntityMap&, entities);

		Scene(SceneData* data)
			: mData(data)
		{
		}

		SceneData* mData;

		friend class SceneLoader;
		friend class _SceneInitializer;
		friend class Application;
    };

	class _SceneInitializer
	{
	private:
		_SceneInitializer() = default;

		OTTO_RCR_MEMBER_FUNCTION_0_ARGS(Shared<Scene>, createScene);

		OTTO_RCR_MEMBER_FUNCTION_1_ARGS(void, initClientLog, Log*, mainLog);

		friend class SceneLoader;
	};

#endif

} // namespace otto

#pragma warning(default : 4005)
