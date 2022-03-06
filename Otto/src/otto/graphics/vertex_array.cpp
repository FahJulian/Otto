#include "vertex_array.h"

#include <glew/glew.h>

namespace otto
{
    VertexArray::VertexArray()
        : mSize(0), mNCopies(nullptr), mOpenglHandle(0), mIndexBufferOpenglHandle(0)
    {
        GL_FLOAT;
    }

    VertexArray::VertexArray(const DynamicArray<uint32>& indices, const DynamicArray<VertexBuffer>& vertexBuffers)
        : VertexArray(indices.getData(), uint32(indices.getSize()), vertexBuffers)
    {
    }

    VertexArray::VertexArray(const uint32* indices, uint32 size, const DynamicArray<VertexBuffer>& vertexBuffers)
        : mSize(size), mNCopies(new uint64(1)), mOpenglHandle(0), mIndexBufferOpenglHandle(0)
    {
        glCreateVertexArrays(1, &mOpenglHandle);
        glBindVertexArray(mOpenglHandle);

        glCreateBuffers(1, &mIndexBufferOpenglHandle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBufferOpenglHandle);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(uint32), indices, GL_STATIC_DRAW);

        for (uint64 i = 0; i < vertexBuffers.getSize(); i++)
        {
            vertexBuffers[i].bind();

            for (uint64 j = 0; j < vertexBuffers[i].mLayout.getSize(); j++)
            {
                auto& element = vertexBuffers[i].mLayout[j];

                glEnableVertexAttribArray(uint32(j));
                if (element.openglBaseType == GL_FLOAT)
                {
                    glVertexAttribPointer(uint32(j), element.elementCount, GL_FLOAT, element.normalized ? GL_TRUE : GL_FALSE,
                        vertexBuffers[i].mStride, reinterpret_cast<const void*>(element.offset));
                }
                else
                {
                    glVertexAttribIPointer(uint32(j), element.elementCount, GL_FLOAT,
                        vertexBuffers[i].mStride, reinterpret_cast<const void*>(element.offset));
                }
            }
        }
    }

    VertexArray::VertexArray(const VertexArray& other)
        : mSize(0), mNCopies(nullptr), mOpenglHandle(0), mIndexBufferOpenglHandle(0)
    {
        *this = other;
    }

    VertexArray::~VertexArray()
    {
        if (mNCopies == nullptr)
            return;

        (*mNCopies)--;
        if (*mNCopies == 0)
        {
            delete mNCopies;

            glDeleteVertexArrays(1, &mOpenglHandle);
            glDeleteBuffers(1, &mIndexBufferOpenglHandle);
        }
    }

    VertexArray& VertexArray::operator=(const VertexArray& other)
    {
        if (mOpenglHandle == other.mOpenglHandle)
            return *this;

        this->~VertexArray();

        mSize = other.mSize;
        mOpenglHandle = other.mOpenglHandle;
        mIndexBufferOpenglHandle = other.mIndexBufferOpenglHandle;

        mNCopies = other.mNCopies;
        if (mNCopies != nullptr)
            (*mNCopies)++;

        return *this;
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(mOpenglHandle);
    }

    uint32 VertexArray::getSize() const
    {
        return mSize;
    }

} // namespace otto
