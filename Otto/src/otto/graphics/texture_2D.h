#pragma once

#include "otto/base.h"
#include "otto/util/file_path.h"

namespace otto
{
    class Texture2D
    {
    public:
        enum class Filter
        {
            NEAREST,
            LINEAR,
        };

        enum class Wrap
        {
            REPEAT,
        };

        Texture2D();
        Texture2D(const FilePath& filePath, Filter filter = Filter::LINEAR, Wrap wrap = Wrap::REPEAT);
        Texture2D(const Texture2D& other);

        ~Texture2D();

        Texture2D& operator=(const Texture2D& other);

        bool8 operator==(const Texture2D& other) { return mOpenglHandle == other.mOpenglHandle; }

        bool8 isValid() const { return mOpenglHandle != 0; }

        float32 getWidth() const { return float32(mWidth); }
        float32 getHeight() const { return float32(mHeight); }

        void bind(uint32 slot = 0);

    private:
        uint32 mWidth;
        uint32 mHeight;
        uint32 mOpenglHandle;
        uint64* mNCopies;

        friend class TextureLoader;
    };

} // namespace otto
