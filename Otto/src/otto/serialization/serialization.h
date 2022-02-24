#pragma once

#include "otto/base.h"
#include "otto/util/map.h"
#include "otto/math/math.h"
#include "otto/serialization/serializer.h"
#include "otto/serialization/serialized.h"

namespace otto
{
    template<typename T> requires std::is_integral_v<T>
    Serialized serialize(const Vec2<T>& v)
    {
        return String::valueOf(v.x) + ", " + String::valueOf(v.y);
    }

    template<>
    Serialized serialize(const Vec3f32& v)
    {
        return String::valueOf(v.x) + ", " + String::valueOf(v.y) + ", " + String::valueOf(v.z);
    }

    template<typename T> requires std::is_integral_v<T>
    Vec2<T> deserialize(const Serialized& serialized)
    {
        auto values = String::split(serialized.get<String>(), ", ");
        return { values[0].to<T>(), values[1].to<T>() };
    }

    template<>
    Vec3f32 deserialize(const Serialized& serialized)
    {
        auto values = String::split(serialized.get<String>(), ", ");
        return { values[0].to<float32>(), values[1].to<float32>(), values[2].to<float32>() };
    }

} // namespace otto
