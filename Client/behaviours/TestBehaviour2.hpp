#include <otto.h>

using namespace otto;

class TestBehaviour2 : public Behaviour
{
public:
    void onInit()
    {
        Log::info("Initializing TestBehaviour2");
    }

    void onUpdate(float32 delta)        
    {
    }

    void onDestroy()
    {

    }
    
private:
    float32 var;
};
