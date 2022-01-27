#pragma once

#include "otto/base.h"
#include "otto/scene/scene.h"
#include "otto/util/result.h"
#include "otto/util/dynamic_array.h"
#include "otto/util/platform/file_path.h"
#include "otto/serialization/serializer.h"

namespace otto
{
    class SceneLoader
    {
        SceneLoader() = delete;
        SceneLoader(const SceneLoader& other) = delete;

    private:
        static void init(const DynamicArray<String>& components, const DynamicArray<String>& behaviours,
            const DynamicArray<String>& systems, const DynamicArray<String>& events);

        static void reloadDll();

        static Result<Scene*, Serializer::ParsingError> loadScene(const FilePath& filePath);

        static void _createSceneFile(const FilePath& filePath);

        friend class Application;
    };

} // namespace otto
