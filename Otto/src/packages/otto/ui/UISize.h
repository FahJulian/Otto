#pragma once

#include "otto.h"

namespace otto
{
    struct UISize
    {
        enum class Mode
        {
            ABSOLUTE,
        };

        Mode mode = Mode::ABSOLUTE;
        float32 absoluteValue = 0.0f;
    };

} // namespace otto
