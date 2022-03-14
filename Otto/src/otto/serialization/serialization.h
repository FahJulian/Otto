#pragma once

#include <string>

#include "otto/base.h"
#include "otto/util/map.h"
#include "otto/math/math.h"
#include "otto/math/vec2.h"
#include "otto/math/vec3.h"
#include "otto/math/vec4.h"
#include "otto/graphics/color.h"
#include "otto/graphics/sprite.h"
#include "otto/serialization/serializer.h"
#include "otto/serialization/serialized.h"

#include "otto/debug/log.h"

namespace otto
{
    template<typename T>
    Serialized serialize(T value) requires isIntegral<T>
    {
        return String::valueOf(value);
    }

    template<>
    Serialized serialize(const String& string);

    template<typename T> requires isIntegral<T>
    Serialized serialize(const Vec2<T>& v)
    {
        return String::valueOf(v.x) + ", " + String::valueOf(v.y);
    }

    template<typename T> requires isIntegral<T>
    Serialized serialize(const Vec3<T>& v)
    {
        return String::valueOf(v.x) + ", " + String::valueOf(v.y) + ", " + String::valueOf(v.z);
    }

    template<typename T> requires isIntegral<T>
    Serialized serialize(const Vec4<T>& v)
    {
        return String::valueOf(v.x) + ", " + String::valueOf(v.y) + ", " + String::valueOf(v.z), + ", " + String::valueOf(v.w);
    }

	template<>
    Serialized serialize(const Color& color);

    template<typename T>
    T deserialize(const Serialized& serialized) requires isIntegral<T>
    {
        return serialized.get<T>();
    }

    template<>
    String deserialize(const Serialized& serialized);

    //template<typename T>
    //Vec2<T> deserialize(const Serialized& serialized) requires isIntegral<T>
    //{
    //    Log::debug("In");
    //    auto values = String::split(serialized.get<String>(), ", ");

    //    return { String::stringTo<T>(values[0]), String::stringTo<T>(values[1]) };
    //}

    template<>
    Vec2<float32> deserialize(const Serialized& serialized);

    template<typename T> requires isIntegral<T>
    Vec3<T> deserialize(const Serialized& serialized)
    {
        auto values = serialized.get<String>().split(", ");

        return { values[0], values[1], values[2] };
    }

    template<typename T> requires isIntegral<T>
    Vec4<T> deserialize(const Serialized& serialized)
    {
        auto values = serialized.get<String>().split(", ");

        return { values[0], values[1], values[2], values[3] };
    }

	template<>
    Color deserialize(const Serialized& serialized);

    template<>
    Sprite deserialize(const Serialized& serialized);

} // namespace otto
