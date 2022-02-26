#pragma once

#include "otto/base.h"
#include "otto/graphics/color.h"
#include "otto/window/input/mouse.h"
#include "otto/window/window_mode.h"
#include "otto/window/input/keyboard.h"
#include "otto/util/platform/file_path.h"

struct HWND__;

namespace otto 
{
	class Window
	{
	private:
		Window() = delete;
		Window(const Window& other) = delete;

	public:
		static bool init(const FilePath& infoFilePath);

		static void saveInfo();
		static void destroy();

		static void pollEvents();

		static void clear();
		static void swapBuffers();

		static void setClearColor(const Color& color);

		static void setCursor(const String& cursorName);

		static void setTitle(const String& title);
		static String getTitle();

		static void setWidth(float width);
		static void setHeight(float height);
		static void setSize(float width, float height);

		static float getWidth();
		static float getHeight();

		static void setMaximized(bool b);
		static void setMinimized(bool b);

		static bool isMinimized();
		static bool isMaximized();

		static float getMonitorWidth();
		static float getMonitorHeight();

		static void setWindowMode(WindowMode mode);
		static WindowMode getWindowMode();

	private:
		static bool _createWin32Window();

		static void _onResized(int width, int height);
		static void _onMouseButtonPressed(MouseButton button);
		static void _onMouseButtonReleased(MouseButton button);

		static __int64 __stdcall WindowProc(HWND__* handle, unsigned int msg, unsigned __int64 wParam, __int64 lParam);

		static bool _createContext();
	};

}
