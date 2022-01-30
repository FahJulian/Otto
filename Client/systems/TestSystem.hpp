#include <otto.h>

// #include "components/TestComponent.hpp"
// #include "components/TestComponent2.hpp"

using namespace otto;

class TestSystem
{
public:
    void onInit(Shared<Scene> scene, MultiView<TestComponent, TestComponent2>* view1, View<TestComponent>* view2)
    {
        mScene = scene;
        mView1 = view1;
        mView2 = view2;
    }

    void onUpdate(float64 delta)        // OR: float32 delta
    {
        
    }

private:
    Shared<Scene> mScene;
    MultiView<TestComponent, TestComponent2>* mView1;
    View<TestComponent>* mView2;
};
