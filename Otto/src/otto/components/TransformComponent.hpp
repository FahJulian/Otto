#pragma once

#include <otto.h>

namespace otto
{
    struct TransformComponent
    {
        Vec3f32 translation;
        Vec3f32 rotation;
        Vec3f32 scale;
    };

    template<>
    TransformComponent deserializeComponent(const Serialized& args, const Map<String, Entity>& entities)
    {
        TransformComponent c;

        if (args.contains("translation"))
            c.translation = args.get<Vec3f32>("translation");
        if (args.contains("rotation"))
            c.rotation = args.get<Vec3f32>("rotation");
        if (args.contains("scale"))
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
