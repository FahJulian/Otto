#include "file_utils.h"

#ifdef OTTO_WINDOWS

#include <Windows.h> 

namespace otto
{
    bool8 FileUtils::directoryExists(const FilePath& directoryPath)
    {
        auto fileAttributes = GetFileAttributesA(directoryPath.toString().getData());
        return fileAttributes != INVALID_FILE_ATTRIBUTES && (fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
    }

    bool8 FileUtils::fileExists(const FilePath& filePath)
    {
        auto fileAttributes = GetFileAttributesA(filePath.toString().getData());
        return fileAttributes != INVALID_FILE_ATTRIBUTES && !(fileAttributes & FILE_ATTRIBUTE_DIRECTORY);
    }

    bool8 FileUtils::createDirectory(const FilePath& directory)
    {
        return CreateDirectoryA(directory.toString().getData(), NULL);
    }

    bool8 FileUtils::moveFile(const FilePath& oldName, const FilePath& newName)
    {
        return MoveFileA(oldName.toString().getData(), newName.toString().getData());
    }

    bool8 FileUtils::renameFile(const FilePath& oldName, const FilePath& newName)
    {
        return moveFile(oldName, newName);
    }

    bool8 FileUtils::createDirectoryRecursively(const FilePath& directory)
    {
        for (uint64 index = directory.toString().findFirstOf('\\', directory.toString().findFirstOf(":\\") + 1), end = directory.toString().getSize();
            index < end; index = directory.toString().findFirstOf('\\', index + 1))
        {
            String path = String::subString(directory.toString(), 0, index + 1);
            if (!directoryExists(path) && !createDirectory(path))
                return false;
        }

        return true;
    }

    bool8 FileUtils::deleteFile(const FilePath& fileName)
    {
        return DeleteFileA(fileName.toString().getData());
    }

    bool8 FileUtils::deleteEmptyDirectory(const FilePath& fileName)
    {
        return RemoveDirectoryA(fileName.toString().getData());
    }

    bool8 FileUtils::deleteEmptyDirectoryRecursively(const FilePath& directory)
    {
        for (uint64 index = directory.toString().findLastOf('\\'), end = directory.toString().findFirstOf(":\\");
            index > end + 1; index = directory.toString().findLastOf('\\', index - 1))
        {
            String path = String::subString(directory.toString(), 0, index + 1);

            if (!directoryExists(path))
                continue;

            if (!deleteEmptyDirectory(path))
                return false;
        }

        return true;
    }
#endif

} // namespace otto
