#pragma once

#include "otto/base.h"
#include "otto/util/map.h"
#include "otto/util/string.h"
#include "otto/graphics/color.h"
#include "otto/window/icon/icon.h"
#include "otto/window/icon/cursor.h"
#include "otto/window/window_mode.h"

typedef unsigned long DWORD;

namespace otto
{
	struct WindowSettings
	{
		String title;
		bool8 saveTitle = true;

		Color clearColor = 0x333333ff;
		bool8 saveClearColor = true;

		uint16 width = 0;
		uint16 height = 0;
		bool8 saveSize = true;

		WindowMode windowMode = WindowMode::WINDOWED;
		bool8 saveWindowMode = true;

		bool8 resizable = false;
		bool8 closeButton = true;

		bool8 closeOnAltF4 = true;

		bool8 minimized = false;
		bool8 maximized = false;

		bool8 saveMinimized = false;
		bool8 saveMaximized = true;

		Map<String, Icon> iconSet;
		Map<String, Cursor> cursorSet;

		String defaultCursor;

		uint16 unmaximizedWidth = 0;
		uint16 unmaximizedHeight = 0;
		uint16 monitorWidth = 0;
		uint16 monitorHeight = 0;

		DWORD windowedStyle = 0;
		DWORD windowedExStyle = 0;
	};

} // namespace otto
