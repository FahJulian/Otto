#pragma once

#include "otto/base.h"

#include "otto/util/dynamic_array.h"
#include "otto/util/function.h"
#include "otto/util/map.h"
#include "otto/util/optional.h"
#include "otto/util/pair.h"
#include "otto/util/result.h"
#include "otto/util/shared.h"
#include "otto/util/static_array.h"
#include "otto/util/string.h"
#include "otto/util/unique.h"

#include "otto/scene/scene.h"

#include "otto/math/mat2x2.h"
#include "otto/math/math.h"
#include "otto/math/vec2.h"

#include "otto/debug/log/log.h"

#include "otto/debug/profile/profiler.h"

#include "otto/core/platform/time.h"

#include "otto/scene/view.h"
#include "otto/scene/scene.h"
#include "otto/scene/multi_view.h"
#include "otto/scene/serialization.h"
#include "otto/scene/behaviour.h"

#include "otto/window/events.h"

#include "otto/events/key/KeyPressedEvent.hpp"
#include "otto/events/key/KeyReleasedEvent.hpp"
#include "otto/events/mouse/MouseButtonPressedEvent.hpp"
#include "otto/events/mouse/MouseButtonReleasedEvent.hpp"
#include "otto/events/mouse/MouseMovedEvent.hpp"
#include "otto/events/mouse/MouseDraggedEvent.hpp"
#include "otto/events/mouse/MouseScrolledEvent.hpp"
#include "otto/events/window/WindowClosedEvent.hpp"
#include "otto/events/window/WindowResizedEvent.hpp"
#include "otto/events/window/WindowGainedFocusEvent.hpp"
#include "otto/events/window/WindowLostFocusEvent.hpp"
