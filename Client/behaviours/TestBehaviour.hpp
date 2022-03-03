#include <otto.h>
#include "events/TestEvent.hpp"

using namespace otto;

class TestBehaviour : public Behaviour
{
public:
    void onEvent(const InitEvent& e)
    {
        Log::info("Initializing TestBehaviour");
    }

    void onEvent(const UpdateEvent& e)   
    {
    }

    void onEvent(const TestEvent & e)
    {

    }

private:
    float32 var;
};
