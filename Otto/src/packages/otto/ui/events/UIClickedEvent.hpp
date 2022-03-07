#pragma once

#include "otto.h"

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
