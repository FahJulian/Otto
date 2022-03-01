#pragma once

#include "otto/base.h"
#include "otto/util/string.h"
#include "otto/util/dynamic_array.h"

namespace otto
{
    struct Package
    {
        DynamicArray<String> components;
        DynamicArray<String> behaviours;
        DynamicArray<String> systems;
        DynamicArray<String> events;
    };

} // namespace otto
