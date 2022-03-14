#pragma once

#include "otto/base.h"
#include "otto/math/vec2.h"
#include "otto/math/vec3.h"
#include "otto/math/vec4.h"
#include "otto/math/mat3x3.h"
#include "otto/math/mat4x4.h"
#include "otto/util/file_path.h"
#include "otto/util/static_array.h"

namespace otto
{
    class Shader
    {
    public:
        Shader();
        Shader(const FilePath& filePath);
        Shader(const Shader& other);

        ~Shader();

        Shader& operator=(const Shader& other);

        void bind() const;

        template<typename T>
        void setUniform(const String& name, T value) requires isIntegral<T>
        {
            static_assert(false);
        }

        template<typename T>
        void setUniform(const String& name, const T& value) requires isNotIntegral<T>
        {
            static_assert(false);
        }

        template<> void setUniform<int32>(const String& name, int32 value);
        template<> void setUniform<float32>(const String& name, float32 value);
        template<> void setUniform<Vec2<float32>>(const String& name, const Vec2<float32>& value);
        template<> void setUniform<Vec3<float32>>(const String& name, const Vec3<float32>& value);
        template<> void setUniform<Vec4<float32>>(const String& name, const Vec4<float32>& value);
        template<> void setUniform<Mat3x3<float32>>(const String& name, const Mat3x3<float32>& value);
        template<> void setUniform<Mat4x4<float32>>(const String& name, const Mat4x4<float32>& value);

        template<uint64 N>
        void setUniform(const String& name, const StaticArray<int32, N>& values)
        {
            setUniform(name, values.getData(), uint32(N));
        }

        void setUniform(const String& name, const int32* values, uint32 amount);

    private:
        uint32 mOpenglHandle;
        uint64* mNCopies;
    };

} // namespace otto
