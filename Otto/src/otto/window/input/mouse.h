#pragma once

#include "otto/base.h"
#include "otto/util/static_array.h"
#include "otto/window/input/mouse_buttons.h"

namespace otto 
{
	class Mouse
	{
	private:
		Mouse() = delete;
		Mouse(const Mouse& other) = delete;

	public:
		static float32 getX() { return sX; }
		static float32 getY() { return sY; }

		static bool isButtonPressed(MouseButton button) { return sButtons[static_cast<uint64>(button)]; }

	private:
		static float32 sX, sY;
		static StaticArray<bool, static_cast<uint8>(MouseButton::_HIGHEST_BUTTON) + 1> sButtons;

		friend class Window;
	};

} // namespace otto
