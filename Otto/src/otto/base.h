#pragma once

#include <cstdint>
#include <stdexcept>

#include "otto/core/platform_detection.h"

#define NONE {}

#ifdef OTTO_DEBUG
#define OTTO_ASSERT(b, message) if (!(b)) throw std::runtime_error(message);
#else
#define OTTO_ASSERT(b, ...)
#endif


namespace otto
{
    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef int64_t int64;
    
    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef uint64_t uint64;

    typedef float float32;
    typedef double float64;

    typedef uintptr_t ptr64;
}
