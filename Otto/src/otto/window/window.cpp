#include "window.h"

#include "_win32_include.h"

#include "otto/debug/log/log.h"
#include "otto/core/application.h"
#include "otto/window/events.h"
#include "otto/window/window_settings_loader.h"
#include "otto/serialization/serializer.h"

namespace otto
{
    namespace
    {
        static FilePath sSettingsFilePath;
        static Serialized sInitialSettings;
        static WindowSettings sSettings;

        static Window::Listeners sListeners;

        static bool sIgnoreSizeMessage = false;

        static HWND sWin32Handle;
        static HDC sDeviceContext;

        static HICON sSmallIconHandle;
        static HICON sLargeIconHandle;
        static HCURSOR sCurrentCursor;
        static Map<String, HCURSOR> sCursors;

        std::wstring _toWideString(const String& string)
        {
            std::wstring wideString = std::wstring(string.getSize(), char16(0));
            MultiByteToWideChar(CP_ACP, 0, string.getData(), -1, LPWSTR((wideString.data())), uint32((string.getSize())));
            return wideString;
        }

        static BITMAPV5HEADER _createBitmapHeader(uint16 width, uint16 height)
        {
            BITMAPV5HEADER bi = {};

            bi.bV5Size = sizeof(BITMAPV5HEADER);
            bi.bV5Width = width;
            bi.bV5Height = height;
            bi.bV5Planes = 1;
            bi.bV5BitCount = 32;
            bi.bV5Compression = BI_BITFIELDS;
            bi.bV5RedMask = 0x00ff0000;
            bi.bV5GreenMask = 0x0000ff00;
            bi.bV5BlueMask = 0x000000ff;
            bi.bV5AlphaMask = 0xff000000;

            return bi;
        }

        static HICON _createNativeIcon(uint16 width, uint16 height, uint8_t* bitmap, bool isCursor = false, uint16 hotspotX = 0, uint16 hotspotY = 0)
        {
            if (!bitmap)
                return 0;

            HICON handle;
            HBITMAP color, mask;
            BITMAPV5HEADER bi = _createBitmapHeader(width, height);
            ICONINFO info = {};
            uint8_t* nativeBitmap;

            HDC dc = GetDC(NULL);
            color = CreateDIBSection(dc, (BITMAPINFO*)&bi, DIB_RGB_COLORS,
                (void**)&nativeBitmap, NULL, (DWORD)0);

            mask = CreateBitmap(width, height, 1, 1, NULL);
            ReleaseDC(NULL, dc);

            if (!color || !mask)
            {
                Log::error("Error: Could not create win32 bitmaps");
                return NULL;
            }

            std::copy(bitmap, bitmap + uint64(4) * uint64(width) * uint64(height), nativeBitmap);

            info.fIcon = !isCursor;
            info.xHotspot = hotspotX;
            info.yHotspot = hotspotY;
            info.hbmMask = mask;
            info.hbmColor = color;

            handle = CreateIconIndirect(&info);

            DeleteObject(color);
            DeleteObject(mask);

            if (!handle)
            {
                Log::error("Error: Could not create win32 ", isCursor ? "cursor" : "icon");
                return NULL;
            }

            return handle;
        }

        static void _initCursors()
        {
            for (auto& [name, cursor] : sSettings.cursors)
            {
                HCURSOR handle = HCURSOR(_createNativeIcon(cursor.width, cursor.height, cursor.bitmap, true, cursor.hotspotX, cursor.hotspotY));
                if (handle)
                    sCursors.insert(name, handle);
            }

            //sCurrentCursor = sCursors[Cursors::Arrow];
        }

