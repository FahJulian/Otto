#pragma once

#include "otto/base.h"
#include "otto/util/optional.h"

namespace otto
{
    class Context
    {
    private:
        Context() = delete;
        Context(const Context& other) = delete;

        static bool init(ptr64 windowHandle);

        static void swapBuffers();

        friend class Window;
    };

} // namespace otto
