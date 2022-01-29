#include "otto/base.h"
#include "otto/scene/scene_loader.h"
#include "otto/core/application.h"

int main()
{
    //otto::Time::init();
    //otto::Log::init("C:/dev/Otto/Sandbox/.log/main.log", std::cout, otto::Log::ALL, otto::Log::ALL);

    if (!otto::Application::init("C:/dev/Otto/Client/app_settings_concept.otto"))
        return -1;

    auto scene = otto::SceneLoader::loadScene(otto::Application::getRootDirectory() + "scenes/scene_concept.otto");
}
