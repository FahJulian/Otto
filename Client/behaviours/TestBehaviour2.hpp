#include <otto.h>

using namespace otto;

class TestBehaviour2 : public Behaviour
{
public:
    void onInit()
    {

    }

    void onUpdate(float32 delta)        // OPTIONAL: onUpdate(float64 delta)
    {

    }

    void onDestroy()
    {

    }
    
private:
    float32 var;
};
