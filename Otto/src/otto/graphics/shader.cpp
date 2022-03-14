#include "shader.h"

#include <glew/glew.h>

#include "otto/util/map.h"
#include "otto/util/file.h"
#include "otto/math/math.h"
#include "otto/debug/log.h"
#include "otto/util/optional.h"

namespace otto
{
    namespace
    {
        GLenum _toShaderType(const String& type)
        {
            if (type.equalsIgnoreCase("vertex"))
                return GL_VERTEX_SHADER;
            else if (type.equalsIgnoreCase("fragment"))
                return GL_FRAGMENT_SHADER;

            return 0;
        }

        Map<GLenum, String> _preProcess(const String& source)
        {
            Map<GLenum, String> shaderSources;

            uint64 typeIndex = min(source.findFirstOfIgnoreCase("#type"), source.findFirstOfIgnoreCase("# type"));
            while (typeIndex != source.getSize())
            {
                uint64 nextTypeIndex = min(source.findFirstOfIgnoreCase("#type", typeIndex + 1), source.findFirstOfIgnoreCase("# type", typeIndex + 1));
                GLenum type = _toShaderType(String::subString(source, typeIndex + strlen("# type"), source.findFirstOf('\n', typeIndex)).trim());

                if (type != 0)
                    shaderSources.insert(type, String::subString(source, source.findFirstOf('\n', typeIndex) + 1, nextTypeIndex));

                typeIndex = nextTypeIndex;
            }

            return shaderSources;
        }

        Optional<String> _getErrors(uint32 programHandle, DynamicArray<uint32> shaderHandles)
        {
            int32 info;

            for (uint32 handle : shaderHandles)
            {
                glGetShaderiv(handle, GL_COMPILE_STATUS, &info);
                if (info == GL_FALSE)
                {
                    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &info);

                    String infoLog = String(uint64(info));
                    glGetShaderInfoLog(handle, info, NULL, infoLog.getData());

                    return infoLog;
                }
            }

            glGetProgramiv(programHandle, GL_LINK_STATUS, &info);
            if (info == GL_FALSE)
            {
                glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &info);

                String infoLog = String(uint64(info));
                glGetProgramInfoLog(programHandle, info, NULL, infoLog.getData());

                return infoLog;
            }

            return NULLOPT;
        }

        Optional<String> _compileShaderAndGetErrors(uint32 programHandle, Map<GLenum, String> shaderSources)
        {
            DynamicArray<uint32> shaderHandles;

            for (auto& [type, source] : shaderSources)
            {
                const char8* shaderSourceData = source.getData();
                int32 shaderSourceLength = int32(source.getSize());

                uint32 handle = glCreateShader(type);
                glShaderSource(handle, 1, &shaderSourceData, &shaderSourceLength);
                glCompileShader(handle);    
                glAttachShader(programHandle, handle);

                shaderHandles.add(handle);
            }

            glLinkProgram(programHandle);

            return _getErrors(programHandle, shaderHandles);
        }

    } // namespace

    Shader::Shader()
        : mOpenglHandle(0), mNCopies(nullptr)
    {
    }

    Shader::Shader(const FilePath& filePath)
        : mOpenglHandle(0), mNCopies(new uint64(1))
    {
        String source = File(filePath).read();
        Map<GLenum, String> shaderSources = _preProcess(source);

        mOpenglHandle = glCreateProgram();
        auto error = _compileShaderAndGetErrors(mOpenglHandle, shaderSources);

        if (error.hasValue())
            Log::error("Error compiling shader ", filePath, ":\n", error.getValue());
    }

    Shader::Shader(const Shader& other)
        : mOpenglHandle(0), mNCopies(nullptr)
    {
        *this = other;
    }

    Shader::~Shader()
    {
        if (mNCopies == nullptr)
            return;

        (*mNCopies)--;
        if (*mNCopies == 0)
        {
            delete mNCopies;

            glDeleteProgram(mOpenglHandle);
        }
    }

    Shader& Shader::operator=(const Shader& other)
    {
        if (mOpenglHandle == other.mOpenglHandle)
            return *this;

        this->~Shader();

        mOpenglHandle = other.mOpenglHandle;

        mNCopies = other.mNCopies;
        if (mNCopies != nullptr)
            (*mNCopies)++;

        return *this;
    }

    void Shader::bind() const
    {
        glUseProgram(mOpenglHandle);
    }

    template<> 
    void Shader::setUniform<int32>(const String& name, int32 value)
    {
        GLint location = glGetUniformLocation(mOpenglHandle, name.getData());
        glUniform1i(location, value);
    }

    template<> 
    void Shader::setUniform<float32>(const String& name, float32 value)
    {
        GLint location = glGetUniformLocation(mOpenglHandle, name.getData());
        glUniform1f(location, value);
    }

    template<> 
    void Shader::setUniform<Vec2<float32>>(const String& name, const Vec2<float32>& value)
    {
        GLint location = glGetUniformLocation(mOpenglHandle, name.getData());
        glUniform2f(location, value.x, value.y);
    }

    template<> 
    void Shader::setUniform<Vec3<float32>>(const String& name, const Vec3<float32>& value)
    {
        GLint location = glGetUniformLocation(mOpenglHandle, name.getData());
        glUniform3f(location, value.x, value.y, value.z);
    }

    template<> 
    void Shader::setUniform<Vec4<float32>>(const String& name, const Vec4<float32>& value)
    {
        GLint location = glGetUniformLocation(mOpenglHandle, name.getData());
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

    template<> 
    void Shader::setUniform<Mat3x3<float32>>(const String& name, const Mat3x3<float32>& value)
    {
        GLint location = glGetUniformLocation(mOpenglHandle, name.getData());
        glUniformMatrix3fv(location, 1, GL_FALSE, value.getData());
    }

    template<>
    void Shader::setUniform<Mat4x4<float32>>(const String& name, const Mat4x4<float32>& value)
    {
        GLint location = glGetUniformLocation(mOpenglHandle, name.getData());
        glUniformMatrix4fv(location, 1, GL_FALSE, value.getData());
    }

    void Shader::setUniform(const String& name, const int32* values, uint32 amount)
    {
        GLint location = glGetUniformLocation(mOpenglHandle, name.getData());
        glUniform1iv(location, amount, values);
    }

} // namespace otto
