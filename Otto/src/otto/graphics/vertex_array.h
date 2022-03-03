#pragma once

#include "otto/base.h"
#include "otto/util/static_array.h"
#include "otto/util/dynamic_array.h"
#include "otto/graphics/vertex_buffer.h"

namespace otto
{
    class VertexArray
    {
    public:
        VertexArray();
        VertexArray(const DynamicArray<VertexBuffer> vertexBuffers, const uint32* indices, uint32 size);
        VertexArray(const VertexArray& other);

        template<uint64 N>
        VertexArray(const DynamicArray<VertexBuffer> vertexBuffers, const StaticArray<int32, N> indices)
            : VertexArray(vertexBuffers, indices.getData(), uint32(N))
        {
        }

        ~VertexArray();

        VertexArray& operator=(const VertexArray& other);

        void bind() const;
        void unbind() const;

        uint32 getSize() const;

    private:
        uint32 mSize;
        uint64* mNCopies;
        uint32 mOpenglHandle;
        uint32 mIndexBufferOpenglHandle;
    };

} // namespace otto
