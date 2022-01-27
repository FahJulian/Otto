#include "application.h"

#include <iostream>

#include "otto/util/dll_reloading/dll_reloading.h"
#include "otto/util/platform/file_utils.h"
#include "otto/serialization/serializer.h"
#include "otto/core/platform/time.h"
#include "otto/scene/scene_loader.h"
#include "otto/debug/log/log.h"
#include "otto/scene/scene.h"
#include "otto/util/file.h"

namespace otto
{
    struct LogLevelParsingError
    {
    } sLogLevelParsingError;

    static bool sInitialized = false;
    static Application* sApplication;
    static const FilePath CORE_ROOT_DIRECTORY = "C:/ProgramData/Otto/";

    namespace
    {
        Result<uint8, LogLevelParsingError> _parseLogLevel(const String& logLevel)
        {
            if (logLevel == "ALL")
                return uint8(Log::ALL);
            else if (logLevel == "DISABLED")
                return uint8(Log::DISABLED);
            else if (logLevel == "FATAL")
                return uint8(Log::FATAL);
            else if (logLevel == "ERROR")
                return uint8(Log::ERROR);
            else if (logLevel == "WARM")
                return uint8(Log::WARN);
            else if (logLevel == "INFO")
                return uint8(Log::INFO);
            else if (logLevel == "DEBUG")
                return uint8(Log::DEBUG);
            else if (logLevel == "TRACE")
                return uint8(Log::TRACE);

            return sLogLevelParsingError;
        }

    } // namespace

    bool Application::init(const FilePath& settingsFilePath)
    {
        if (sInitialized)
            return false;

        sApplication = new Application();

        Time::init();

        auto result = _loadSettings(settingsFilePath);
        if (result.hasError())
            return false;

        auto settings = result.getResult();

        sApplication->mRootDirectory = settings.rootDirectory;

        Log::init(getRootDirectory() + settings.logFilePath, std::cout, settings.logConsoleLevel, settings.logFileLevel);

        SceneLoader::init(settings.components, settings.behaviours, settings.systems, settings.events);

#ifdef OTTO_DYNAMIC

        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // Had to copy everything from C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x64 to 
        // CORE_ROOT_DIRECTORY + "MSVC/" + settings.msvcVersion + "/bin/Hostx86/x64/cl.exe"
        // for the linker to work
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        DllReloader::Settings dllSettings;
        dllSettings.compilerExePath = CORE_ROOT_DIRECTORY + "MSVC/" + settings.msvcVersion + "/bin/Hostx86/x64/cl.exe";
        dllSettings.linkerExePath = CORE_ROOT_DIRECTORY + "MSVC/" + settings.msvcVersion + "/bin/Hostx86/x64/link.exe";
        dllSettings.tmpDir = sApplication->mRootDirectory + ".tmp/";
        dllSettings.includeDirs = { 
            "C:/dev/Otto/Otto/src",
            sApplication->mRootDirectory,
            CORE_ROOT_DIRECTORY + "WindowsKits/10.0.19041.0/include/shared",
            CORE_ROOT_DIRECTORY + "WindowsKits/10.0.19041.0/include/ucrt",
            CORE_ROOT_DIRECTORY + "WindowsKits/10.0.19041.0/include/um",
            CORE_ROOT_DIRECTORY + "WindowsKits/10.0.19041.0/include/winrt",
            CORE_ROOT_DIRECTORY + "WindowsKits/10.0.19041.0/include/cppwinrt",
            CORE_ROOT_DIRECTORY + "MSVC/" + settings.msvcVersion + "/include",
            //"C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.29.30133/include",
        };
        dllSettings.libDirs = { 
            CORE_ROOT_DIRECTORY + "WindowsKits/10.0.19041.0/lib/ucrt/x64",
            //CORE_ROOT_DIRECTORY + "WindowsKits/10.0.19041.0/lib/ucrt_enclave/x64",
            CORE_ROOT_DIRECTORY + "WindowsKits/10.0.19041.0/lib/um/x64",
            CORE_ROOT_DIRECTORY + "MSVC/" + settings.msvcVersion + "lib/x64/uwp",
            CORE_ROOT_DIRECTORY + "MSVC/" + settings.msvcVersion + "/lib/x64",
            "C:/dev/Otto/bin/Debug/x64",
        };
        dllSettings.configuration = DllReloader::Configuration::DEBUG;
        dllSettings.libs = { "otto.lib" };

        DllReloader::init(dllSettings);

        SceneLoader::reloadDll();
#endif

        sInitialized = true;

        return true;
    }

    const FilePath& Application::getRootDirectory()
    {
        return sApplication->mRootDirectory;
    }

    const FilePath& Application::getCoreRootDirectory()
    {
        return CORE_ROOT_DIRECTORY;
    }

    void Application::destroy()
    {
#ifdef OTTO_DYNAMIC
        DllReloader::destroy();
#endif
    }

    Result<Application::Settings, Application::SettingsError> Application::_loadSettings(const FilePath& filePath)
    {
        Application::Settings settings;

        auto result = Serializer::deserialize(filePath);
        if (result.hasError())
            return SettingsError::SYNTAX_ERROR;

        auto file = result.getResult();

        if (!file.contains("RootDirectory"))
            return SettingsError::ROOT_DIRECTORY_NOT_FOUND;
        else
            settings.rootDirectory = file.get<String>("RootDirectory");

        if (!file.contains("LogFilePath"))
            return SettingsError::LOG_FILE_PATH_NOT_FOUND;
        else
            settings.logFilePath = file.get<String>("LogFilePath");

        if (!file.contains("MSVC_Version"))
            return SettingsError::MSVC_VERSION_NOT_FOUND;
        else
            settings.msvcVersion = file.get<String>("MSVC_Version");

        if (file.contains("LogConsoleLevel"))
        {
            auto logConsoleLevel = _parseLogLevel(file.get<String>("LogConsoleLevel"));
            if (!logConsoleLevel.hasError())
                settings.logConsoleLevel = logConsoleLevel.getResult();
        }

        if (file.contains("LogFileLevel"))
        {
            auto logFileLevel = _parseLogLevel(file.get<String>("LogFileLevel"));
            if (!logFileLevel.hasError())
                settings.logFileLevel = logFileLevel.getResult();
        }

        if (file.contains("Components"))
        {
            auto components = file.get("Components");
            if (components.isList())
                settings.components = components.getList<String>();
        }

        if (file.contains("Behaviours"))
        {
            auto behaviours = file.get("Behaviours");
            if (behaviours.isList())
                settings.behaviours = behaviours.getList<String>();
        }

        if (file.contains("Systems"))
        {
            auto systems = file.get("Systems");
            if (systems.isList())
                settings.systems = systems.getList<String>();
        }

        if (file.contains("Events"))
        {
            auto events = file.get("Events");
            if (events.isList())
                settings.events = events.getList<String>();
        }

        return settings;
    }

#ifdef OTTO_DYNAMIC
    FilePath Application::_getClientDllPath()
    {
        return sApplication->mRootDirectory + ".tmp/client.dll";
    }
#endif

} // namespace otto