        static void _initIcons()
        {
            uint16 smallIconWidth = uint16(GetSystemMetrics(SM_CXSMICON));
            uint16 smallIconHeight = uint16(GetSystemMetrics(SM_CYSMICON));
            uint16 largeIconWidth = uint16(GetSystemMetrics(SM_CXICON));
            uint16 largeIconHeight = uint16(GetSystemMetrics(SM_CYICON));

            Icon smallIcon;
            float64 smallestSizeDifference = 1.0e35;
            for (auto& [name, icon] : sSettings.icons)
            {
                int32 xoff = smallIconWidth - icon.width;
                int32 yoff = smallIconHeight - icon.height;

                float64 offset = sqrt(xoff * xoff + yoff * yoff);
                if (offset < smallestSizeDifference)
                {
                    smallestSizeDifference = offset;
                    smallIcon = icon;
                }
            }

            Icon largeIcon;
            smallestSizeDifference = 1.0e35;
            for (auto& [name, icon] : sSettings.icons)
            {
                int32 xoff = largeIconWidth - icon.width;
                int32 yoff = largeIconHeight - icon.height;

                double offset = sqrt(xoff * xoff + yoff * yoff);
                if (offset < smallestSizeDifference)
                {
                    smallestSizeDifference = offset;
                    largeIcon = icon;
                }
            }

            if (smallIcon.bitmap)
                sSmallIconHandle = _createNativeIcon(smallIcon.width, smallIcon.height, smallIcon.bitmap);
            if (largeIcon.bitmap)
                sLargeIconHandle = _createNativeIcon(largeIcon.width, largeIcon.height, largeIcon.bitmap);

            if (sSmallIconHandle)
                SendMessage(sWin32Handle, WM_SETICON, ICON_SMALL, LPARAM(sSmallIconHandle));
            if (sLargeIconHandle)
                SendMessage(sWin32Handle, WM_SETICON, ICON_BIG, LPARAM(sLargeIconHandle));
        }

