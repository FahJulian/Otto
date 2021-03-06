#pragma

#include <otto.h>

#include <packages/otto/2D.h>
#include <packages/otto/core.h>
#include <packages/otto/window.h>

#include "events/TestEvent.hpp"
#include "events/TestEvent2.hpp"

using namespace otto;

class TestSystem : public System
{
public:
    TestSystem(MultiView<Renderer2DComponent, Transform2DComponent>* view1, View<TestComponent>* view2, View<Transform2DComponent>* view3)
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
    MultiView<Renderer2DComponent, Transform2DComponent>* mView1;
    View<TestComponent>* mView2;
    View<Transform2DComponent>* mView3;
};
