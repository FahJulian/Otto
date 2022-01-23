#pragma once

#include "otto/base.h"
#include "otto/util/string.h"
#include "otto/util/dynamic_array.h"
#include "otto/util/platform/file_path.h"

namespace otto
{
    class File
    {
    public:
        File(const FilePath& filePath);

        String read() const;

        DynamicArray<String> readLines() const;

    private:
        FilePath mFilePath;
    };

} // namespace otto