        static void _initStyle()
        {
            sSettings.windowedStyle = (WS_OVERLAPPED | WS_CAPTION);
            sSettings.windowedExStyle = 0;

            if (sSettings.closeButton)
                sSettings.windowedStyle |= WS_SYSMENU;

            if (sSettings.resizable)
                sSettings.windowedStyle |= (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
        }

        static void _setDecorated(bool decorated)
        {
            sIgnoreSizeMessage = true;
            SetWindowLong(sWin32Handle, GWL_STYLE, decorated ? sSettings.windowedStyle : 0);
            SetWindowLong(sWin32Handle, GWL_EXSTYLE, decorated ? sSettings.windowedExStyle : 0);
            sIgnoreSizeMessage = false;
        }

        static void _updateWindowSize()
        {
            if (sSettings.windowMode == WindowMode::WINDOWED)
            {
                RECT newWindow = { (sSettings.monitorWidth - sSettings.unmaximizedWidth) / 2, (sSettings.monitorHeight - sSettings.unmaximizedHeight) / 2,
                    (sSettings.monitorWidth + sSettings.unmaximizedWidth) / 2, (sSettings.monitorHeight + sSettings.unmaximizedHeight) / 2 };
                AdjustWindowRectEx(&newWindow, sSettings.windowedStyle, FALSE, sSettings.windowedExStyle);

                SetWindowPos(sWin32Handle, HWND_TOP, newWindow.left, newWindow.top,
                    newWindow.right - newWindow.left, newWindow.bottom - newWindow.top, SWP_NOZORDER);

                ShowWindow(sWin32Handle, sSettings.maximized ? SW_MAXIMIZE : sSettings.minimized ? SW_MINIMIZE : SW_NORMAL);
            }
            else if (sSettings.windowMode == WindowMode::WINDOWED_BORDERLESS)
            {
                SetWindowPos(sWin32Handle, HWND_TOP, (sSettings.monitorWidth - sSettings.unmaximizedWidth) / 2, (sSettings.monitorHeight - sSettings.unmaximizedHeight) / 2,
                    sSettings.unmaximizedWidth, sSettings.unmaximizedHeight, SWP_NOZORDER);

                ShowWindow(sWin32Handle, sSettings.minimized ? SW_MINIMIZE : SW_SHOW);
            }
            else
            {
                SetWindowPos(sWin32Handle, HWND_TOP, 0, 0, sSettings.monitorWidth, sSettings.monitorHeight, SWP_NOZORDER);
                ShowWindow(sWin32Handle, sSettings.minimized ? SW_MINIMIZE : SW_SHOW);
            }
        }

        static void _adjustSize(uint16 width, uint16 height)
        {
            sSettings.unmaximizedWidth = width;
            sSettings.unmaximizedHeight = height;

            _updateWindowSize();
        }

        static void _setFullscreen(bool fullscreen)
        {
            DISPLAY_DEVICE displayDevice = {};
            displayDevice.cb = sizeof(DISPLAY_DEVICE);
            EnumDisplayDevices(NULL, 0, &displayDevice, 0);

            if (fullscreen)
            {
                DEVMODE deviceMode = {};
                deviceMode.dmSize = sizeof(DEVMODE);
                BOOL s = EnumDisplaySettingsEx(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &deviceMode, 0);

                deviceMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL |
                    DM_DISPLAYFREQUENCY;

                LONG success = ChangeDisplaySettingsEx(displayDevice.DeviceName, &deviceMode, NULL, CDS_FULLSCREEN, NULL);
                if (success != DISP_CHANGE_SUCCESSFUL)
                    Log::error("Error changing display settings");
            }
            else
            {
                LONG success = ChangeDisplaySettingsEx(displayDevice.DeviceName, NULL, NULL, CDS_FULLSCREEN, NULL);
                if (success != DISP_CHANGE_SUCCESSFUL)
                    Log::error("Error changing display settings");
            }
        }

        static Key _getKey(LPARAM lParam, WPARAM wParam)
        {
            Key key = Key(wParam);

            if (key == Key::CONTROL) // Win32 does weird things
            {
                if (lParam & 1 << 24) // Extended key -> Right Control
                {
                    key = Key::RIGHT_CONTROL;
                }
                else if (MSG next;  // When right alt is pressed, two messages are sent, the first one has to be ignored
                    PeekMessage(&next, NULL, WM_KEYFIRST, WM_KEYLAST, PM_NOREMOVE))
                {
                    if (Key nextKey = *((Key*)&next.wParam);
                        nextKey == Key::ALT && next.lParam & 1 << 24)
                    {
                        return Key::UNKNOWN;
                    }
                }
            }
            else if (lParam & 1 << 24)
            {
                if (key == Key::ALT)
                    key = Key::RIGHT_ALT;
                else if (key == Key::ENTER)
                    key = Key::NUMPAD_ENTER;
            }

            return key;
        }

    } // namespace

    bool Window::init(const FilePath& filePath, const Listeners& listeners)
    {
        auto serialized = Serializer::deserialize(filePath);
        if (serialized.hasError())
            return false;

        auto settings = WindowSettingsLoader::_loadWindowSettingsFromSerialized(serialized.getResult());
        if (settings.hasError())
            return false;

        sSettingsFilePath = filePath;
        sInitialSettings = serialized.getResult();
        sSettings = settings.getResult();

        sListeners = listeners;

        sSettings.unmaximizedWidth = sSettings.width;
        sSettings.unmaximizedHeight = sSettings.height;
        sSettings.windowMode = sSettings.windowMode;
        sSettings.title = sSettings.title;

        if (!_createWin32Window())
            return false;

        _initStyle();

        sSettings.monitorWidth = GetSystemMetrics(SM_CXSCREEN);
        sSettings.monitorHeight = GetSystemMetrics(SM_CYSCREEN);

        _setFullscreen(sSettings.windowMode == WindowMode::FULLSCREEN);
        _setDecorated(sSettings.windowMode == WindowMode::WINDOWED);

        sSettings.maximized = sSettings.maximized;
        sSettings.minimized = !sSettings.maximized && sSettings.minimized;

        _updateWindowSize();

        _initCursors();
        _initIcons();

        if (!_createContext())
            return false;

        setClearColor(sSettings.clearColor = sSettings.clearColor);

        return true;
    }

