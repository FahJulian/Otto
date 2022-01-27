#pragma once

#include "otto/base.h"
#include "otto/math/math.h"
#include "otto/serialization/serialized.h"

namespace otto
{
    template<>
    Serialized serialize(const Vec3f32& v)
    {
        return String::valueOf(v.x) + ", " + String::valueOf(v.y) + ", " + String::valueOf(v.z);
    }

    template<>
    Vec3f32 deserialize(const Serialized& serialized)
    {
        auto values = String::split(serialized.get<String>(), ", ");
        return { values[0].to<float32>(), values[1].to<float32>(), values[2].to<float32>() };
    }

} // namespace otto
