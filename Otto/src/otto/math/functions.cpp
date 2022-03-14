#include "functions.h"

#include <cmath>

namespace otto
{
    namespace
    {
        template<typename T>
        T _integerSqrt(T value)
        {
            if (value <= static_cast<T>(1))
                return value;

            T nextTrial = value >> 1;
            T currentAnswer;

            do
            {
                currentAnswer = nextTrial;
                nextTrial = (nextTrial + value / nextTrial) >> 1;
            } while (nextTrial < currentAnswer);

            return currentAnswer;
        }

    } // namespace

    float32 sqrt(float32 value)
    {
        return static_cast<float32>(std::sqrt(value));
    }

    float64 sqrt(float64 value)
    {
        return std::sqrt(value);
    }

    int8 sqrt(int8 value)
    {
        return _integerSqrt(value);
    }

    int16 sqrt(int16 value)
    {
        return _integerSqrt(value);
    }

    int32 sqrt(int32 value)
    {
        return _integerSqrt(value);
    }

    int64 sqrt(int64 value)
    {
        return _integerSqrt(value);
    }

    uint8 sqrt(uint8 value)
    {
        return _integerSqrt(value);
    }

    uint16 sqrt(uint16 value)
    {
        return _integerSqrt(value);
    }

    uint32 sqrt(uint32 value)
    {
        return _integerSqrt(value);
    }

    uint64 sqrt(uint64 value)
    {
        return _integerSqrt(value);
    }

    float32 pow(float32 base, uint8 exponent)
    {
        return pow(base, float32(exponent));
    }

    float32 pow(float32 base, uint16 exponent)
    {
        return pow(base, float32(exponent));
    }

    float32 pow(float32 base, uint32 exponent)
    {
        return pow(base, float32(exponent));
    }

    float32 pow(float32 base, uint64 exponent)
    {
        return pow(base, float32(exponent));
    }

    float32 pow(float32 base, float32 exponent)
    {
        return std::pow(base, exponent);
    }

    float64 pow(float64 base, uint8 exponent)
    {
        return pow(base, float64(exponent));
    }

    float64 pow(float64 base, uint16 exponent)
    {
        return pow(base, float64(exponent));
    }

    float64 pow(float64 base, uint32 exponent)
    {
        return pow(base, float64(exponent));
    }

    float64 pow(float64 base, uint64 exponent)
    {
        return pow(base, float64(exponent));
    }

    float64 pow(float64 base, float64 exponent)
    {
        return std::pow(base, exponent);
    }

} // namespace otto
