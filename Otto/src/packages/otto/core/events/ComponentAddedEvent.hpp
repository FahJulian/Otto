#pragma once

#include "otto.h"

namespace otto
{
    template<typename C>
    struct ComponentAddedEvent
    {
        Entity entity;
    };

} // namespace otto