    bool Window::_createWin32Window()
    {
        const char16* className = L"Sample Window Class";
        HMODULE win32ModuleHandle = GetModuleHandle(0);

        WNDCLASSEX win32WindowClass = { };
        win32WindowClass.cbSize = sizeof(WNDCLASSEX);
        win32WindowClass.lpfnWndProc = Window::WindowProc;
        win32WindowClass.hInstance = win32ModuleHandle;
        win32WindowClass.lpszClassName = className;
        win32WindowClass.style = CS_OWNDC;

        RegisterClassEx(&win32WindowClass);

        sWin32Handle = CreateWindowEx(0, className, _toWideString(sSettings.title).c_str(), 0,
            0, 0, 0, 0, NULL, NULL, win32ModuleHandle, NULL
        );

        if (sWin32Handle == NULL)
        {
            DWORD error = GetLastError();
            Log::error("Error creating win32 Window (Win32 Error code: ", error, ")");
            return false;
        }

        return true;
    }

    bool Window::_createContext()
    {
        return true;
    }

    void Window::pollEvents()
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    void Window::_onResized(uint16 width, uint16 height)
    {
        if (sSettings.windowMode == WindowMode::WINDOWED && !sSettings.minimized && !sSettings.maximized)
        {
            sSettings.unmaximizedWidth = width;
            sSettings.unmaximizedHeight = height;
        }

        if (sSettings.width != width || sSettings.height != height)
        {
            sSettings.width = width;
            sSettings.height = height;

            sListeners.onWindowResized(_WindowResizedEvent(float32(width), float32(height)));
        }
    }

    void Window::_onMouseButtonPressed(MouseButton button)
    {
        Mouse::sButtons[uint64(button)] = true;
        sListeners.onMouseButtonPressed(_MouseButtonPressedEvent(button, Mouse::sX, Mouse::sY));
        SetCapture(sWin32Handle);
    }

    void Window::_onMouseButtonReleased(MouseButton button)
    {
        Mouse::sButtons[uint64(button)] = false;
        sListeners.onMouseButtonReleased(_MouseButtonReleasedEvent(button, Mouse::sX, Mouse::sY));
        ReleaseCapture();
    }

    void Window::setTitle(const String& title)
    {
        sSettings.title = title;
        SetWindowText(sWin32Handle, _toWideString(title).c_str());
    }

    String Window::getTitle()
    {
        return sSettings.title;
    }

    void Window::setWidth(float32 width)
    {
        _adjustSize(uint16(width + 0.5f), sSettings.height);
    }

    void Window::setHeight(float32 height)
    {
        _adjustSize(sSettings.width, uint16(height + 0.5f));
    }

    void Window::setSize(float32 width, float32 height)
    {
        _adjustSize(uint16(width + 0.5f), uint16(height + 0.5f));
    }

    void Window::setMaximized(bool b)
    {
        sSettings.maximized = b;

        if (sSettings.windowMode == WindowMode::WINDOWED)
            ShowWindow(sWin32Handle, sSettings.maximized ? SW_MAXIMIZE : SW_NORMAL);
    }

    void Window::setMinimized(bool b)
    {
        sSettings.minimized = b;

        ShowWindow(sWin32Handle, sSettings.minimized ? SW_MINIMIZE : (sSettings.maximized && sSettings.windowMode == WindowMode::WINDOWED) ? SW_MAXIMIZE : SW_NORMAL);
    }

    bool Window::isMaximized()
    {
        return sSettings.maximized || sSettings.windowMode == WindowMode::WINDOWED_FULLSCREEN || sSettings.windowMode == WindowMode::FULLSCREEN ||
            (sSettings.windowMode == WindowMode::WINDOWED_BORDERLESS && sSettings.width == sSettings.monitorWidth && sSettings.width == sSettings.monitorHeight);
    }

