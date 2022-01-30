#pragma once

#include "otto/base.h"
#include "otto/util/map.h"
#include "otto/scene/entity.h"

namespace otto
{
    template<typename C>
    class ComponentPool
    {
        void addComponent(Entity entity, const C& component)
        {
            OTTO_ASSERT(!mComponents.containsKey(entity), "Can't add Component: Pool already contains Entity");

            mComponents.insert(entity, component);
        }

        void removeComponent(Entity entity)
        {
            OTTO_ASSERT(mComponents.containsKey(entity), "Can't remove Component: Pool does not contain Entity " + entity);

            mComponents.remove(entity);
        }

        C& getComponent(Entity entity)
        {
            OTTO_ASSERT(mComponents.containsKey(entity), "Can't retrieve Component: Pool does not contain Entity " + entity);

            return mComponents[entity];
        }

        bool hasComponent(Entity entity)
        {
            return mComponents.containsKey(entity);
        }

        Map<Entity, C> mComponents;

        friend class Scene;

        template<typename C>
        friend class View;
        template<typename C1, typename C2>
        friend class MultiView;
    };

} // namespace otto
