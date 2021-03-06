#pragma once

#include "otto/base.h"
#include "otto/scene/entity.h"

namespace otto
{
    class Scene;

    class Behaviour
    {
    protected:
        Entity mEntity;
        Scene* mScene;

        friend class Scene;
    };

} // namespace otto