    bool Window::isMinimized()
    {
        return sSettings.minimized;
    }

    float32 Window::getWidth()
    {
        return float32(sSettings.width);
    }

    float32 Window::getHeight()
    {
        return float32(sSettings.height);
    }

    float32 Window::getMonitorWidth()
    {
        return float32(sSettings.monitorWidth);
    }

    float32 Window::getMonitorHeight()
    {
        return float32(sSettings.monitorHeight);
    }

    WindowMode Window::getWindowMode()
    {
        return sSettings.windowMode;
    }

    void Window::setWindowMode(WindowMode mode)
    {
        if (mode == sSettings.windowMode)
            return;

        sIgnoreSizeMessage = true;

        if (mode == WindowMode::FULLSCREEN )
            _setFullscreen(true);
        else if (sSettings.windowMode == WindowMode::FULLSCREEN )
            _setFullscreen(false);

        if (mode == WindowMode::WINDOWED)
            _setDecorated(true);
        else if (sSettings.windowMode == WindowMode::WINDOWED)
            _setDecorated(false);

        sSettings.windowMode = mode;

        _updateWindowSize();
    }

    void Window::setClearColor(const Color& color)
    {
        sSettings.clearColor = color;
    }

    void Window::setCursor(const String& name)
    {
        RECT clientArea;
        POINT cursorPos;

        if (!GetCursorPos(&cursorPos))
            return;

        if (WindowFromPoint(cursorPos) != sWin32Handle)
            return;

        GetClientRect(sWin32Handle, &clientArea);
        ClientToScreen(sWin32Handle, (POINT*)&clientArea.left);
        ClientToScreen(sWin32Handle, (POINT*)&clientArea.right);

        if (!PtInRect(&clientArea, cursorPos))
            return;

        if (sCursors.containsKey(name))
            SetCursor(sCurrentCursor = sCursors.get(name));
    }

    void Window::clear()
    {
    }

    void Window::swapBuffers()
    {
        SwapBuffers(sDeviceContext);
    }

    void Window::saveSettings()
    {
        //if (sSettings.saveSize)
        //{
        //    sSettings.width = sSettings.unmaximizedWidth ;
        //    sSettings.height = sSettings.unmaximizedHeight;
        //}

        //if (sSettings.saveTitle)
        //    sSettings.title = sSettings.title;
        //if (sSettings.saveWindowMode)
        //    sSettings.windowMode = sSettings.windowMode;
        //if (sSettings.saveClearColor)
        //    sSettings.clearColor = sSettings.clearColor;
        //if (sSettings.saveMaximized)
        //    sSettings.maximized = sSettings.maximized;
        //if (sSettings.saveMinimized)
        //    sSettings.minimized = sSettings.minimized;

        WindowSettingsLoader::_saveWindowSettingsToOtto(sSettings, sInitialSettings, sSettingsFilePath);
    }

    void Window::destroy()
    {
        if (sSettings.windowMode == WindowMode::FULLSCREEN )
            _setFullscreen(false);

        if (sSmallIconHandle)
            DestroyIcon(sSmallIconHandle);
        if (sLargeIconHandle)
            DestroyIcon(sLargeIconHandle);

        for (auto& [_, cursor] : sCursors)
            DestroyCursor(cursor);

        DestroyWindow(sWin32Handle);
    }

