#pragma once

#include "otto/base.h"
#include "otto/serialization/serialized.h"

namespace otto
{
    struct OttoFile
    {
        enum class Type : uint16
        {
            VOID,
            SCENE,
        };

        Type type = Type::VOID;
        Serialized serialized = Serialized(Serialized::Type::DICTIONARY);
    };

} // namespace otto
