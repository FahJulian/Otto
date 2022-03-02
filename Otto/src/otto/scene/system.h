#pragma once

#include "otto/base.h"
#include "otto/scene/scene.h"

namespace otto
{
    class System
    {
    protected:
        Scene* mScene;

        friend class Scene;
    };

} // namespace otto
