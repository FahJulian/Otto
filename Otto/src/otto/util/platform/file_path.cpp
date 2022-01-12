#include "file_path.h"

namespace otto
{

#ifdef OTTO_WINDOWS

    FilePath::FilePath(const String& path)
        : mFilePath(path)
    {
        mFilePath.replaceAll('/', '\\');
    }

    FilePath::FilePath(String&& path) noexcept
        : mFilePath(std::forward<String>(path))
    {
        mFilePath.replaceAll('/', '\\');
    }

    FilePath::FilePath(const char* path)
        : mFilePath(path)
    {
        mFilePath.replaceAll('/', '\\');
    }

    FilePath FilePath::getParentDirectory() const
    {
        return String::subString(mFilePath, 0, mFilePath.findLastOf('\\', mFilePath.getSize() - 1) + 1);
    }

#endif

    std::ostream& operator<<(std::ostream& stream, const FilePath& filePath)
    {
        return stream << filePath.toString() << std::endl;
    }

} // namespace otto
