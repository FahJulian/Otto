#pragma once

#include "otto/base.h"
#include "otto/util/result.h"
#include "otto/debug/log/log.h"
#include "otto/util/platform/file_path.h"
#include "otto/window/events.h"

extern int main();

namespace otto
{
    class Application
    {
    private:
        struct Settings
        {
            FilePath rootDirectory;
            FilePath logFilePath;
            uint8 logConsoleLevel = Log::ALL;
            uint8 logFileLevel = Log::ALL;
            String msvcVersion;
            String startScene;
            FilePath windowSettingsPath;
            DynamicArray<String> components;
            DynamicArray<String> behaviours;
            DynamicArray<String> systems;
            DynamicArray<String> events;
        };

        enum class SettingsError : uint8
        {
            SYNTAX_ERROR,
            WINDOW_SETTINGS_PATH_NOT_FOUND,
            ROOT_DIRECTORY_NOT_FOUND,
            LOG_FILE_PATH_NOT_FOUND,
            MSVC_VERSION_NOT_FOUND,
            START_SCENE_NOT_FOUND,
        };

        Application()
        {
        }

    public:
        static const FilePath& getRootDirectory();
        static const FilePath& getCoreRootDirectory();

#ifdef OTTO_DYNAMIC
        static FilePath _getClientDllPath();
#endif

    private:
        static bool init(const FilePath& settingsFilePath);

        static void run();

        static void stop();

        static void destroy();

        static Result<Settings, SettingsError> _loadSettings(const FilePath& filePath);

        static void _onKeyPressed(const _KeyPressedEvent& e);
        static void _onKeyReleased(const _KeyReleasedEvent& e);
        static void _onMouseButtonPressed(const _MouseButtonPressedEvent& e);
        static void _onMouseButtonReleased(const _MouseButtonReleasedEvent& e);
        static void _onMouseMoved(const _MouseMovedEvent& e);
        static void _onMouseDragged(const _MouseDraggedEvent& e);
        static void _onMouseScrolled(const _MouseScrolledEvent& e);
        static void _onWindowClosed(const _WindowClosedEvent& e);
        static void _onWindowResized(const _WindowResizedEvent& e);
        static void _onWindowGainedFocus(const _WindowGainedFocusEvent& e);
        static void _onWindowLostFocus(const _WindowLostFocusEvent& e);

        FilePath mRootDirectory;

        friend int ::main();
        friend class Scene;
    };

} // namespace otto
