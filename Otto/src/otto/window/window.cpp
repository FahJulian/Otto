#include "window.h"

#include "_win32_include.h"

#include "otto/debug/log.h"
#include "otto/util/file_utils.h"
#include "otto/core/application.h"
#include "otto/graphics/context.h"
#include "otto/graphics/graphics_api.h"
#include "otto/serialization/serializer.h"
#include "otto/serialization/window_settings_loader.h"

namespace otto
{
    namespace
    {
        static Window* sInstance = nullptr;

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

        static HICON _createNativeIcon(uint16 width, uint16 height, uint8_t* bitmap, bool8 isCursor = false, uint16 hotspotX = 0, uint16 hotspotY = 0)
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



        static void _setFullscreen(bool8 fullscreen)
        {
            DISPLAY_DEVICE displayDevice = {};
            displayDevice.cb = sizeof(DISPLAY_DEVICE);
            EnumDisplayDevices(NULL, 0, &displayDevice, 0);

            if (fullscreen)
            {
                DEVMODE deviceMode = {};
                deviceMode.dmSize = sizeof(DEVMODE);
                bool8 s = EnumDisplaySettingsEx(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &deviceMode, 0);

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

    bool8 Window::init(const FilePath& filePath)
    {
        static Window instance;

        if (sInstance != nullptr)
            return false;

        sInstance = &instance;

        Log::trace("Initializing Window...");

        auto serialized = Serializer::load(filePath);
        if (serialized.hasError())
        {
            Log::error("Failed to initialize Window: Syntax error");
            return false;
        }

        auto settings = WindowSettingsLoader::_loadWindowSettingsFromSerialized(serialized.getResult());
        if (settings.hasError())
        {
            Log::error("Failed to initialize Window: Could not load Window Settings");
            return false;
        }

        sInstance->mSettingsFilePath = filePath;
        sInstance->mInitialSettings = serialized.getResult();
        sInstance->mSettings = settings.getResult();

        sInstance->mSettings.unmaximizedWidth = sInstance->mSettings.width;
        sInstance->mSettings.unmaximizedHeight = sInstance->mSettings.height;
        sInstance->mSettings.windowMode = sInstance->mSettings.windowMode;
        sInstance->mSettings.title = sInstance->mSettings.title;

        if (!_createWin32Window())
        {
            Log::error("Failed to initialize Window: Could not create win32 window");
            return false;
        }

        _initStyle();

        sInstance->mSettings.monitorWidth = GetSystemMetrics(SM_CXSCREEN);
        sInstance->mSettings.monitorHeight = GetSystemMetrics(SM_CYSCREEN);

        _setFullscreen(sInstance->mSettings.windowMode == WindowMode::FULLSCREEN);
        _setDecorated(sInstance->mSettings.windowMode == WindowMode::WINDOWED);

        sInstance->mSettings.maximized = sInstance->mSettings.maximized;
        sInstance->mSettings.minimized = !sInstance->mSettings.maximized && sInstance->mSettings.minimized;

        _updateWindowSize();

        _initCursors(sInstance->mSettings.defaultCursor);
        _initIcons();

        return true;
    }

    bool8 Window::init(Window* instance)
    {
        if (sInstance != nullptr)
            return false;

        sInstance = instance;

        Context::init(reinterpret_cast<ptr64>(instance->mWin32Handle));
        GraphicsAPI::init();

        return true;
    }

    Window* Window::getInstance()
    {
        return sInstance;
    }

    void Window::setListeners(const Listeners& listeners)
    {
        sInstance->mListeners = listeners;
    }

    bool8 Window::_createWin32Window()
    {
        const char16* className = L"Otto Window";
        HMODULE win32ModuleHandle = GetModuleHandle(0);

        WNDCLASSEX win32WindowClass = { };
        win32WindowClass.cbSize = sizeof(WNDCLASSEX);
        win32WindowClass.lpfnWndProc = Window::WindowProc;
        win32WindowClass.hInstance = win32ModuleHandle;
        win32WindowClass.lpszClassName = className;
        win32WindowClass.style = CS_OWNDC;

        RegisterClassEx(&win32WindowClass);

        sInstance->mWin32Handle = CreateWindowEx(0, className, _toWideString(sInstance->mSettings.title).c_str(), 0,
            0, 0, 0, 0, NULL, NULL, win32ModuleHandle, NULL
        );

        if (sInstance->mWin32Handle == NULL)
        {
            DWORD error = GetLastError();
            Log::error("Error creating win32 Window (Win32 Error code: ", error, ")");
            return false;
        }

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
        if (sInstance->mSettings.windowMode == WindowMode::WINDOWED && !sInstance->mSettings.minimized && !sInstance->mSettings.maximized)
        {
            sInstance->mSettings.unmaximizedWidth = width;
            sInstance->mSettings.unmaximizedHeight = height;
        }

        if (sInstance->mSettings.width != width || sInstance->mSettings.height != height)
        {
            sInstance->mSettings.width = width;
            sInstance->mSettings.height = height;

            //GraphicsAPI::setViewport(0, 0, uint32(width), uint32(height));
            sInstance->mListeners.onWindowResized(_WindowResizedEvent(float32(width), float32(height)));
        }
    }

    void Window::_onMouseButtonPressed(MouseButton button)
    {
        Mouse::sButtons[uint64(button)] = true;
        sInstance->mListeners.onMouseButtonPressed(_MouseButtonPressedEvent(button, Mouse::sX, Mouse::sY));
        SetCapture(sInstance->mWin32Handle);
    }

    void Window::_onMouseButtonReleased(MouseButton button)
    {
        Mouse::sButtons[uint64(button)] = false;
        sInstance->mListeners.onMouseButtonReleased(_MouseButtonReleasedEvent(button, Mouse::sX, Mouse::sY));
        ReleaseCapture();
    }

    void Window::setTitle(const String& title)
    {
        sInstance->mSettings.title = title;
        SetWindowText(sInstance->mWin32Handle, _toWideString(title).c_str());
    }

    String Window::getTitle()
    {
        return sInstance->mSettings.title;
    }

    void Window::setWidth(float32 width)
    {
        _adjustSize(uint16(width + 0.5f), sInstance->mSettings.height);
    }

    void Window::setHeight(float32 height)
    {
        _adjustSize(sInstance->mSettings.width, uint16(height + 0.5f));
    }

    void Window::setSize(float32 width, float32 height)
    {
        _adjustSize(uint16(width + 0.5f), uint16(height + 0.5f));
    }

    void Window::setMaximized(bool8 b)
    {
        sInstance->mSettings.maximized = b;

        if (sInstance->mSettings.windowMode == WindowMode::WINDOWED)
            ShowWindow(sInstance->mWin32Handle, sInstance->mSettings.maximized ? SW_MAXIMIZE : SW_NORMAL);
    }

    void Window::setMinimized(bool8 b)
    {
        sInstance->mSettings.minimized = b;

        ShowWindow(sInstance->mWin32Handle, sInstance->mSettings.minimized ? SW_MINIMIZE : (sInstance->mSettings.maximized && sInstance->mSettings.windowMode == WindowMode::WINDOWED) ? SW_MAXIMIZE : SW_NORMAL);
    }

    bool8 Window::isMaximized()
    {
        return sInstance->mSettings.maximized || sInstance->mSettings.windowMode == WindowMode::WINDOWED_FULLSCREEN || sInstance->mSettings.windowMode == WindowMode::FULLSCREEN ||
            (sInstance->mSettings.windowMode == WindowMode::WINDOWED_BORDERLESS && sInstance->mSettings.width == sInstance->mSettings.monitorWidth && sInstance->mSettings.width == sInstance->mSettings.monitorHeight);
    }

    bool8 Window::isMinimized()
    {
        return sInstance->mSettings.minimized;
    }

    float32 Window::getWidth()
    {
        return float32(sInstance->mSettings.width);
    }

    float32 Window::getHeight()
    {
        return float32(sInstance->mSettings.height);
    }

    float32 Window::getMonitorWidth()
    {
        return float32(sInstance->mSettings.monitorWidth);
    }

    float32 Window::getMonitorHeight()
    {
        return float32(sInstance->mSettings.monitorHeight);
    }

    WindowMode Window::getWindowMode()
    {
        return sInstance->mSettings.windowMode;
    }

    void Window::setWindowMode(WindowMode mode)
    {
        if (mode == sInstance->mSettings.windowMode)
            return;

        sInstance->mIgnoreSizeMessage = true;

        if (mode == WindowMode::FULLSCREEN )
            _setFullscreen(true);
        else if (sInstance->mSettings.windowMode == WindowMode::FULLSCREEN )
            _setFullscreen(false);

        if (mode == WindowMode::WINDOWED)
            _setDecorated(true);
        else if (sInstance->mSettings.windowMode == WindowMode::WINDOWED)
            _setDecorated(false);

        sInstance->mSettings.windowMode = mode;

        _updateWindowSize();
    }

    void Window::setClearColor(const Color& color)
    {
        sInstance->mSettings.clearColor = color;

        GraphicsAPI::setClearColor(color);
    }

    void Window::setCursor(const String& name)
    {
        RECT clientArea;
        POINT cursorPos;

        if (!GetCursorPos(&cursorPos))
            return;

        if (WindowFromPoint(cursorPos) != sInstance->mWin32Handle)
            return;

        GetClientRect(sInstance->mWin32Handle, &clientArea);
        ClientToScreen(sInstance->mWin32Handle, (POINT*)&clientArea.left);
        ClientToScreen(sInstance->mWin32Handle, (POINT*)&clientArea.right);

        if (!PtInRect(&clientArea, cursorPos))
            return;

        if (sInstance->mCursors.containsKey(name))
            SetCursor(sInstance->mCurrentCursor = sInstance->mCursors.get(name));
    }

    void Window::clear()
    {
        GraphicsAPI::clear();
    }

    void Window::swapBuffers()
    {
        Context::swapBuffers();
    }

    void Window::saveSettings()
    {
        FileUtils::createDirectoryRecursively(sInstance->mSettingsFilePath);
        WindowSettingsLoader::saveWindowSettingsToOtto(sInstance->mSettings, sInstance->mInitialSettings, sInstance->mSettingsFilePath);
    }

    void Window::destroy()
    {
        Log::trace("Destroying Window...");

        if (sInstance->mSettings.windowMode == WindowMode::FULLSCREEN )
            _setFullscreen(false);

        if (sInstance->mSmallIconHandle)
            DestroyIcon(sInstance->mSmallIconHandle);
        if (sInstance->mLargeIconHandle)
            DestroyIcon(sInstance->mLargeIconHandle);

        for (auto& [_, cursor] : sInstance->mCursors)
            DestroyCursor(cursor);

        DestroyWindow(sInstance->mWin32Handle);
    }

    void Window::_initCursors(const String& defaultCursor)
    {
        for (auto& [name, cursor] : sInstance->mSettings.cursorSet)
        {
            HCURSOR handle = HCURSOR(_createNativeIcon(cursor.width, cursor.height, cursor.bitmap, true, cursor.hotspotX, cursor.hotspotY));
            if (handle)
                sInstance->mCursors.insert(name, handle);
        }

        sInstance->mCurrentCursor = sInstance->mCursors.get(defaultCursor);
    }

    void Window::_initIcons()
    {
        uint16 smallIconWidth = uint16(GetSystemMetrics(SM_CXSMICON));
        uint16 smallIconHeight = uint16(GetSystemMetrics(SM_CYSMICON));
        uint16 largeIconWidth = uint16(GetSystemMetrics(SM_CXICON));
        uint16 largeIconHeight = uint16(GetSystemMetrics(SM_CYICON));

        Icon smallIcon;
        float64 smallestSizeDifference = 1.0e35;
        for (auto& [name, icon] : sInstance->mSettings.iconSet)
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
        for (auto& [name, icon] : sInstance->mSettings.iconSet)
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
            sInstance->mSmallIconHandle = _createNativeIcon(smallIcon.width, smallIcon.height, smallIcon.bitmap);
        if (largeIcon.bitmap)
            sInstance->mLargeIconHandle = _createNativeIcon(largeIcon.width, largeIcon.height, largeIcon.bitmap);

        if (sInstance->mSmallIconHandle)
            SendMessage(sInstance->mWin32Handle, WM_SETICON, ICON_SMALL, LPARAM(sInstance->mSmallIconHandle));
        if (sInstance->mLargeIconHandle)
            SendMessage(sInstance->mWin32Handle, WM_SETICON, ICON_BIG, LPARAM(sInstance->mLargeIconHandle));
    }

    void Window::_initStyle()
    {
        sInstance->mSettings.windowedStyle = (WS_OVERLAPPED | WS_CAPTION);
        sInstance->mSettings.windowedExStyle = 0;

        if (sInstance->mSettings.closeButton)
            sInstance->mSettings.windowedStyle |= WS_SYSMENU;

        if (sInstance->mSettings.resizable)
            sInstance->mSettings.windowedStyle |= (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
    }

    void Window::_setDecorated(bool8 decorated)
    {
        sInstance->mIgnoreSizeMessage = true;
        SetWindowLong(sInstance->mWin32Handle, GWL_STYLE, decorated ? sInstance->mSettings.windowedStyle : 0);
        SetWindowLong(sInstance->mWin32Handle, GWL_EXSTYLE, decorated ? sInstance->mSettings.windowedExStyle : 0);
        sInstance->mIgnoreSizeMessage = false;
    }

    void Window::_updateWindowSize()
    {
        if (sInstance->mSettings.windowMode == WindowMode::WINDOWED)
        {
            RECT newWindow = { (sInstance->mSettings.monitorWidth - sInstance->mSettings.unmaximizedWidth) / 2, (sInstance->mSettings.monitorHeight - sInstance->mSettings.unmaximizedHeight) / 2,
                (sInstance->mSettings.monitorWidth + sInstance->mSettings.unmaximizedWidth) / 2, (sInstance->mSettings.monitorHeight + sInstance->mSettings.unmaximizedHeight) / 2 };
            AdjustWindowRectEx(&newWindow, sInstance->mSettings.windowedStyle, FALSE, sInstance->mSettings.windowedExStyle);

            SetWindowPos(sInstance->mWin32Handle, HWND_TOP, newWindow.left, newWindow.top,
                newWindow.right - newWindow.left, newWindow.bottom - newWindow.top, SWP_NOZORDER);

            ShowWindow(sInstance->mWin32Handle, sInstance->mSettings.maximized ? SW_MAXIMIZE : sInstance->mSettings.minimized ? SW_MINIMIZE : SW_NORMAL);
        }
        else if (sInstance->mSettings.windowMode == WindowMode::WINDOWED_BORDERLESS)
        {
            SetWindowPos(sInstance->mWin32Handle, HWND_TOP, (sInstance->mSettings.monitorWidth - sInstance->mSettings.unmaximizedWidth) / 2, (sInstance->mSettings.monitorHeight - sInstance->mSettings.unmaximizedHeight) / 2,
                sInstance->mSettings.unmaximizedWidth, sInstance->mSettings.unmaximizedHeight, SWP_NOZORDER);

            ShowWindow(sInstance->mWin32Handle, sInstance->mSettings.minimized ? SW_MINIMIZE : SW_SHOW);
        }
        else
        {
            SetWindowPos(sInstance->mWin32Handle, HWND_TOP, 0, 0, sInstance->mSettings.monitorWidth, sInstance->mSettings.monitorHeight, SWP_NOZORDER);
            ShowWindow(sInstance->mWin32Handle, sInstance->mSettings.minimized ? SW_MINIMIZE : SW_SHOW);
        }
    }

    void Window::_adjustSize(uint16 width, uint16 height)
    {
        sInstance->mSettings.unmaximizedWidth = width;
        sInstance->mSettings.unmaximizedHeight = height;

        _updateWindowSize();
    }

    __int64 __stdcall Window::WindowProc(HWND__* handle, unsigned int msg, unsigned __int64 wParam, __int64 lParam)
    {
        switch (msg)
        {
        case WM_CLOSE: [[fallthrough]];
        case WM_QUIT:
            sInstance->mListeners.onWindowClosed(_WindowClosedEvent());
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
            Mouse::sY = sInstance->mSettings.height - float32(GET_Y_LPARAM(lParam)) - 1;

            float32 deltaX = Mouse::sX - beforeX;
            float32 deltaY = Mouse::sY - beforeY;

            for (uint64 button = 0; button < Mouse::sButtons.getSize(); button++)
                if (Mouse::sButtons[button])
                    sInstance->mListeners.onMouseDragged(_MouseDraggedEvent(MouseButton(button), Mouse::sX, Mouse::sY, beforeX, beforeY, deltaX, deltaY));

            sInstance->mListeners.onMouseMoved(_MouseMovedEvent(Mouse::sX, Mouse::sY, beforeX, beforeY, deltaX, deltaY));

            return 0;
        }

        case WM_MOUSEWHEEL:
            sInstance->mListeners.onMouseScrolled(_MouseScrolledEvent(Mouse::sX, Mouse::sY, 0.0f, float32(GET_WHEEL_DELTA_WPARAM(wParam)) / WHEEL_DELTA));
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
            sInstance->mListeners.onKeyPressed(_KeyPressedEvent(key, keyChar));

            if (sInstance->mSettings.closeOnAltF4 && key == Key::F4 && Keyboard::isKeyPressed(Key::ALT))
                sInstance->mListeners.onWindowClosed(_WindowClosedEvent());

            return 0;
        }

        case WM_SYSKEYUP: [[fallthrough]];
        case WM_KEYUP:
        {
            Key key = _getKey(lParam, wParam);

            Keyboard::sKeys[uint64(key)] = false;
            sInstance->mListeners.onKeyReleased(_KeyReleasedEvent(key));

            return 0;
        }

        case WM_SIZE:
            if (!sInstance->mIgnoreSizeMessage)
            {
                if (wParam == SIZE_MINIMIZED)
                    sInstance->mSettings.minimized = true;
                else if (sInstance->mSettings.windowMode == WindowMode::WINDOWED && wParam == SIZE_MAXIMIZED)
                    sInstance->mSettings.maximized = true;

                _onResized(uint16(LOWORD(lParam)), uint16(HIWORD(lParam)));
            }

            return 0;

        case WM_KILLFOCUS:
            sInstance->mListeners.onWindowLostFocus(_WindowLostFocusEvent());

            if (sInstance->mSettings.windowMode == WindowMode::FULLSCREEN )
                ShowWindow(sInstance->mWin32Handle, SW_MINIMIZE);

            return 0;

        case WM_SETFOCUS:
            Mouse::sButtons = { false };
            Keyboard::sKeys = { false };

            sInstance->mListeners.onWindowGainedFocus(_WindowGainedFocusEvent());
            return 0;

        case WM_SETCURSOR:
            if (LOWORD(lParam) == HTCLIENT)
            {
                SetCursor(sInstance->mCurrentCursor);
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
