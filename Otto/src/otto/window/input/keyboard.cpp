#include "keyboard.h"

namespace otto
{
    StaticArray<bool, static_cast<uint8>(Keys::_HIGHEST_KEY) + 1> Keyboard::sKeys;

} // namespace otto
