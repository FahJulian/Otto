#pragma once

#include "otto/base.h"
#include "otto/window/events.h"
#include "otto/graphics/color.h"
#include "otto/window/input/mouse.h"
#include "otto/window/window_mode.h"
#include "otto/event/event_listener.h"
#include "otto/window/input/keyboard.h"
#include "otto/window/window_settings.h"
#include "otto/util/platform/file_path.h"
#include "otto/serialization/serialized.h"

struct HWND__;
struct HICON__;

namespace otto 
{
	class Window
	{
	private:
		Window() = default;
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

		static bool8 init(const FilePath& infoFilePath);
		static bool8 init(Window* instance);

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

		static float32 getWidth();
		static float32 getHeight();

		static void setMaximized(bool8 b);
		static void setMinimized(bool8 b);

		static bool8 isMinimized();
		static bool8 isMaximized();

		static float32 getMonitorWidth();
		static float32 getMonitorHeight();

		static void setWindowMode(WindowMode mode);
		static WindowMode getWindowMode();

	private:
		static Window* getInstance();

		static void setListeners(const Listeners& listeners);

		static bool8 _createWin32Window();

		static void _onResized(uint16 width, uint16 height);
		static void _onMouseButtonPressed(MouseButton button);
		static void _onMouseButtonReleased(MouseButton button);

		static void _initCursors(const String& defaultCursor);
		static void _initIcons();
		static void _initStyle();

		static void _setDecorated(bool8 decorated);

		static void _updateWindowSize();

		static void _adjustSize(uint16 width, uint16 height);

		static __int64 __stdcall WindowProc(HWND__* handle, unsigned int msg, unsigned __int64 wParam, __int64 lParam);

	private:
		FilePath mSettingsFilePath;
		Serialized mInitialSettings;
		WindowSettings mSettings;

		Window::Listeners mListeners;

		bool8 mIgnoreSizeMessage = false;

		HWND__* mWin32Handle;

		HICON__* mSmallIconHandle;
		HICON__* mLargeIconHandle;
		HICON__* mCurrentCursor;
		Map<String, HICON__*> mCursors;

		friend class Application;
	};

}
