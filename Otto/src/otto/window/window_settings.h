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
		bool saveTitle = true;

		Color clearColor = 0x333333ff;
		bool saveClearColor = true;

		uint16 width = 0;
		uint16 height = 0;
		bool saveSize = true;

		WindowMode windowMode = WindowMode::WINDOWED;
		bool saveWindowMode = true;

		bool resizable = false;
		bool closeButton = true;

		bool closeOnAltF4 = true;

		bool minimized = false;
		bool maximized = false;

		bool saveMinimized = false;
		bool saveMaximized = true;

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
