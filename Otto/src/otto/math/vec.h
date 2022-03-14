#pragma once

#include "otto/base.h"

namespace otto
{
    template<uint64 L, typename T>
    struct Vec;

    template<typename T> using Vec2 = Vec<2, T>;
    template<typename T> using Vec3 = Vec<3, T>;
    template<typename T> using Vec4 = Vec<4, T>;

} // namespace otto
