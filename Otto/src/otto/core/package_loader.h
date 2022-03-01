#pragma once

#include "otto/base.h"
#include "otto/util/result.h"
#include "otto/core/package.h"
#include "otto/serialization/serialized.h"
#include "otto/util/platform/file_path.h"

namespace otto
{
    class PackageLoader
    {
    public:
        enum PackageLoadingError
        {
            SYNTAX_ERROR,
            ROOT_DIRECTORY_NOT_FOUND,
        };

        PackageLoader() = delete;
        PackageLoader(const PackageLoader& other) = delete;

        static Result<Package, PackageLoadingError> loadPackage(const Serialized& serialized, const FilePath& coreRootDirectory);
    };

} // namespace otto
