#pragma once

#include <string>

#include "otto/base.h"
#include "otto/util/map.h"
#include "otto/math/math.h"
#include "otto/graphics/color.h"
#include "otto/serialization/serializer.h"
#include "otto/serialization/serialized.h"

namespace otto
{
    template<>
    Serialized serialize(const String& string);

    template<typename T> requires std::is_integral_v<T>
    Serialized serialize(const Vec2<T>& v);

    template<>
    Serialized serialize(const Vec3f32& v);

	template<>
    Serialized serialize(const Color& color);

    template<>
    String deserialize(const Serialized& serialized);

    template<typename T> requires std::is_integral_v<T>
    Vec2<T> deserialize(const Serialized& serialized);

    template<>
    Vec3f32 deserialize(const Serialized& serialized);
	
	template<>
    Color deserialize(const Serialized& serialized);

} // namespace otto
