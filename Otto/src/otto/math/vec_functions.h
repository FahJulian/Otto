#pragma once

#include "otto/base.h"
#include "otto/math/vec.h"
#include "otto/math/vec2.h"
#include "otto/math/vec3.h"
#include "otto/math/vec4.h"

namespace otto
{
    template<typename T>
    T length(const Vec<3, T>& vec)
    {
        return sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    }

    template<typename T, typename F>
    T dot(const Vec<3, T>& v1, const Vec<3, F>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    template<typename T, typename F>
    T distance(const Vec<3, T>& v1, const Vec<3, F>& v2)
    {
        return length(v2 - v1);
    }

    template<typename T>
    Vec<3, T> normalize(const Vec<3, T>& vec)
    {
        return (1 / length(vec)) * vec;
    }

    template<typename T, typename F>
    Vec<3, T> cross(const Vec<3, T>& v1, const Vec<3, F>& v2)
    {
        return {
            T(v1.y * v2.z - v2.y * v1.z),
            T(v1.z * v2.x - v2.z * v1.x),
            T(v1.x * v2.y - v2.x * v1.y),
        };
    }

} // namespace otto
