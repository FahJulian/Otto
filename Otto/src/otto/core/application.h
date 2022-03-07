#pragma once

#include "otto/base.h"
#include "otto/debug/log.h"
#include "otto/util/result.h"
#include "otto/core/package.h"
#include "otto/window/window_events.h"
#include "otto/window/window.h"
#include "otto/util/file_path.h"
#include "otto/serialization/serialized.h"

extern int main();

namespace otto
{
    class Application
    {
    private:
        Application() = default;

        struct Settings
        {
            FilePath rootDirectory;
            FilePath logFilePath;
            uint8 logConsoleLevel = Log::ALL;
            uint8 logFileLevel = Log::ALL;
            String msvcVersion;
            String startScene;
            FilePath windowSettingsPath;
            Package applicationPackage;
            float64 updateCap = 60.0f;
            Map<String, Serialized> additionalSettings;
        };

        enum class SettingsError : uint8
        {
            SYNTAX_ERROR,
            WINDOW_SETTINGS_PATH_NOT_FOUND,
            ROOT_DIRECTORY_NOT_FOUND,
            LOG_FILE_PATH_NOT_FOUND,
            MSVC_VERSION_NOT_FOUND,
            START_SCENE_NOT_FOUND,
            PACKAGE_ERROR,
        };

    public:
        static const FilePath& getRootDirectory();
        static const FilePath& getCoreRootDirectory();

        static const Map<String, Serialized>& getSettings();

#ifdef OTTO_DYNAMIC
        static FilePath _getClientDllPath();
#endif

    private:
        static Application* getInstance();

        static bool8 init(Application* mainApplication, Window* mainWindow, Log* mainLog);

        static bool8 init(const FilePath& settingsFilePath);

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

    private:
        bool8 mRunning = false;
        FilePath mRootDirectory;
        Map<String, Serialized> mAdditionalSettings;
        float64 mSecondsPerUpdate;

        friend class Scene;
        friend int ::main();
    };

} // namespace otto
