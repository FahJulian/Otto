#pragma once

#include <cstdint>
#include <stdexcept>

#include "otto/platform_detection.h"

#define NONE {}

#ifdef OTTO_DEBUG

#define OTTO_ASSERT(b, message) if (!(b)) throw std::runtime_error(message);
#else
#define OTTO_ASSERT(b, ...)

#endif

#ifdef OTTO_DEBUG

#ifndef OTTO_COUNT_FPS
#ifndef OTTO_PREVENT_COUNT_FPS
#define OTTO_COUNT_FPS
#endif
#endif

#endif


namespace otto
{
    using bool8 = bool;

    using int8 = int8_t;
    using int16 = int16_t; 
    using int32 = int32_t; 
    using int64 = int64_t; 
    
    using uint8 = uint8_t; 
    using uint16 = uint16_t; 
    using uint32 = uint32_t; 
    using uint64 = uint64_t; 

    using float32 = float;
    using float64 = double;

    using char8 = char;
    using uchar8 = unsigned char;
    using char16 = wchar_t;

    using ptr64 = uintptr_t;

    template<typename T>
    static constexpr bool8 isIntegral = false;

    template<> static constexpr bool8 isIntegral<bool8> = true;

    template<> static constexpr bool8 isIntegral<int8> = true;
    template<> static constexpr bool8 isIntegral<int16> = true;
    template<> static constexpr bool8 isIntegral<int32> = true;
    template<> static constexpr bool8 isIntegral<int64> = true;

    template<> static constexpr bool8 isIntegral<uint8> = true;
    template<> static constexpr bool8 isIntegral<uint16> = true;
    template<> static constexpr bool8 isIntegral<uint32> = true;
    template<> static constexpr bool8 isIntegral<uint64> = true;

    template<> static constexpr bool8 isIntegral<float32> = true;
    template<> static constexpr bool8 isIntegral<float64> = true;

    template<> static constexpr bool8 isIntegral<char8> = true;
    template<> static constexpr bool8 isIntegral<char16> = true;

    template<typename T>
    static constexpr bool8 isNotIntegral = !isIntegral<T>;

    template<typename T>
    static constexpr bool8 isInteger = false;

    template<> static constexpr bool8 isInteger<int8> = true;
    template<> static constexpr bool8 isInteger<int16> = true;
    template<> static constexpr bool8 isInteger<int32> = true;
    template<> static constexpr bool8 isInteger<int64> = true;
    template<> static constexpr bool8 isInteger<uint8> = true;
    template<> static constexpr bool8 isInteger<uint16> = true;
    template<> static constexpr bool8 isInteger<uint32> = true;
    template<> static constexpr bool8 isInteger<uint64> = true;

    template<typename T>
    static constexpr bool8 isFloat = false;

    template<> static constexpr bool8 isFloat<float32> = true;
    template<> static constexpr bool8 isFloat<float64> = true;

    template<typename T>
    static constexpr bool isNumber = isInteger<T> || isFloat<T>;

} // namespace otto
