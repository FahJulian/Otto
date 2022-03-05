#pragma once

#include "otto/base.h"
#include "otto/graphics/color.h"
#include "otto/graphics/vertex_array.h"

namespace otto
{
    class GraphicsAPI
    {
    private:
        GraphicsAPI() = delete;
        GraphicsAPI(const GraphicsAPI& other) = delete;

        static bool init();

        static void setClearColor(const Color& color);

        static void clear();

    public:
        static void setViewport(uint32 x, uint32 y, uint32 width, uint32 height);

        static void drawIndexed(const VertexArray& vertexArray, uint32 indexCount = 0);

        friend class Window;
    };

} // namespace otto
