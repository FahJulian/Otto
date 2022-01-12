#include "math.h"
#include "otto/debug/profile/profiler.h"

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

    const float32 PI_32 = 3.14159274f;
    const float64 PI_64 = 3.1415926535897931;

    const float32 ONE_OVER_PI_32 = 0.318309873f;
    const float64 ONE_OVER_PI_64 = 0.31830988618379069;

    float32 sqrt(float32 value)
    {
        OTTO_PROFILE_FUNCTION("sqrt");

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

    float32 radians(float32 degrees)
    {
        return PI_32 / 180.0f * degrees;
    }

    float64 radians(float64 degrees)
    {
        return PI_64 / 180.0 * degrees;
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
