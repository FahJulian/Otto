#pragma once

#include "otto/base.h"
#include "otto/math/vec3.h"
#include "otto/scene/serialization.h"

namespace otto
{
    struct TransformComponent
    {
        Vec3f32 translation;
        Vec3f32 rotation;
        Vec3f32 scale;
    };

    template<>
    TransformComponent deserialize<TransformComponent>(const Serialized& args)
    {
        TransformComponent c;

        c.translation = args.get<Vec3f32>("translation");
        c.rotation = args.get<Vec3f32>("rotation");
        c.scale = args.get<Vec3f32>("scale");

        return c;
    }

    template<>
    Serialized serialize<TransformComponent>(const TransformComponent& c)
    {
        Serialized s;

        s.insert("translation", c.translation);
        s.insert("rotation", c.rotation);
        s.insert("scale", c.scale);

        return s;
    }

} // namespace otto
