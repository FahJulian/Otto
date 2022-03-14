#pragma once

#include "otto.h"

#include "packages/otto/ui/UISize.h"
#include "packages/otto/ui/UIRenderingProperties.h"
#include "packages/otto/ui/events/UIClickedEvent.hpp"

namespace otto
{
    struct UIComponent
    {
        Entity parent = 0;

        bool8 dirty;

        UISize x;
        UISize y;
        UISize width;
        UISize height;
        float32 zIndex;

        bool8 hovered = false;

        UIRenderingProperties renderingProperties;
        UIRenderingProperties hoveredRenderingProperties;

        EventListener<UIClickedEvent> clickListener;
        EventListener<UIClickedEvent> hoverListener;
    };

    template<>
    UIComponent deserializeComponent(const Serialized& args, const Map<String, Entity>& entities)
    {
        UIComponent component = { };

        if (args.contains("x"))
        {
            if (args.get("x").isValue())
            {
                float32 x = args.get<float32>("x");
                component.x = { UISize::Mode::ABSOLUTE, x };
            }
        }
        if (args.contains("y"))
        {
            if (args.get("y").isValue())
                component.y = { UISize::Mode::ABSOLUTE, args.get<float32>("y") };
        }
        if (args.contains("width"))
        {
            if (args.get("width").isValue())
                component.width = { UISize::Mode::ABSOLUTE, args.get<float32>("width") };
        }
        if (args.contains("height"))
        {
            if (args.get("height").isValue())
                component.height = { UISize::Mode::ABSOLUTE, args.get<float32>("height") };
        }

        if (args.contains("zIndex"))
            component.zIndex = args.get<float32>("zIndex");
        if (args.contains("renderingProperties"))
            component.renderingProperties = args.get<UIRenderingProperties>("renderingProperties");
        if (args.contains("hoveredRenderingProperties"))
            component.hoveredRenderingProperties = args.get<UIRenderingProperties>("hoveredRenderingProperties");
        else
            component.hoveredRenderingProperties = component.renderingProperties;

        return component;
    }

} // namespace otto
