#include <otto.h>

#include "events/TestEvent.hpp"
#include "events/TestEvent2.hpp"
// #include "otto/components/TransformComponent.hpp"

using namespace otto;

class TestSystem
{
public:
    void onInit(Shared<Scene> scene, MultiView<TestComponent, TransformComponent>* view1, View<TestComponent>* view2, View<TransformComponent>* view3)
    {
        Log::info("Initializing TestSystem");
        mScene = scene;
        mView1 = view1;
        mView2 = view2;
        mView3 = view3;
    }

    void onUpdate(float32 delta)       
    {
    }

    void onEvent(const TestEvent& e)
    {
        Log::info("TestSystem::onTestEvent");
    }

    void onEvent(const TestEvent2& e)
    {

    }

private:
    Shared<Scene> mScene;
    MultiView<TestComponent, TransformComponent>* mView1;
    View<TestComponent>* mView2;
    View<TransformComponent>* mView3;
};
