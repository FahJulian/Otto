#include <otto.h>
#include "events/TestEvent.hpp"

using namespace otto;

class TestBehaviour : public Behaviour
{
public:
    void onInit()
    {
        Log::info("Initializing TestBehaviour");
    }

    void onUpdate(float32 delta)   
    {
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
