#include <otto.h>

#include "events/TestEvent.hpp"
#include "events/TestEvent2.hpp"
// #include "otto/components/TransformComponent.hpp"

using namespace otto;

class TestSystem : public System
{
public:
    TestSystem(MultiView<TestComponent, TransformComponent>* view1, View<TestComponent>* view2, View<TransformComponent>* view3)
    {
        mView1 = view1;
        mView2 = view2;
        mView3 = view3;
    }

    void onEvent(const InitEvent& e)
    {
    }

    void onEvent(const UpdateEvent& e)       
    {
    }

    void onEvent(const TestEvent& e)
    {
    }

    void onEvent(const MouseMovedEvent& e)
    {
    }

    void onEvent(const ComponentAddedEvent<TestComponent>& e)
    {
    }

    void onEvent(const ComponentRemovedEvent<TestComponent2>& e)
    {
    }

private:
    MultiView<TestComponent, TransformComponent>* mView1;
    View<TestComponent>* mView2;
    View<TransformComponent>* mView3;
};
