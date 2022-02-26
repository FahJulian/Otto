#include "keyboard.h"

namespace otto
{
    StaticArray<bool, static_cast<uint8>(Key::_HIGHEST_KEY) + 1> Keyboard::sKeys;

} // namespace otto
