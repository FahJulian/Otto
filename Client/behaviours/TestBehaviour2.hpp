#include <otto.h>

using namespace otto;

class TestBehaviour2 : public Behaviour
{
public:
    void onEvent(const InitEvent& e)
    {
    }

    void onUpdate(const UpdateEvent& e)        
    {
    }
    
private:
    float32 var;
};
