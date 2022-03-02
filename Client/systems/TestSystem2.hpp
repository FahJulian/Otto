#include <otto.h>

using namespace otto;

class TestSystem2 : public System
{
public:
    void onInit()
    {
        Log::info("Initializing TestSystem2");
    }

    void onUpdate(float32 delta)      
    {
    }
};
