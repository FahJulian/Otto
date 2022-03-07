#pragma once

#include "otto/base.h"
#include "otto/scene/scene.h"
#include "otto/util/file_path.h"

namespace otto
{
    class SceneManager
    {
    private:
        SceneManager() = delete;
        SceneManager(const SceneManager& other) = delete;
        SceneManager& operator=(const SceneManager& other) = delete;

        static bool8 setScene(const FilePath& scene);

        static Shared<Scene> sCurrentScene;

        friend class Application;
    };

} // namespace otto
