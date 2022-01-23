#pragma once

#include "otto/util/string.h"
#include "otto/util/map.h"

namespace otto
{
    struct SceneSettings
    {
        Map<String, String> components;
        Map<String, String> behaviours;
        Map<String, String> systems;
    };

} // namespace otto
