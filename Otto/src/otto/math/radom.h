#pragma once

#include "otto/base.h"

namespace otto
{
    template<typename T>
    T random() requires isNumber<T>
    {
        static_assert(false);
    }

    template<typename T>
    T random(T min, T max) requires isNumber<T>
    {
        static_assert(false);
    }

    template<> float32 random<float32>(float32 min, float32 max);
    template<> float64 random<float64>(float64 min, float64 max);

    template<> int8 random<int8>(int8 min, int8 max);
    template<> int16 random<int16>(int16 min, int16 max);
    template<> int32 random<int32>(int32 min, int32 max);
    template<> int64 random<int64>(int64 min, int64 max);
    template<> uint8 random<uint8>(uint8 min, uint8 max);
    template<> uint16 random<uint16>(uint16 min, uint16 max);
    template<> uint32 random<uint32>(uint32 min, uint32 max);
    template<> uint64 random<uint64>(uint64 min, uint64 max);

} // namespace otto
