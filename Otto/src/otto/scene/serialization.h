#pragma once

#include "otto/base.h"
#include "otto/scene/entity.h"
#include "otto/serialization/serialization.h"

namespace otto
{
    template<typename C>
    C deserializeComponent(const Serialized& args, const Map<String, Entity>& entities)
    {
        OTTO_ASSERT(false, "Component can't be deserialized.");
    }

} // namespace otto
