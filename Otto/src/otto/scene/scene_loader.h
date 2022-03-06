#pragma once

#include "otto/base.h"
#include "otto/util/result.h"
#include "otto/util/shared.h"
#include "otto/scene/scene.h"
#include "otto/core/package.h"
#include "otto/util/platform/file_path.h"

namespace otto
{
    class SceneLoader
    {
    private:
        enum SceneLoadingError
        {
            SYNTAX_ERROR,
            DUPLICATE_ENTITY,
        };

        SceneLoader() = delete;
        SceneLoader(const SceneLoader& other) = delete;

#ifdef OTTO_DYNAMIC
        static bool8 reloadDll(const Package& package);

        static void initClient(Application* mainApplication, Window* mainWindow, Log* mainLog, const Color& clearColor);
#endif

        static Result<Shared<Scene>, SceneLoadingError> loadScene(const FilePath& filePath);

        friend class Application;
        friend class SceneManager;
    };
}
