//#include <otto.h>
#include <otto/core/application.h>

int main()
{
    if (!otto::Application::init("C:/dev/Otto/Client/AppSettings.otto"))
        return -1;      

    otto::Application::run();

    otto::Application::destroy();
}
