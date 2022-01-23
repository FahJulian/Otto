#pragma once

#include "otto/base.h"
#include "otto/scene/entity.h"
#include "otto/util/optional.h"
#include "otto/debug/log/log.h"
#include "otto/event/event_listener.h"
#include "otto/scene/component_pool.h"

#define OTTO_DYNAMIC

#ifdef OTTO_DYNAMIC

#define OTTO_RCR_DLL_PATH getClientDllPath()
#include "otto/util/dll_reloading/dll_reloading.h"

#endif


namespace otto
{
	struct SceneData;
	enum SceneType : uint16;

	OTTO_RCR_FUNCTION_0_ARGS(String, getClientDllPath);

    class Scene
    {
#ifdef OTTO_DYNAMIC

        //OTTO_RCR_MEMBER_FUNCTION_1_ARGS(void, load, SceneLoadData& sceneLoadData);
		OTTO_RCR_MEMBER_FUNCTION_0_ARGS(void, init);
		OTTO_RCR_MEMBER_FUNCTION_1_ARGS(void, update, float, delta);

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
		Scene(SceneData* data, SceneType type)
			: mData(data), mType(type)
		{
		}

		SceneData* mData;
		SceneType mType;

		friend Optional<Scene*> createScene(const String& sceneName);
#endif
    };

#ifdef OTTO_DYNAMIC
    OTTO_RCR_FUNCTION_1_ARGS(Optional<Scene*>, createScene, const String&, sceneName);
    OTTO_RCR_FUNCTION_1_ARGS(void, initClientLog, Log*, mainLog);
#endif
}
