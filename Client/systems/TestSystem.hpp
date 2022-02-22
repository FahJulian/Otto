#include <otto.h>

#include "events/TestEvent.hpp"
#include "events/TestEvent2.hpp"

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

    void onUpdate(float32 delta)        // OR: float32 delta
    {
        Log::info("Updating TestSystem...");
    }

    void onEvent(const TestEvent& e)
    {

    }

    void onEvent(const TestEvent2& e)
    {

    }

private:
    Shared<Scene> mScene;
    MultiView<TestComponent, TestComponent2>* mView1;
    View<TestComponent>* mView2;
};
