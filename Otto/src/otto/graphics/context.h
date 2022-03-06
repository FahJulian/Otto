#pragma once

#include "otto/base.h"

namespace otto
{
    class Context
    {
    private:
        Context() = delete;
        Context(const Context& other) = delete;

        static bool8 init(ptr64 windowHandle);

        static void swapBuffers();

        friend class Window;
    };

} // namespace otto
