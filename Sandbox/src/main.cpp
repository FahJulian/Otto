#include "otto/base.h"
#include "otto/scene/scene_loader.h"
#include "otto/core/application.h"

int main()
{
    if (!otto::Application::init("C:/dev/Otto/Client/app_settings_concept.otto"))
        return -1;

    auto scene = otto::SceneLoader::loadScene(otto::Application::getRootDirectory() + "scenes/scene_concept.otto");
    scene.getResult()->init();
    scene.getResult()->update(3.0f);
}
