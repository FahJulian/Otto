#pragma once

#include "otto/base.h"
#include "otto/util/platform/file_path.h"

namespace otto
{
    class FileUtils
    {
        FileUtils() = delete;
        FileUtils(const FileUtils& other) = delete;

    public:
        static bool8 directoryExists(const FilePath& directoryPath);
        static bool8 fileExists(const FilePath& filePath);

        static bool8 createDirectory(const FilePath& directory);
        static bool8 createDirectoryRecursively(const FilePath& directory);

        static bool8 moveFile(const FilePath& oldName, const FilePath& newName);
        static bool8 renameFile(const FilePath& oldName, const FilePath& newName);

        static bool8 deleteFile(const FilePath& fileName);
        static bool8 deleteEmptyDirectory(const FilePath& directoryPath);
        static bool8 deleteEmptyDirectoryRecursively(const FilePath& directoryPath);
    };

} // namespace otto::platform
