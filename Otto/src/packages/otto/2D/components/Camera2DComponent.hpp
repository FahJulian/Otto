#pragma once

#include "otto.h"

#include "packages/otto/2D/Camera2D.h"

namespace otto
{
    struct Camera2DComponent
    {
        Camera2D camera;
        bool8 isActive = true;
    };

    template<>
    Camera2DComponent deserializeComponent(const Serialized& args, const Map<String, Entity>& entities)
    {
        Camera2DComponent component;

        component.camera = args.get<Camera2D>("camera");
        if (args.contains("isActive"))
            component.isActive = args.get<bool8>("isActive");

        return component;
    }

} // namespace otto

