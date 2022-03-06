#pragma once

#include "otto/base.h"
#include "otto/window/input/mouse.h"

namespace otto
{
    struct UIClickedEvent
    {
        float32 x;
        float32 y;
        float32 windowX;
        float32 windowY;
        MouseButton button;
    };

} // namespace otto
