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
        VertexArray(const uint32* indices, uint32 size, const DynamicArray<VertexBuffer>& vertexBuffers);
        VertexArray(const DynamicArray<uint32>& indices, const DynamicArray<VertexBuffer>& vertexBuffers);
        VertexArray(const VertexArray& other);

        template<uint64 N>
        VertexArray(const StaticArray<uint32, N> indices, const DynamicArray<VertexBuffer> vertexBuffers)
            : VertexArray(indices.getData(), uint32(N), vertexBuffers)
        {
        }

        ~VertexArray();

        VertexArray& operator=(const VertexArray& other);

        void bind() const;

        uint32 getSize() const;

    private:
        uint32 mSize;
        uint64* mNCopies;
        uint32 mOpenglHandle;
        uint32 mIndexBufferOpenglHandle;
    };

} // namespace otto
