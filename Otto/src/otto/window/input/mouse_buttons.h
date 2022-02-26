#pragma once

#include "otto/base.h"

namespace otto 
{
    enum class MouseButton : uint8
    {
        LEFT = 0x00,
        RIGHT = 0x01,
        MIDDLE = 0x02,
        MB_4 = 0x03,
        MB_5 = 0x04,
        _HIGHEST_BUTTON = 0x04
    };

} // namespace otto
