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

        operator const String&() const
        {
            return toString();
        }

        const String& toString() const
        {
            return mFilePath;
        }

        FilePath getParentDirectory() const;

    private:
        String mFilePath;
    };

    std::ostream& operator<<(std::ostream& stream, const FilePath& filePath);

} // namespace otto
