#include "vertex_buffer.h"

#include <glew/glew.h>

namespace otto
{
    VertexBuffer::VertexBuffer()
        : mOpenglHandle(0), mNCopies(nullptr), mStride(0)
    {
    }

    VertexBuffer::VertexBuffer(uint64 size, const DynamicArray<BufferElement>& layout)
        : mOpenglHandle(0), mNCopies(new uint64(1)), mLayout(layout), mStride(0)
    {
        _calculateOffsetAndStride();

        glCreateBuffers(1, &mOpenglHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mOpenglHandle);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::VertexBuffer(const void* data, uint64 size, const DynamicArray<BufferElement>& layout)
        : mOpenglHandle(0), mNCopies(new uint64(1)), mLayout(layout), mStride(0)
    {
        _calculateOffsetAndStride();

        glCreateBuffers(1, &mOpenglHandle);
        glBindBuffer(GL_ARRAY_BUFFER, mOpenglHandle);
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    }

    VertexBuffer::VertexBuffer(const VertexBuffer& other)
        : mOpenglHandle(0), mNCopies(nullptr), mStride(0)
    {
        *this = other;
    }

    VertexBuffer::~VertexBuffer()
    {
        if (mNCopies == nullptr)
            return;

        (*mNCopies)--;
        if (*mNCopies == 0)
        {
            delete mNCopies;
            glDeleteBuffers(1, &mOpenglHandle);
        }
    }

    VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other)
    {
        if (mOpenglHandle == other.mOpenglHandle)
            return *this;

        this->~VertexBuffer();

        mStride = other.mStride;
        mOpenglHandle = other.mOpenglHandle;
        mLayout = other.mLayout;

        mNCopies = other.mNCopies;
        if (mNCopies != nullptr)
            (*mNCopies)++;

        return *this;
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, mOpenglHandle);
    }

    void VertexBuffer::setData(const void* data, uint64 size)
    {
        glBindBuffer(GL_ARRAY_BUFFER, mOpenglHandle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
    }

    void VertexBuffer::_calculateOffsetAndStride()
    {
        uint64 offset = 0;
        for (auto& element : mLayout)
        {
            element.offset = offset;
            mStride += element.size;
            offset += element.size;
        }
    }

} // namespace otto
