#pragma once

#include "otto/base.h"
#include "otto/graphics/color.h"
#include "otto/graphics/sprite.h"
#include "otto/serialization/serialization.h"

namespace otto
{
    struct UIRenderingProperties
    {
        Sprite sprite;
        Color color = 0xffffffff;

        Color borderColor;
        float32 borderWeight;
        float32 edgeRadius;
    };

    template<>
    UIRenderingProperties deserialize(const Serialized& serialized)
    {
        UIRenderingProperties properties = { };

        if (serialized.contains("sprite"))
            properties.sprite = serialized.get<Sprite>("sprite");
        if (serialized.contains("color"))
            properties.color = serialized.get<Color>("color");
        if (serialized.contains("borderColor"))
            properties.borderColor = serialized.get<Color>("borderColor");
        if (serialized.contains("borderWeight"))
            properties.borderWeight = serialized.get<float32>("borderWeight");
        if (serialized.contains("edgeRadius"))
            properties.edgeRadius = serialized.get<float32>("edgeRadius");

        return properties;
    }

} // namespace otto
