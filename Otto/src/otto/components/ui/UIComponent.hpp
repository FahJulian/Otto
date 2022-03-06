#pragma once

#include "otto/base.h"
#include "otto/scene/entity.h"
#include "otto/systems/ui/UISize.h"
#include "otto/scene/serialization.h"
#include "otto/event/event_listener.h"
#include "otto/events/ui/UIClickedEvent.hpp"
#include "otto/systems/ui/UIRenderingProperties.h"

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

        UIRenderingProperties renderingProperties;

        EventListener<UIClickedEvent> clickListener;
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

        return component;
    }

} // namespace otto
