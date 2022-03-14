#pragma once

#include "otto/base.h"

namespace otto
{
    extern const float32 PI_32;
    extern const float64 PI_64;
    extern const float32 ONE_OVER_PI_32;
    extern const float64 ONE_OVER_PI_64;

    float32 radians(float32 degrees);
    float64 radians(float64 degrees);

    float32 degrees(float32 radians);
    float64 degrees(float64 radians);

    float32 sin(float32 radians);
    float64 sin(float64 radians);

    float32 cos(float32 radians);
    float64 cos(float64 radians);

    float32 tan(float32 radians);
    float64 tan(float64 radians);

    float32 asin(float32 radians);
    float64 asin(float64 radians);

    float32 acos(float32 radians);
    float64 acos(float64 radians);

    float32 atan(float32 radians);
    float64 atan(float64 radians);

    float32 sinh(float32 radians);
    float64 sinh(float64 radians);

    float32 cosh(float32 radians);
    float64 cosh(float64 radians);

    float32 tanh(float32 radians);
    float64 tanh(float64 radians);

    float32 asinh(float32 radians);
    float64 asinh(float64 radians);

    float32 acosh(float32 radians);
    float64 acosh(float64 radians);

    float32 atanh(float32 radians);
    float64 atanh(float64 radians);

} // namespace otto
