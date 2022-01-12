#pragma once

#include "otto/base.h"

namespace otto
{
    class Time
    {
    public:
        static void init();

        static float32 getTime32();

        static float64 getTime64();

        static float32 getTime()
        {
            return getTime32();
        }
    };

} // namespace otto
