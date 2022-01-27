#pragma once

#include "otto/base.h"
#include "otto/util/string.h"

namespace otto
{
    class FilePath
    {
    public:
        FilePath() = default;
        FilePath(const String& path);
        FilePath(String&& path) noexcept;
        FilePath(const char* path);

        const String& toString() const
        {
            return mFilePath;
        }

        FilePath getParentDirectory() const;

        friend FilePath operator+(const FilePath& filePath1, const FilePath& filePath2) 
        {
            return FilePath(filePath1.mFilePath + filePath2.mFilePath);
        }

        bool operator==(const FilePath& other) const { return mFilePath == other.mFilePath; }

    private:
        String mFilePath;
    };

    std::ostream& operator<<(std::ostream& stream, const FilePath& filePath);

} // namespace otto
