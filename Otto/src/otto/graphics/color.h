#pragma once

#include "otto/base.h"
#include "otto/math/math.h"

namespace otto
{
    using ColorCode = uint32;

    struct Color
    {
        float32 r = 0.0f;
        float32 g = 0.0f;
        float32 b = 0.0f;
        float32 a = 0.0f;

        Color()
            : r(0.0f), g(0.0f), b(0.0f), a(0.0)
        {
        }

        Color(float32 r, float32 g, float32 b, float32 a)
            : r(r), g(g), b(b), a(a)
        {
        }

        Color(uint8 r, uint8 g, uint8 b, uint8 a)
            : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f)
        {
        }

        Color(ColorCode colorCode)
            : Color(((colorCode >> 24) & 255) / 255.0f, ((colorCode >> 16) & 255) / 255.0f,
                ((colorCode >> 8) & 255) / 255.0f, (colorCode & 255) / 255.0f)
        {
        }

        const Vec4<float32>& toVec4() const
        {
            return *reinterpret_cast<const Vec4<float32>*>(this);
        }

        ColorCode toColorCode() const
        {
            return (static_cast<uint32>(r * 255) << 24) +
                (static_cast<uint32>(g * 255) << 16) +
                (static_cast<uint32>(b * 255) << 8) +
                static_cast<uint32>(a * 255);
        }
    };

} // namespace otto
