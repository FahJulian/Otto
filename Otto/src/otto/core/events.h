#pragma once

#include "otto/base.h"

namespace otto
{
    struct _InitEvent
    {
    };

    struct _UpdateEvent
    {
        float32 delta;
    };

    struct _RebufferEvent
    {
    };

    struct _RenderEvent
    {
    };

} // namespace otto
