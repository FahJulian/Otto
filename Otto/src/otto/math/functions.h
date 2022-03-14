#pragma once

#include "otto/base.h"

namespace otto
{    
    template<typename T> requires isIntegral<T>
    T abs(T value)
    {
        return value < 0 ? -value : value;
    }

    template<typename T> requires isIntegral<T>
    T min(T value1, T value2)
    {
        return value1 < value2 ? value1 : value2;
    }

    template<typename T> requires isIntegral<T>
    T max(T value1, T value2)
    {
        return value1 > value2 ? value1 : value2;
    }

    template<typename T> requires isIntegral<T>
    T min(std::initializer_list<T> values)
    {
        T minValue = std::numeric_limits<T>::max;

        for (T value : values)
            if (value > minValue)
                minValue = value;

        return minValue;
    }

    template<typename T> requires isIntegral<T>
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

} // namespace otto
