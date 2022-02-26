#pragma once

#include "otto/base.h"
#include "otto/window/input/keys.h"
#include "otto/window/input/mouse_buttons.h"

namespace otto
{
    struct _KeyPressedEvent
    {
        Key key;
        char16 character;

        _KeyPressedEvent(Key key, char16 character)
            : key(key), character(character)
        {
        }
    };

    struct _KeyReleasedEvent
    {
        Key key;

        _KeyReleasedEvent(Key key)
            : key(key)
        {
        }
    };

    struct _MouseMovedEvent
    {
        float32 x;
        float32 y;
        float32 beforeX;
        float32 beforeY;
        float32 deltaX;
        float32 deltaY;

        _MouseMovedEvent(float32 x, float32 y, float32 beforeX, float32 beforeY, float32 deltaX, float32 deltaY)
            : x(x), y(y), beforeX(beforeX), beforeY(beforeY), deltaX(deltaX), deltaY(deltaY)
        {
        }
    };

    struct _MouseDraggedEvent
    {
        MouseButton button;

        float32 x;
        float32 y;
        float32 beforeX;
        float32 beforeY;
        float32 deltaX;
        float32 deltaY;

        _MouseDraggedEvent(MouseButton button, float32 x, float32 y, float32 beforeX, float32 beforeY, float32 deltaX, float32 deltaY)
            : button(button), x(x), y(y), beforeX(beforeX), beforeY(beforeY), deltaX(deltaX), deltaY(deltaY)
        {
        }
    };

    struct _MouseButtonPressedEvent
    {
        MouseButton button;

        float32 x;
        float32 y;

        _MouseButtonPressedEvent(MouseButton button, float32 x, float32 y)
            : button(button), x(x), y(y)
        {
        }
    };

    struct _MouseButtonReleasedEvent
    {
        MouseButton button;

        float32 x;
        float32 y;

        _MouseButtonReleasedEvent(MouseButton button, float32 x, float32 y)
            : button(button), x(x), y(y)
        {
        }
    };

    struct _MouseScrolledEvent
    {
        float32 x;
        float32 y;
        float32 scrollX;
        float32 scrollY;

        _MouseScrolledEvent(float32 x, float32 y, float32 scrollX, float32 scrollY)
            : x(x), y(y), scrollX(scrollX), scrollY(scrollY)
        {
        }
    };

    struct _WindowClosedEvent
    {
    };

    struct _WindowResizedEvent
    {
        float32 width;
        float32 height;

        _WindowResizedEvent(float32 width, float32 height)
            : width(width), height(height)
        {
        }
    };

    struct _WindowGainedFocusEvent
    {
    };

    struct _WindowLostFocusEvent
    {
    };

} // namespace otto
