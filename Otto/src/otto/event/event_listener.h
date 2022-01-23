#pragma once

#include "otto/base.h"
#include "otto/util/function.h"

namespace otto
{
    template<typename E>
    using EventListener = Function<void(const E&)>;

} // namespace otto
