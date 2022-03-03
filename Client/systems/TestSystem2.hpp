#include <otto.h>

using namespace otto;

class TestSystem2 : public System
{
public:
    void onEvent(const InitEvent& e)
    {
        Log::info("Initializing TestSystem2");
    }

    void onEvent(const UpdateEvent& e)      
    {
    }
};
