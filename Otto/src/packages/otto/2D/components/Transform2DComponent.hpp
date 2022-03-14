#pragma once

#include <otto.h>

namespace otto
{
    struct Transform2DComponent
    {
        Vec2<float32> translation;
        Vec2<float32> scale;
        float32 rotation = 0.0f;
    };

    template<>
    Transform2DComponent deserializeComponent(const Serialized& args, const Map<String, Entity>& entities)
    {
        Transform2DComponent component;

        if (args.contains("translation"))
        {
        Log::info("Test");
            component.translation = args.get<Vec2<float32>>("translation");
        }
        if (args.contains("scale"))
            component.scale = args.get<Vec2<float32>>("scale");
        if (args.contains("rotation"))
            component.rotation = args.get<float32>("rotation");

        return component;
    }

    template<>
    Serialized serializeComponent<Transform2DComponent>(const Transform2DComponent& c, const Map<String, Entity>& entities)
    {
        Serialized serialized;

        serialized.insert("translation", c.translation);
        serialized.insert("scale", c.scale);
        serialized.insert("rotation", c.rotation);

        return serialized;
    }

} // namespace otto
