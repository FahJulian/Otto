#include "mouse.h"

namespace otto
{
    float32 Mouse::sX = 0.0f;
    float32 Mouse::sY = 0.0f;

    StaticArray<bool, static_cast<uint8>(MouseButtons::_HIGHEST_BUTTON) + 1> Mouse::sButtons;

} // namespace otto
