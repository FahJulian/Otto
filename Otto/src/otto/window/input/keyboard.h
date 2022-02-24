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
		static bool isKeyPressed(Key key) 
		{ 
			return sKeys[key]; 
		}

	private:
		static StaticArray<bool, static_cast<uint8>(Keys::_HIGHEST_KEY) + 1> sKeys;

		friend class Window;
	};

} // namespace otto
