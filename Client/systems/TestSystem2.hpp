#include <otto.h>

using namespace otto;

class TestSystem2
{
public:
    void onUpdate(float32 delta)        // OR: float32 delta
    {
        Log::info("Updating TestSystem2...");
    }
};
