#pragma once

#include "otto/base.h"
#include "otto/util/result.h"
#include "otto/debug/log/log.h"
#include "otto/util/platform/file_path.h"

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

        static void destroy();

        static Result<Settings, SettingsError> _loadSettings(const FilePath& filePath);

        FilePath mRootDirectory;

        friend int ::main();
        friend class Scene;
    };

} // namespace otto
