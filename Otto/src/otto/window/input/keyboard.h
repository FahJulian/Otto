#pragma once

#include "otto/base.h"
#include "otto/util/static_array.h"
#include "otto/window/input/keys.h"

namespace otto 
{
	
	class Keyboard
	{
	private:
		Keyboard() = delete;
		Keyboard(const Keyboard& other) = delete;

	public:
		static bool8 isKeyPressed(Key key) { return sKeys[static_cast<uint64>(key)]; }

	private:
		static StaticArray<bool8, static_cast<uint8>(Key::_HIGHEST_KEY) + 1> sKeys;

		friend class Window;
	};

} // namespace otto
