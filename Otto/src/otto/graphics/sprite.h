#pragma once

#include "otto/base.h"
#include "otto/graphics/texture_2D.h"

namespace otto
{
    struct Sprite
    {
        Texture2D texture;

        float32 x0, y0;
        float32 x1, y1;
    };

} // namespace otto
