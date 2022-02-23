#include "scene_manager.h"

#include "otto/scene/scene_loader.h"

namespace otto
{
    Shared<Scene> SceneManager::sCurrentScene = nullptr;

    bool SceneManager::setScene(const FilePath& filePath)
    {
        auto result = SceneLoader::loadScene(filePath);
        if (result.hasError())
            return false;
        
        sCurrentScene = result.getResult();
        return true;
    }

} // namespace otto
