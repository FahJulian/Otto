#include "window.h"

#include "_win32_include.h"

#include "otto/debug/log/log.h"
#include "otto/core/application.h"
#include "otto/window/events.h"
#include "otto/window/window_settings_loader.h"

namespace otto
{
    static WindowSettings sSettings;

    static int sCurrentWidht;
    static int sCurrentHeight;
    static int sUnmaximizedWidth;
    static int sUnmaximizedHeight;
    static String sTitle;
    static WindowMode sWindowMode;
    static Color sClearColor;
    static bool sCloseOnAltF4 = true;

    static HWND sWin32Handle;

    static DWORD sWindowedStyle;
    static DWORD sWindowedExStyle;

    static int sMonitorWidth;
    static int sMonitorHeight;

    static bool sMaximized;
    static bool sMinimized;

    static bool sIgnoreSizeMessage = false;

    static HDC sDeviceContext;

    static HICON smallIconHandle;
    static HICON largeIconHandle;

    static HCURSOR sCurrentCursor;

    static Map<String, HCURSOR> sCursors;

    static FilePath sSettingsFilePath;


    std::wstring toWideString(const String& string)
    {
        std::wstring wideString = std::wstring(string.getSize(), wchar_t(0));
        MultiByteToWideChar(CP_ACP, 0, string.getData(), -1, static_cast<LPWSTR>(wideString.data()), static_cast<uint32>(string.getSize()));
        return wideString;
    }

    static void initCursors();
    static void initIcons();
    static void initStyle();
    static HICON createNativeIcon(int width, int height, uint8_t* bitmap, int isCursor = FALSE, int hotspotX = 0, int hotspotY = 0);
    static BITMAPV5HEADER createBitmapHeader(int width, int height);
    static void setDecorated(bool decorated);
    static void adjustSize(int width, int height);
    static void setFullscreen(bool fullscreen);
    static void updateWindowSize();
    static Key getKey(LPARAM lParam, WPARAM wParam);


    bool Window::init(const FilePath& filePath)
    {
        sSettingsFilePath = filePath;

        auto result = WindowSettingsLoader::loadWindowSettings(filePath);
        if (result.hasError())
            return false;

        sSettings = result.getResult();
        sUnmaximizedWidth = sSettings.width;
        sUnmaximizedHeight = sSettings.height;
        sWindowMode = sSettings.windowMode;
        sTitle = sSettings.title;

        if (!_createWin32Window())
            return false;

        initStyle();

        sMonitorWidth = GetSystemMetrics(SM_CXSCREEN);
        sMonitorHeight = GetSystemMetrics(SM_CYSCREEN);

        setFullscreen(sWindowMode == WindowMode::FULLSCREEN);
        setDecorated(sWindowMode == WindowMode::WINDOWED);

        sMaximized = sSettings.maximized;
        sMinimized = !sMaximized && sSettings.minimized;

        updateWindowSize();

        initCursors();
        initIcons();

        if (!_createContext())
            return false;

        setClearColor(sClearColor = sSettings.clearColor);

        return true;
    }

