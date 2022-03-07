#include "scene_manager.h"

#include "otto/debug/log.h"
#include "otto/serialization/scene_loader.h"

namespace otto
{
    Shared<Scene> SceneManager::sCurrentScene = nullptr;

    bool8 SceneManager::setScene(const FilePath& filePath)
    {
        Log::trace("Setting scene to ", filePath);

        auto result = SceneLoader::loadScene(filePath);
        if (result.hasError())
        {
            Log::error("Failed to set Scene to ", filePath);
            return false;
        }
        
        sCurrentScene = result.getResult();
        return true;
    }

} // namespace otto
