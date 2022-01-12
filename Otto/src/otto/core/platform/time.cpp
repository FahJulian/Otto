#include "time.h"

#ifdef OTTO_WINDOWS

#include <Windows.h>

namespace otto
{
    static bool INITIALIZED = false;
    static uint64 INIT_TICKS = 0;
    static uint64 FREQUENCY = 0;

    void Time::init()
    {
        if (INITIALIZED)
            return;

        LARGE_INTEGER initTicks;
        QueryPerformanceCounter(&initTicks);
        INIT_TICKS = initTicks.QuadPart;

        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        FREQUENCY = frequency.QuadPart;

        INITIALIZED = true;
    }

    float32 Time::getTime32()
    {
        LARGE_INTEGER currentTicks;
        QueryPerformanceCounter(&currentTicks);

        return float32(currentTicks.QuadPart - INIT_TICKS) / float32(FREQUENCY);
    }

    float64 Time::getTime64()
    {
        LARGE_INTEGER currentTicks;
        QueryPerformanceCounter(&currentTicks);

        return float64(currentTicks.QuadPart - INIT_TICKS) / float64(FREQUENCY);
    }

} // namespace otto

#endif