    LRESULT CALLBACK Window::WindowProc(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
        case WM_CLOSE: [[fallthrough]];
        case WM_QUIT:
            sListeners.onWindowClosed(_WindowClosedEvent());
            return 0;

        case WM_LBUTTONDOWN:
            _onMouseButtonPressed(MouseButton::LEFT);
            return 0;

        case WM_LBUTTONUP:
            _onMouseButtonReleased(MouseButton::LEFT);
            return 0;

        case WM_RBUTTONDOWN:
            _onMouseButtonPressed(MouseButton::RIGHT);
            return 0;

        case WM_RBUTTONUP:
            _onMouseButtonReleased(MouseButton::RIGHT);
            return 0;

        case WM_MBUTTONDOWN:
            _onMouseButtonPressed(MouseButton::MIDDLE);
            return 0;

        case WM_MBUTTONUP:
            _onMouseButtonReleased(MouseButton::MIDDLE);
            return 0;

        case WM_XBUTTONDOWN:
            _onMouseButtonPressed(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseButton::MB_4 : MouseButton::MB_5);
            return 0;

        case WM_XBUTTONUP:
            _onMouseButtonReleased(GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? MouseButton::MB_4 : MouseButton::MB_5);
            return 0;

        case WM_MOUSEMOVE:
        {
            float32 beforeX = Mouse::sX;
            float32 beforeY = Mouse::sY;

            Mouse::sX = float32(GET_X_LPARAM(lParam));
            Mouse::sY = sSettings.height - float32(GET_Y_LPARAM(lParam)) - 1;

            float32 deltaX = Mouse::sX - beforeX;
            float32 deltaY = Mouse::sY - beforeY;

            for (uint64 button = 0; button < Mouse::sButtons.getSize(); button++)
                if (Mouse::sButtons[button])
                    sListeners.onMouseDragged(_MouseDraggedEvent(MouseButton(button), Mouse::sX, Mouse::sY, beforeX, beforeY, deltaX, deltaY));

            sListeners.onMouseMoved(_MouseMovedEvent(Mouse::sX, Mouse::sY, beforeX, beforeY, deltaX, deltaY));

            return 0;
        }

        case WM_MOUSEWHEEL:
            sListeners.onMouseScrolled(_MouseScrolledEvent(Mouse::sX, Mouse::sY, 0.0f, float32(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA));
            return 0;

        case WM_SYSKEYDOWN: [[fallthrough]];
        case WM_KEYDOWN:
        {
            Key key = _getKey(lParam, wParam);

            char16 keyChar = 0;
            if (MSG charMessage;
                PeekMessage(&charMessage, NULL, WM_CHAR, WM_CHAR, PM_REMOVE))
            {
                keyChar = *(reinterpret_cast<char16*>(&charMessage.wParam));
            }

            Keyboard::sKeys[uint64(key)] = true;
            sListeners.onKeyPressed(_KeyPressedEvent(key, keyChar));

            if (sSettings.closeOnAltF4 && key == Key::F4 && Keyboard::isKeyPressed(Key::ALT))
                sListeners.onWindowClosed(_WindowClosedEvent());

            return 0;
        }

        case WM_SYSKEYUP: [[fallthrough]];
        case WM_KEYUP:
        {
            Key key = _getKey(lParam, wParam);

            Keyboard::sKeys[uint64(key)] = false;
            sListeners.onKeyReleased(_KeyReleasedEvent(key));

            return 0;
        }

        case WM_SIZE:
            if (!sIgnoreSizeMessage)
            {
                if (wParam == SIZE_MINIMIZED)
                    sSettings.minimized = true;
                else if (sSettings.windowMode == WindowMode::WINDOWED && wParam == SIZE_MAXIMIZED)
                    sSettings.maximized = true;

                _onResized(uint16(LOWORD(lParam)), uint16(HIWORD(lParam)));
            }

            return 0;

        case WM_KILLFOCUS:
            sListeners.onWindowLostFocus(_WindowLostFocusEvent());

            if (sSettings.windowMode == WindowMode::FULLSCREEN )
                ShowWindow(sWin32Handle, SW_MINIMIZE);

            return 0;

        case WM_SETFOCUS:
            Mouse::sButtons = { false };
            Keyboard::sKeys = { false };

            sListeners.onWindowGainedFocus(_WindowGainedFocusEvent());
            return 0;

        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT)
            {
                SetCursor(sCurrentCursor);
                return TRUE;
            }
            else
            {
                break;
            }
        }

        return DefWindowProc(handle, msg, wParam, lParam);
    }

} // namespace otto
