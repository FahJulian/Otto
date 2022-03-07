#pragma once

#include <otto.h>

#include <packages/otto/core.h>

using namespace otto;

class TestSystem2 : public System
{
public:
    void onEvent(const InitEvent& e)
    {
    }

    void onEvent(const UpdateEvent& e)      
    {
    }
};
