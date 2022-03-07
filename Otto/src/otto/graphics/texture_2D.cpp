#include "texture_2D.h"

#include <glew/glew.h>
#include <stb/stb_image.h>

namespace otto
{
    namespace
    {
        GLint _toGLint(Texture2D::Filter filter)
        {
            switch (filter)
            {
            case Texture2D::Filter::LINEAR: return GL_LINEAR;
            case Texture2D::Filter::NEAREST: return GL_NEAREST;
            }

            return 0;
        }

        GLint _toGLint(Texture2D::Wrap wrap)
        {
            switch (wrap)
            {
            case Texture2D::Wrap::REPEAT: return GL_REPEAT;
            }

            return 0;
        }

    } // namespace 

    Texture2D::Texture2D()
        : mWidth(0), mHeight(0), mOpenglHandle(0), mNCopies(nullptr)
    {
    }

    Texture2D::Texture2D(const FilePath& filePath, Texture2D::Filter filter, Texture2D::Wrap wrap)
        : mWidth(0), mHeight(0), mOpenglHandle(0), mNCopies(new uint64(1))
    {
        stbi_set_flip_vertically_on_load(1);

        int32 width, height, channels;
        uint8* data = stbi_load(filePath.toString().getData(), &width, &height, &channels, 0);

        if (data)
        {
            mWidth = width;
            mHeight = height;

            GLenum internalFormat = 0;
            GLenum dataFormat = 0;

            if (channels == 4)
            {
                internalFormat = GL_RGBA8;
                dataFormat = GL_RGBA;
            }
            else if (channels == 3)
            {
                internalFormat = GL_RGB8;
                dataFormat = GL_RGB;
            }

            glCreateTextures(GL_TEXTURE_2D, 1, &mOpenglHandle);
            glTextureStorage2D(mOpenglHandle, 1, internalFormat, mWidth, mHeight);

            glTextureParameteri(mOpenglHandle, GL_TEXTURE_MIN_FILTER, _toGLint(filter));
            glTextureParameteri(mOpenglHandle, GL_TEXTURE_MAG_FILTER, _toGLint(filter));

            glTextureParameteri(mOpenglHandle, GL_TEXTURE_WRAP_S, _toGLint(wrap));
            glTextureParameteri(mOpenglHandle, GL_TEXTURE_WRAP_T, _toGLint(wrap));

            glTextureSubImage2D(mOpenglHandle, 0, 0, 0, mWidth, mHeight, dataFormat, GL_UNSIGNED_BYTE, data);

            stbi_image_free(data);
        }
    }

    Texture2D::Texture2D(const Texture2D& other)
        : mWidth(0), mHeight(0), mOpenglHandle(0), mNCopies(nullptr)
    {
        *this = other;
    }

    Texture2D::~Texture2D()
    {
        if (mNCopies == nullptr)
            return;

        (*mNCopies)--;
        if (*mNCopies == 0)
        {
            delete mNCopies;

            glDeleteTextures(1, &mOpenglHandle);
        }
    }

    Texture2D& Texture2D::operator=(const Texture2D& other)
    {
        if (mOpenglHandle == other.mOpenglHandle)
            return *this;

        this->~Texture2D();

        mWidth = other.mWidth;
        mHeight = other.mHeight;
        mOpenglHandle = other.mOpenglHandle;

        mNCopies = other.mNCopies;
        if (mNCopies != nullptr)
            (*mNCopies)++;

        return *this;
    }

    void Texture2D::bind(uint32 slot)
    {
        glBindTextureUnit(slot, mOpenglHandle);
    }

} // namespace otto
