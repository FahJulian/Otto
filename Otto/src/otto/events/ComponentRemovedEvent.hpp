#pragma once

#include "otto/base.h"
#include "otto/scene/entity.h"

namespace otto
{
    template<typename C>
    struct ComponentRemovedEvent
    {
        Entity entity;
    };

} // namespace otto
