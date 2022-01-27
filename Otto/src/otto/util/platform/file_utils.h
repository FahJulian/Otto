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
        static bool directoryExists(const FilePath& directoryPath);
        static bool fileExists(const FilePath& filePath);

        static bool createDirectory(const FilePath& directory);
        static bool createDirectoryRecursively(const FilePath& directory);

        static bool moveFile(const FilePath& oldName, const FilePath& newName);
        static bool renameFile(const FilePath& oldName, const FilePath& newName);

        static bool deleteFile(const FilePath& fileName);
        static bool deleteEmptyDirectory(const FilePath& directoryPath);
        static bool deleteEmptyDirectoryRecursively(const FilePath& directoryPath);
    };

} // namespace otto::platform
