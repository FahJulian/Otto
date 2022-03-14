#include "trigonometry_functions.h"

#include <cmath>

namespace otto
{
    const float32 PI_32 = 3.14159274f;
    const float64 PI_64 = 3.1415926535897931;

    const float32 ONE_OVER_PI_32 = 0.318309873f;
    const float64 ONE_OVER_PI_64 = 0.31830988618379069;

    float32 radians(float32 angle)
    {
        return PI_32 * angle / 180.0f;
    }

    float64 radians(float64 angle)
    {
        return PI_64 * angle / 180.0;
    }

    float32 degrees(float32 radians)
    {
        return 180.0f * ONE_OVER_PI_32 * radians;
    }

    float64 degrees(float64 radians)
    {
        return 180.0 * ONE_OVER_PI_64 * radians;
    }

    float32 sin(float32 radians)
    {
        return std::sin(radians);
    }

    float64 sin(float64 radians)
    {
        return std::sin(radians);
    }

    float32 cos(float32 radians)
    {
        return std::cos(radians);
    }

    float64 cos(float64 radians)
    {
        return std::cos(radians);
    }

    float32 tan(float32 radians)
    {
        return std::tan(radians);
    }

    float64 tan(float64 radians)
    {
        return std::tan(radians);
    }

    float32 asin(float32 radians)
    {
        return std::asin(radians);
    }

    float64 asin(float64 radians)
    {
        return std::asin(radians);
    }

    float32 acos(float32 radians)
    {
        return std::acos(radians);
    }

    float64 acos(float64 radians)
    {
        return std::acos(radians);
    }

    float32 atan(float32 radians)
    {
        return std::atan(radians);
    }

    float64 atan(float64 radians)
    {
        return std::atan(radians);
    }

    float32 sinh(float32 radians)
    {
        return std::sinh(radians);
    }

    float64 sinh(float64 radians)
    {
        return std::sinh(radians);
    }

    float32 cosh(float32 radians)
    {
        return std::cosh(radians);
    }

    float64 cosh(float64 radians)
    {
        return std::cosh(radians);
    }

    float32 tanh(float32 radians)
    {
        return std::tanh(radians);
    }

    float64 tanh(float64 radians)
    {
        return std::tanh(radians);
    }

    float32 asinh(float32 radians)
    {
        return std::asinh(radians);
    }

    float64 asinh(float64 radians)
    {
        return std::asinh(radians);
    }

    float32 acosh(float32 radians)
    {
        return std::acosh(radians);
    }

    float64 acosh(float64 radians)
    {
        return std::acosh(radians);
    }

    float32 atanh(float32 radians)
    {
        return std::atanh(radians);
    }

    float64 atanh(float64 radians)
    {
        return std::atanh(radians);
    }

} // namespace otto
