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

    typedef char char8;
    typedef unsigned char uchar8;
    typedef wchar_t char16;

    typedef uintptr_t ptr64;
}
