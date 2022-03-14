#pragma once

#include "otto/base.h"

namespace otto
{
    template<uint64 R, uint64 C, typename T>
    struct Mat;

    template<typename T> using Mat2x2 = Mat<2, 2, T>;
    template<typename T> using Mat3x3 = Mat<3, 3, T>;
    template<typename T> using Mat4x4 = Mat<4, 4, T>;

    template<typename T> using Mat2 = Mat<2, 2, T>;
    template<typename T> using Mat3 = Mat<3, 3, T>;
    template<typename T> using Mat4 = Mat<4, 4, T>;

} // namespace otto
