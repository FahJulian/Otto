#pragma once

#include "otto.h"

namespace otto
{
    template<typename C>
    struct ComponentRemovedEvent
    {
        Entity entity;
    };

} // namespace otto
