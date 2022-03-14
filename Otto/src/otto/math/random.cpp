#include "radom.h"

#include <random>
#include <limits>

namespace otto
{
    namespace
    {
        static std::random_device RANDOM_DEVICE;

        static std::mt19937 GENERATOR_32 = std::mt19937(RANDOM_DEVICE() );
        static std::mt19937_64 GENERATOR_64 = std::mt19937_64(RANDOM_DEVICE() );

        static std::uniform_int_distribution<uint32> DISTRIBUTION_INT_32 = 
            std::uniform_int_distribution<uint32>(std::numeric_limits<uint32>::min(), std::numeric_limits<uint32>::max());
        static std::uniform_int_distribution<uint64> DISTRIBUTION_INT_64 =
            std::uniform_int_distribution<uint64>(std::numeric_limits<uint64>::min(), std::numeric_limits<uint64>::max());

        static std::uniform_real_distribution<float32> DISTRIBUTION_FLOAT_32 = std::uniform_real_distribution<float32>(0.0f, 1.0f);
        static std::uniform_real_distribution<float64> DISTRIBUTION_FLOAT_64 = std::uniform_real_distribution<float64>(0.0, 1.0);

    } // namespace

    template<> 
    float32 random<float32>(float32 min, float32 max)
    {
        return DISTRIBUTION_FLOAT_32(GENERATOR_32) * (max - min) + min;
    }

    template<> 
    float64 random<float64>(float64 min, float64 max)
    {
        return DISTRIBUTION_FLOAT_64(GENERATOR_64) * (max - min) + min;
    }

    template<> 
    int8 random<int8>(int8 min, int8 max)
    {
        return int8(DISTRIBUTION_INT_32(GENERATOR_32) % uint32(max - min)) + min;
    }

    template<> 
    int16 random<int16>(int16 min, int16 max)
    {
        return int16(DISTRIBUTION_INT_32(GENERATOR_32) % uint32(max - min)) + min;
    }

    template<> 
    int32 random<int32>(int32 min, int32 max)
    {
        return int32(DISTRIBUTION_INT_32(GENERATOR_32) % uint32(max - min)) + min;
    }

    template<> 
    int64 random<int64>(int64 min, int64 max)
    {
        return int64(DISTRIBUTION_INT_64(GENERATOR_64) % uint64(max - min)) + min;
    }

    template<> 
    uint8 random<uint8>(uint8 min, uint8 max)
    {
        return uint8(DISTRIBUTION_INT_32(GENERATOR_32) % uint32(max - min)) + min;
    }

    template<> 
    uint16 random<uint16>(uint16 min, uint16 max)
    {
        return uint16(DISTRIBUTION_INT_32(GENERATOR_32) % uint32(max - min)) + min;
    }

    template<> 
    uint32 random<uint32>(uint32 min, uint32 max)
    {
        return (DISTRIBUTION_INT_32(GENERATOR_32) % (max - min)) + min;
    }

    template<> 
    uint64 random<uint64>(uint64 min, uint64 max)
    {
        return (DISTRIBUTION_INT_64(GENERATOR_64) % (max - min)) + min;
    }

} // namespace otto
