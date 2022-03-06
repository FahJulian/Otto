#pragma once

#include "otto/base.h"
#include "otto/math/vec2.h"
#include "otto/math/vec3.h"
#include "otto/math/vec4.h"
#include "otto/util/static_array.h"
#include "otto/util/dynamic_array.h"

#define GL_FLOAT 0x1406
#define GL_INT 0x1404

namespace otto
{
    struct BufferElement
    {
    private:
        template<typename T>
        static constexpr bool8 _isShaderType = false;

        template<> static constexpr bool8 _isShaderType<float32> = true;
        template<> static constexpr bool8 _isShaderType<Vec2f32> = true;
        template<> static constexpr bool8 _isShaderType<Vec3f32> = true;
        template<> static constexpr bool8 _isShaderType<Vec4f32> = true;
        template<> static constexpr bool8 _isShaderType<int32> = true;
        template<> static constexpr bool8 _isShaderType<Vec2i32> = true;
        template<> static constexpr bool8 _isShaderType<Vec3i32> = true;
        template<> static constexpr bool8 _isShaderType<Vec4i32> = true;

        template<typename T>
        static constexpr uint8 _elementCount = 0;

        template<> static constexpr uint8 _elementCount<float32> = 1;
        template<> static constexpr uint8 _elementCount<Vec2f32> = 2;
        template<> static constexpr uint8 _elementCount<Vec3f32> = 3;
        template<> static constexpr uint8 _elementCount<Vec4f32> = 4;
        template<> static constexpr uint8 _elementCount<int32> = 1;
        template<> static constexpr uint8 _elementCount<Vec2i32> = 2;
        template<> static constexpr uint8 _elementCount<Vec3i32> = 3;
        template<> static constexpr uint8 _elementCount<Vec4i32> = 4;

        template<typename T>
        static constexpr uint32 _openglBaseType = 0;

        template<> static constexpr uint32 _openglBaseType<float32> = GL_FLOAT;
        template<> static constexpr uint32 _openglBaseType<Vec2f32> = GL_FLOAT;
        template<> static constexpr uint32 _openglBaseType<Vec3f32> = GL_FLOAT;
        template<> static constexpr uint32 _openglBaseType<Vec4f32> = GL_FLOAT;
        template<> static constexpr uint32 _openglBaseType<int32> = GL_INT;
        template<> static constexpr uint32 _openglBaseType<Vec2i32> = GL_INT;
        template<> static constexpr uint32 _openglBaseType<Vec3i32> = GL_INT;
        template<> static constexpr uint32 _openglBaseType<Vec4i32> = GL_INT;

    public:
        BufferElement(uint8 size, uint8 elementCount, uint64 offset, uint32 openglBaseType, bool8 normalized)
            : size(size), elementCount(elementCount), offset(offset), openglBaseType(openglBaseType), normalized(normalized)
        {
        }

        template<typename T> requires _isShaderType<T>
        static BufferElement create(bool8 normalized = false)
        {
            return BufferElement(sizeof(T), _elementCount<T>, 0, _openglBaseType<T>, normalized);
        }

        uint8 size;
        uint8 elementCount;
        uint64 offset;
        uint32 openglBaseType;
        bool8 normalized;
    };

    class VertexBuffer
    {
    public:
        VertexBuffer();
        VertexBuffer(uint64 size, const DynamicArray<BufferElement>& layout);
        VertexBuffer(const void* data, uint64 size, const DynamicArray<BufferElement>& layout);
        VertexBuffer(const VertexBuffer& other);

        template<typename T, uint64 N>
        VertexBuffer(const DynamicArray<BufferElement>& layout, const StaticArray<T, N>& data)
            : VertexBuffer(layout, reinterpret_cast<const void*>(data.getData()), N)
        {
        }

        ~VertexBuffer();

        VertexBuffer& operator=(const VertexBuffer& other);

        void bind() const;

        void setData(const void* data, uint64 size);

        template<typename T, uint64 N>
        void setData(const StaticArray<T, N>& data)
        {
            setData(reinterpret_cast<const void*>(data.getData()), N * sizeof(T));
        }

        template<typename T, uint64 N>
        void setData(const StaticArray<T, N>& data, uint64 elements)
        {
            setData(reinterpret_cast<const void*>(data.getData()), elements * sizeof(T));
        }

        template<typename T>
        void setData(const DynamicArray<T>& data)
        {
            setData(reinterpret_cast<const void*>(data.getData()), data.getSize() * sizeof(T));
        }

        template<typename T>
        void setData(const DynamicArray<T>& data, uint64 elements)
        {
            setData(reinterpret_cast<const void*>(data.getData()), elements * sizeof(T));
        }

    private:
        void _calculateOffsetAndStride();

        uint32 mStride;
        uint64* mNCopies;
        uint32 mOpenglHandle;
        DynamicArray<BufferElement> mLayout;

        friend class VertexArray;
    };

} // namespace otto

#undef GL_FLOAT
#undef GL_INT
