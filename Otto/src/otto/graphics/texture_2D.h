#pragma once

#include "otto/base.h"
#include "otto/util/platform/file_path.h"

namespace otto
{
    class Texture2D
    {
    public:
        Texture2D();
        Texture2D(const FilePath& filePath);
        Texture2D(const Texture2D& other);

        ~Texture2D();

        Texture2D& operator=(const Texture2D& other);

        bool operator==(const Texture2D& other) { return mOpenglHandle == other.mOpenglHandle; }

        bool isValid() { return mOpenglHandle != 0; }

        uint32 getWidth() const { return mWidth; }
        uint32 getHeight() const { return mHeight; }

        void bind(uint32 slot = 0);

    private:
        uint32 mWidth;
        uint32 mHeight;
        uint32 mOpenglHandle;
        uint64* mNCopies;
    };

} // namespace otto
