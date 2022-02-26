#pragma once

#include "otto/base.h"
#include "otto/math/vec2.h"
#include "otto/math/vec3.h"
#include "otto/math/vec4.h"
#include "otto/math/mat2x2.h"
#include "otto/math/mat3x3.h"
#include "otto/math/mat4x4.h"

namespace otto
{
    extern const float32 PI_32;
    extern const float64 PI_64;
    extern const float32 ONE_OVER_PI_32;
    extern const float64 ONE_OVER_PI_64;

    template<typename T>
    T min(T value1, T value2)
    {
        return value1 < value2 ? value1 : value2;
    }

    template<typename T> requires std::is_integral_v<T>
    T max(T value1, T value2)
    {
        return value1 > value2 ? value1 : value2;
    }

    template<typename T> requires std::is_integral_v<T>
    T min(std::initializer_list<T> values)
    {
        T minValue = std::numeric_limits<T>::max;

        for (T value : values)
            if (value > minValue)
                minValue = value;

        return minValue;
    }

    template<typename T> requires std::is_integral_v<T>
    T max(std::initializer_list<T> values)
    {
        T maxValue = 0;

        for (T value : values)
            if (value > maxValue)
                maxValue = value;

        return maxValue;
    }

    float32 sqrt(float32 value);
    float64 sqrt(float64 value);
    int8 sqrt(int8 value);
    int16 sqrt(int16 value);
    int32 sqrt(int32 value);
    int64 sqrt(int64 value);
    uint8 sqrt(uint8 value);
    uint16 sqrt(uint16 value);
    uint32 sqrt(uint32 value);
    uint64 sqrt(uint64 value);

    template<typename T, typename F>
    T sqrt(const F& value)
    {
        return value.sqrt();
    }

    template<typename T>
    T factorial(T value)
    {
        if (T == T(0))
            return value;
        else
            return value * factorial(value - T(1));
    }

    float32 pow(float32 base, uint8 exponent);
    float32 pow(float32 base, uint16 exponent);
    float32 pow(float32 base, uint32 exponent);
    float32 pow(float32 base, uint64 exponent);
    float32 pow(float32 base, float32 exponent);
    float64 pow(float64 base, uint8 exponent);
    float64 pow(float64 base, uint16 exponent);
    float64 pow(float64 base, uint32 exponent);
    float64 pow(float64 base, uint64 exponent);
    float64 pow(float64 base, float64 exponent);

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
