#pragma once

#include "otto/base.h"
#include "otto/platform_detection.h"

#include "otto/util/map.h"
#include "otto/util/pair.h"
#include "otto/util/file.h"
#include "otto/util/result.h"
#include "otto/util/shared.h"
#include "otto/util/string.h"
#include "otto/util/triple.h"
#include "otto/util/unique.h"
#include "otto/util/function.h"
#include "otto/util/optional.h"
#include "otto/util/static_array.h"
#include "otto/util/dynamic_array.h"

#include "otto/util/platform/file_path.h"
#include "otto/util/platform/file_utils.h"

#include "otto/math/math.h"
#include "otto/math/vec2.h"
#include "otto/math/vec3.h"
#include "otto/math/vec4.h"
#include "otto/math/mat2x2.h"
#include "otto/math/mat3x3.h"
#include "otto/math/mat4x4.h"

#include "otto/debug/log/log.h"
#include "otto/debug/profile/profiler.h"

#include "otto/core/application.h"
#include "otto/core/scene_manager.h"

#include "otto/core/platform/time.h"

#include "otto/scene/view.h"
#include "otto/scene/scene.h"
#include "otto/scene/entity.h"
#include "otto/scene/system.h"
#include "otto/scene/behaviour.h"
#include "otto/scene/multi_view.h"
#include "otto/scene/serialization.h"
#include "otto/scene/component_pool.h"

#include "otto/serialization/serialized.h"
#include "otto/serialization/serializer.h"
#include "otto/serialization/serialization.h"

#include "otto/window/window.h"
#include "otto/window/window_mode.h"

#include "otto/window/icon/cursor.h"
#include "otto/window/icon/icon.h"

#include "otto/window/input/keys.h"
#include "otto/window/input/mouse.h"
#include "otto/window/input/keyboard.h"
#include "otto/window/input/mouse_buttons.h"

#include "otto/event/event_listener.h"
#include "otto/event/event_dispatcher.h"

#include "otto/graphics/color.h"
#include "otto/graphics/sprite.h"

#include "otto/events/InitEvent.hpp"
#include "otto/events/UpdateEvent.hpp"

#include "otto/events/key/KeyPressedEvent.hpp"
#include "otto/events/key/KeyReleasedEvent.hpp"

#include "otto/events/mouse/MouseMovedEvent.hpp"
#include "otto/events/mouse/MouseDraggedEvent.hpp"
#include "otto/events/mouse/MouseScrolledEvent.hpp"
#include "otto/events/mouse/MouseButtonPressedEvent.hpp"
#include "otto/events/mouse/MouseButtonReleasedEvent.hpp"

#include "otto/events/window/WindowClosedEvent.hpp"
#include "otto/events/window/WindowResizedEvent.hpp"
#include "otto/events/window/WindowLostFocusEvent.hpp"
#include "otto/events/window/WindowGainedFocusEvent.hpp"

#include "otto/events/ui/UIClickedEvent.hpp"

#include "otto/components/TransformComponent.hpp"

#include "otto/components/ui/UIComponent.hpp"
