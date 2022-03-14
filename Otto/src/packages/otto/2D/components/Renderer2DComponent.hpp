#pragma once

#include "otto.h"

namespace otto
{
    struct Renderer2DComponent
    {
        Color color = 0xFFFFFFFF;
        Sprite sprite;
    };

    template<>
    Renderer2DComponent deserializeComponent(const Serialized& args, const Map<String, Entity>& entities)
    {
        Renderer2DComponent component;

        if (args.contains("color"))
            component.color = args.get<Color>("color");
        if (args.contains("sprite"))
            component.sprite = args.get<Sprite>("sprite");

        return component;
    }

    template<>
    Serialized serializeComponent(const Renderer2DComponent& component, const Map<String, Entity>& entities)
    {
        Serialized serialized;

        serialized.insert("color", component.color);
        serialized.insert("sprite", component.sprite);

        return serialized;
    }

} // namespace otto
