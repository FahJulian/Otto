#pragma once

#include "otto/base.h"
#include "otto/util/file_path.h"

namespace otto
{
    class FileUtils
    {
        FileUtils() = delete;
        FileUtils(const FileUtils& other) = delete;

    public:
        static bool8 directoryExists(const FilePath& directorypath);
        static bool8 fileExists(const FilePath& FilePath);

        static bool8 createDirectory(const FilePath& directory);
        static bool8 createDirectoryRecursively(const FilePath& directory);

        static bool8 moveFile(const FilePath& oldname, const FilePath& newname);
        static bool8 renameFile(const FilePath& oldname, const FilePath& newname);

        static bool8 deleteFile(const FilePath& filename);
        static bool8 deleteEmptyDirectory(const FilePath& directorypath);
        static bool8 deleteEmptyDirectoryRecursively(const FilePath& directorypath);
    };

} // namespace otto::platform
