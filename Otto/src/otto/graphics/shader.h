#pragma once

#include "otto/base.h"
#include "otto/math/vec2.h"
#include "otto/math/vec3.h"
#include "otto/math/vec4.h"
#include "otto/math/mat4x4.h"
#include "otto/util/static_array.h"
#include "otto/util/platform/file_path.h"

namespace otto
{
    class Shader
    {
        template<typename T>
        static constexpr bool _isNotIntegral = !std::is_integral_v<T>;

    public:
        Shader();
        Shader(const FilePath& filePath);
        Shader(const Shader& other);

        ~Shader();

        Shader& operator=(const Shader& other);

        void bind() const;
        void unbind() const;

        template<typename T>
        void setUniform(const String& name, T value)
        {
            static_assert(false);
        }

        template<> void setUniform<int32>(const String& name, int32 value);
        template<> void setUniform<float32>(const String& name, float32 value);
        template<> void setUniform<const Vec2f32&>(const String& name, const Vec2f32& value);
        template<> void setUniform<Vec3f32>(const String& name, Vec3f32 value);
        template<> void setUniform<Vec4f32>(const String& name, Vec4f32 value);
        template<> void setUniform<Mat4x4f32>(const String& name, Mat4x4f32 value);

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
