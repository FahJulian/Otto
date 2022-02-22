#include <otto.h>
#include "events/TestEvent.hpp"

using namespace otto;

class TestBehaviour : public Behaviour
{
public:
    void onInit()
    {

    }

    void onUpdate(float32 delta)        // OPTIONAL: onUpdate(float64 delta)
    {
        Log::info("Updating TestBehaviour " + String::valueOf((ptr64)this) + "...");
    }

    void onDestroy()
    {

    }

    void onEvent(const TestEvent & e)
    {

    }

private:
    float32 var;
};
