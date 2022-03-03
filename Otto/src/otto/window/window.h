#pragma once

#include "otto/base.h"
#include "otto/window/events.h"
#include "otto/graphics/color.h"
#include "otto/window/input/mouse.h"
#include "otto/window/window_mode.h"
#include "otto/event/event_listener.h"
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
		struct Listeners
		{
			EventListener<_KeyPressedEvent> onKeyPressed = EventListener<_KeyPressedEvent>([](auto& e) {});
			EventListener<_KeyReleasedEvent> onKeyReleased = EventListener<_KeyReleasedEvent>([](auto& e) {});
			EventListener<_MouseButtonPressedEvent> onMouseButtonPressed = EventListener<_MouseButtonPressedEvent>([](auto& e) {});
			EventListener<_MouseButtonReleasedEvent> onMouseButtonReleased = EventListener<_MouseButtonReleasedEvent>([](auto& e) {});
			EventListener<_MouseMovedEvent> onMouseMoved = EventListener<_MouseMovedEvent>([](auto& e) {});
			EventListener<_MouseDraggedEvent> onMouseDragged = EventListener<_MouseDraggedEvent>([](auto& e) {});
			EventListener<_MouseScrolledEvent> onMouseScrolled = EventListener<_MouseScrolledEvent>([](auto& e) {});
			EventListener<_WindowClosedEvent> onWindowClosed = EventListener<_WindowClosedEvent>([](auto& e) {});
			EventListener<_WindowResizedEvent> onWindowResized = EventListener<_WindowResizedEvent>([](auto& e) {});
			EventListener<_WindowGainedFocusEvent> onWindowGainedFocus = EventListener<_WindowGainedFocusEvent>([](auto& e) {});
			EventListener<_WindowLostFocusEvent> onWindowLostFocus = EventListener<_WindowLostFocusEvent>([](auto& e) {});
		};

		static bool init(const FilePath& infoFilePath, const Listeners& listeners);

		static void saveSettings();
		static void destroy();

		static void pollEvents();

		static void clear();
		static void swapBuffers();

		static void setClearColor(const Color& color);

		static void setCursor(const String& cursorName);

		static void setTitle(const String& title);
		static String getTitle();

		static void setWidth(float32 width);
		static void setHeight(float32 height);
		static void setSize(float32 width, float32 height);

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

		static void _onResized(uint16 width, uint16 height);
		static void _onMouseButtonPressed(MouseButton button);
		static void _onMouseButtonReleased(MouseButton button);

		static __int64 __stdcall WindowProc(HWND__* handle, unsigned int msg, unsigned __int64 wParam, __int64 lParam);
	};

}
