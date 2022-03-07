#pragma once

#include <string>

#include "otto/base.h"
#include "otto/util/map.h"
#include "otto/math/math.h"
#include "otto/graphics/color.h"
#include "otto/graphics/sprite.h"
#include "otto/serialization/serializer.h"
#include "otto/serialization/serialized.h"

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
    Serialized serialize(const Vec2<T>& v);

    template<>
    Serialized serialize(const Vec3f32& v);

	template<>
    Serialized serialize(const Color& color);

    template<typename T>
    T deserialize(const Serialized& serialized) requires isIntegral<T>
    {
        return serialized.get<T>();
    }

    template<>
    String deserialize(const Serialized& serialized);

    template<typename T> requires isIntegral<T>
    Vec2<T> deserialize(const Serialized& serialized);

    template<>
    Vec3f32 deserialize(const Serialized& serialized);
	
	template<>
    Color deserialize(const Serialized& serialized);

    template<>
    Sprite deserialize(const Serialized& serialized);

} // namespace otto
