#pragma once

#include "otto/base.h"
#include "otto/scene/entity.h"

namespace otto
{
    template<typename C>
    struct ComponentAddedEvent
    {
        Entity entity;
    };

} // namespace otto