    bool Window::_createWin32Window()
    {
        const wchar_t* className = L"Sample Window Class";
        HMODULE win32ModuleHandle = GetModuleHandle(0);

        WNDCLASSEX win32WindowClass = { };
        win32WindowClass.cbSize = sizeof(WNDCLASSEX);
        win32WindowClass.lpfnWndProc = Window::WindowProc;
        win32WindowClass.hInstance = win32ModuleHandle;
        win32WindowClass.lpszClassName = className;
        win32WindowClass.style = CS_OWNDC;

        RegisterClassEx(&win32WindowClass);

        sWin32Handle = CreateWindowEx(0, className, toWideString(sSettings.title).c_str(), 0,
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

    void Window::_onResized(int width, int height)
    {
        if (sWindowMode == WindowMode::WINDOWED && !sMinimized && !sMaximized)
        {
            sUnmaximizedWidth = width;
            sUnmaximizedHeight = height;
        }

        if (sCurrentWidht != width || sCurrentHeight != height)
        {
            sCurrentWidht = width;
            sCurrentHeight = height;

            _WindowResizedEvent e = { static_cast<float>(width), static_cast<float>(height) };
            //EventDispatcher::dispatch(e);
            //App::onWindowResized(e);
        }
    }

    void Window::_onMouseButtonPressed(MouseButton button)
    {
        Mouse::sButtons[static_cast<uint64>(button)] = true;
        //EventDispatcher::dispatch(MouseButtonPressedEvent(button, Mouse::sX, Mouse::sY));
        SetCapture(sWin32Handle);
    }

    void Window::_onMouseButtonReleased(MouseButton button)
    {
        Mouse::sButtons[static_cast<uint64>(button)] = false;
        //EventDispatcher::dispatch(MouseButtonReleasedEvent(button, Mouse::sX, Mouse::sY));
        ReleaseCapture();
    }

    void Window::setTitle(const String& title)
    {
        sTitle = title;
        SetWindowText(sWin32Handle, toWideString(title).c_str());
    }

    String Window::getTitle()
    {
        return sTitle;
    }

    void Window::setWidth(float width)
    {
        adjustSize((int)(width + 0.5f), sCurrentHeight);
    }

    void Window::setHeight(float height)
    {
        adjustSize(sCurrentWidht, (int)(height + 0.5f));
    }

    void Window::setSize(float width, float height)
    {
        adjustSize((int)(width + 0.5f), int(height + 0.5f));
    }

    void Window::setMaximized(bool b)
    {
        sMaximized = b;

        if (sWindowMode == WindowMode::WINDOWED)
            ShowWindow(sWin32Handle, sMaximized ? SW_MAXIMIZE : SW_NORMAL);
    }

    void Window::setMinimized(bool b)
    {
        sMinimized = b;

        ShowWindow(sWin32Handle, sMinimized ? SW_MINIMIZE : (sMaximized && sWindowMode == WindowMode::WINDOWED) ? SW_MAXIMIZE : SW_NORMAL);
    }

    bool Window::isMaximized()
    {
        return sMaximized || sWindowMode == WindowMode::WINDOWED_FULLSCREEN || sWindowMode == WindowMode::FULLSCREEN ||
            (sWindowMode == WindowMode::WINDOWED_BORDERLESS && sCurrentWidht == sMonitorWidth && sCurrentWidht == sMonitorHeight);
    }

    bool Window::isMinimized()
    {
        return sMinimized;
    }

    float Window::getWidth()
    {
        return static_cast<float>(sCurrentWidht);
    }

    float Window::getHeight()
    {
        return static_cast<float>(sCurrentHeight);
    }

    float Window::getMonitorWidth()
    {
        return (float)sMonitorWidth;
    }

    float Window::getMonitorHeight()
    {
        return (float)sMonitorHeight;
    }

    static WindowMode getWindowMode()
    {
        return sWindowMode;
    }

    void Window::setWindowMode(WindowMode mode)
    {
        if (mode == sWindowMode)
            return;

        sIgnoreSizeMessage = true;

        if (mode == WindowMode::FULLSCREEN )
            setFullscreen(true);
        else if (sWindowMode == WindowMode::FULLSCREEN )
            setFullscreen(false);

        if (mode == WindowMode::WINDOWED)
            setDecorated(true);
        else if (sWindowMode == WindowMode::WINDOWED)
            setDecorated(false);

        sWindowMode = mode;

        updateWindowSize();
    }

    void Window::setClearColor(const Color& color)
    {
        sClearColor = color;
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

    void Window::saveInfo()
    {
        if (sSettings.saveSize)
        {
            sSettings.width = sUnmaximizedWidth;
            sSettings.height = sUnmaximizedHeight;
        }

        if (sSettings.saveTitle)
            sSettings.title = sTitle;
        if (sSettings.saveWindowMode)
            sSettings.windowMode = sWindowMode;
        if (sSettings.saveClearColor)
            sSettings.clearColor = sClearColor;
        if (sSettings.saveMaximized)
            sSettings.maximized = sMaximized;
        if (sSettings.saveMinimized)
            sSettings.minimized = sMinimized;

        WindowSettingsLoader::saveWindowSettings(sSettings, sSettingsFilePath);
    }

    void Window::destroy()
    {
        if (sWindowMode == WindowMode::FULLSCREEN )
            setFullscreen(false);

        if (smallIconHandle)
            DestroyIcon(smallIconHandle);
        if (largeIconHandle)
            DestroyIcon(largeIconHandle);

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
        {
            _WindowClosedEvent e;
            //EventDispatcher::dispatch(e);
            //App::onWindowClosed(e);

            return 0;
        }

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
            float beforeX = Mouse::sX;
            float beforeY = Mouse::sY;

            Mouse::sX = (float)GET_X_LPARAM(lParam);
            Mouse::sY = sCurrentHeight - (float)GET_Y_LPARAM(lParam) - 1;

            float deltaX = Mouse::sX - beforeX;
            float deltaY = Mouse::sY - beforeY;

            for (uint64 button = 0; button < Mouse::sButtons.getSize(); button++)
                if (Mouse::sButtons[button])
                    ;
                    //EventDispatcher::dispatch(MouseDraggedEvent(button, Mouse::sX, Mouse::sY, beforeX, beforeY, deltaX, deltaY));

            //EventDispatcher::dispatch(_MouseMovedEvent(Mouse::sX, Mouse::sY, beforeX, beforeY, deltaX, deltaY));

            return 0;
        }

        case WM_MOUSEWHEEL:
            //EventDispatcher::dispatch(MouseScrolledEvent(Mouse::sX, Mouse::sY, 0.0f, (float)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA));
            return 0;

        case WM_SYSKEYDOWN: [[fallthrough]];
        case WM_KEYDOWN:
        {
            Key key = getKey(lParam, wParam);

            wchar_t keyChar = 0;
            if (MSG charMessage;
                PeekMessage(&charMessage, NULL, WM_CHAR, WM_CHAR, PM_REMOVE))
            {
                keyChar = *((wchar_t*)&charMessage.wParam);
            }

            Keyboard::sKeys[static_cast<uint64>(key)] = true;
            //EventDispatcher::dispatch(_KeyPressedEvent(key, keyChar));

            if (sSettings.closeOnAltF4 && key == Key::F4 && Keyboard::isKeyPressed(Key::ALT))
                ;
                //EventDispatcher::dispatch(_WindowClosedEvent());

            return 0;
        }

        case WM_SYSKEYUP: [[fallthrough]];
        case WM_KEYUP:
        {
            Key key = getKey(lParam, wParam);

            Keyboard::sKeys[static_cast<uint64>(key)] = false;
            //EventDispatcher::dispatch(_KeyReleasedEvent(key));

            return 0;
        }

        case WM_SIZE:
            if (!sIgnoreSizeMessage)
            {
                if (wParam == SIZE_MINIMIZED)
                    sMinimized = true;
                else if (sWindowMode == WindowMode::WINDOWED && wParam == SIZE_MAXIMIZED)
                    sMaximized = true;

                _onResized((int)LOWORD(lParam), (int)HIWORD(lParam));
            }
            return 0;

        case WM_KILLFOCUS:
            //EventDispatcher::dispatch(_WindowLostFocusEvent());

            if (sWindowMode == WindowMode::FULLSCREEN )
                ShowWindow(sWin32Handle, SW_MINIMIZE);

            return 0;

        case WM_SETFOCUS:
            Mouse::sButtons = { false };
            Keyboard::sKeys = { false };

            //EventDispatcher::dispatch(_WindowGainedFocusEvent());
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


    static void initStyle()
    {
        sWindowedStyle = (WS_OVERLAPPED | WS_CAPTION);
        sWindowedExStyle = 0;

        if (sSettings.closeButton)
            sWindowedStyle |= WS_SYSMENU;

        if (sSettings.resizable)
            sWindowedStyle |= (WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_THICKFRAME);
    }

    static void initCursors()
    {
        for (auto& [name, cursor] : sSettings.cursors)
        {
            if (HCURSOR handle = (HCURSOR)createNativeIcon(cursor.width, cursor.height, cursor.bitmap, TRUE, cursor.hotspotX, cursor.hotspotY);
                handle)
                sCursors.insert(name, handle);
        }

        //sCurrentCursor = sCursors[Cursors::Arrow];
    }

    static void initIcons()
    {
        int smallIconWidth = GetSystemMetrics(SM_CXSMICON);
        int smallIconHeight = GetSystemMetrics(SM_CYSMICON);
        int largeIconWidth = GetSystemMetrics(SM_CXICON);
        int largeIconHeight = GetSystemMetrics(SM_CYICON);

        Icon smallIcon;
        double smallestSizeDifference = 1.0e35;
        for (auto& [name, icon] : sSettings.icons)
        {
            int xoff = smallIconWidth - icon.width;
            int yoff = smallIconHeight - icon.height;

            double offset = sqrt(xoff * xoff + yoff * yoff);
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
            int xoff = largeIconWidth - icon.width;
            int yoff = largeIconHeight - icon.height;

            double offset = sqrt(xoff * xoff + yoff * yoff);
            if (offset < smallestSizeDifference)
            {
                smallestSizeDifference = offset;
                largeIcon = icon;
            }
        }

        if (smallIcon.bitmap)
            smallIconHandle = createNativeIcon(smallIcon.width, smallIcon.height, smallIcon.bitmap);
        if (largeIcon.bitmap)
            largeIconHandle = createNativeIcon(largeIcon.width, largeIcon.height, largeIcon.bitmap);

        if (smallIconHandle)
            SendMessage(sWin32Handle, WM_SETICON, ICON_SMALL, (LPARAM)smallIconHandle);
        if (largeIconHandle)
            SendMessage(sWin32Handle, WM_SETICON, ICON_BIG, (LPARAM)largeIconHandle);
    }

    static HICON createNativeIcon(int width, int height, uint8_t* bitmap, int isCursor, int hotspotX, int hotspotY)
    {
        if (!bitmap)
            return NULL;

        HICON handle;
        HBITMAP color, mask;
        BITMAPV5HEADER bi = createBitmapHeader(width, height);
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

    static BITMAPV5HEADER createBitmapHeader(int width, int height)
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

    static void setDecorated(bool decorated)
    {
        sIgnoreSizeMessage = true;
        SetWindowLong(sWin32Handle, GWL_STYLE, decorated ? sWindowedStyle : 0);
        SetWindowLong(sWin32Handle, GWL_EXSTYLE, decorated ? sWindowedExStyle : 0);
        sIgnoreSizeMessage = false;
    }

    static void adjustSize(int width, int height)
    {
        sUnmaximizedWidth = (int)width;
        sUnmaximizedHeight = height;

        updateWindowSize();
    }

    static void setFullscreen(bool fullscreen)
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

    static void updateWindowSize()
    {
        if (sWindowMode == WindowMode::WINDOWED)
        {
            RECT newWindow = { (sMonitorWidth - sUnmaximizedWidth) / 2, (sMonitorHeight - sUnmaximizedHeight) / 2,
                (sMonitorWidth + sUnmaximizedWidth) / 2, (sMonitorHeight + sUnmaximizedHeight) / 2 };
            AdjustWindowRectEx(&newWindow, sWindowedStyle, FALSE, sWindowedExStyle);

            SetWindowPos(sWin32Handle, HWND_TOP, newWindow.left, newWindow.top,
                newWindow.right - newWindow.left, newWindow.bottom - newWindow.top, SWP_NOZORDER);

            ShowWindow(sWin32Handle, sMaximized ? SW_MAXIMIZE : sMinimized ? SW_MINIMIZE : SW_NORMAL);
        }
        else if (sWindowMode == WindowMode::WINDOWED_BORDERLESS)
        {
            SetWindowPos(sWin32Handle, HWND_TOP, (sMonitorWidth - sUnmaximizedWidth) / 2, (sMonitorHeight - sUnmaximizedHeight) / 2,
                sUnmaximizedWidth, sUnmaximizedHeight, SWP_NOZORDER);

            ShowWindow(sWin32Handle, sMinimized ? SW_MINIMIZE : SW_SHOW);
        }
        else
        {
            SetWindowPos(sWin32Handle, HWND_TOP, 0, 0, sMonitorWidth, sMonitorHeight, SWP_NOZORDER);
            ShowWindow(sWin32Handle, sMinimized ? SW_MINIMIZE : SW_SHOW);
        }
    }

    static Key getKey(LPARAM lParam, WPARAM wParam)
    {
        Key key = *((Key*)&wParam);

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

} // namespace otto
