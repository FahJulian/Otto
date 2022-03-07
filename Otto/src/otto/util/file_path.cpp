#include "file_path.h"

#include "otto/core/application.h"

namespace otto
{
    namespace
    {
        void _putDirectoriesWithSpacesInQuotes(String& path)
        {
            uint64 index = 0;
            while ((index = path.findFirstOf(' ', index)) != path.getSize())
            {
                if (path.findLastOf('\\', index) != path.findLastOf("\\\"", index))
                    path.replaceLast("\\", "\\\"", index);
                
                uint64 i = path.findFirstOf("\"\\", index);
                index = path.findFirstOf('\\', index);
                if (i + 1 != index)
                    path.replace(index, index + 1, "\"\\");
            }
        }

    } //namespace

#ifdef OTTO_WINDOWS

    FilePath::FilePath(const String& path)
        : mFilePath(path)
    {
        mFilePath.replaceAll('/', '\\').trim();
        _putDirectoriesWithSpacesInQuotes(mFilePath);
    }

    FilePath::FilePath(String&& path) noexcept
        : mFilePath(std::forward<String>(path))
    {
        mFilePath.replaceAll('/', '\\').trim();
        _putDirectoriesWithSpacesInQuotes(mFilePath);
    }

    FilePath::FilePath(const char* path)
        : mFilePath(path)
    {
        mFilePath.replaceAll('/', '\\').trim();
        _putDirectoriesWithSpacesInQuotes(mFilePath);
    }

    FilePath FilePath::getParentDirectory() const
    {
        return String::subString(mFilePath, 0, mFilePath.findLastOf('\\', mFilePath.getSize() - 1) + 1);
    }

#endif

    FilePath FilePath::getResourceFilePath(const FilePath& filePath)
    {
        return filePath.toString().startsWith(FilePath("otto/").toString()) ?
            Application::getCoreRootDirectory() + "res/" + filePath : Application::getRootDirectory() + filePath;
    }

    std::ostream& operator<<(std::ostream& stream, const FilePath& filePath)
    {
        return stream << filePath.toString();
    }

} // namespace otto
