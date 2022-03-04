#include "application.h"

#include <iostream>

#include "otto/util/dll_reloading/dll_reloader.h"
#include "otto/util/platform/file_utils.h"
#include "otto/serialization/serializer.h"
#include "otto/window/icon/icon_loader.h"
#include "otto/core/package_loader.h"
#include "otto/core/platform/time.h"
#include "otto/scene/scene_loader.h"
#include "otto/core/scene_manager.h"
#include "otto/window/window.h"
#include "otto/debug/log/log.h"
#include "otto/scene/scene.h"
#include "otto/core/events.h"
#include "otto/util/file.h"

namespace otto
{
    struct LogLevelParsingError
    {
    } sLogLevelParsingError;

    static const uint32 UPDATE_CAP = 60;
    static const float64 sSecondsPerUpdate = 1.0 / UPDATE_CAP;

    static bool sInitialized = false;
    static bool sRunning = false;

    static FilePath sRootDirectory;
    static const FilePath CORE_ROOT_DIRECTORY = "C:/ProgramData/Otto/";

#ifdef OTTO_COUNT_FPS
    static const float64 FPS_UPDATES_PER_SECOND = 1.0;
    static const float64 SECONDS_PER_FPS_UPDATE = 1.0 / FPS_UPDATES_PER_SECOND;
#endif
        

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

        Log::_pushMessage(std::cout, Log::INFO, "Initializing Application...");

        Time::init();

        auto result = _loadSettings(settingsFilePath);
        if (result.hasError())
        {
            Log::_pushMessage(std::cout, Log::ERROR, "Failed to initialize Application: Could not load Settings from ", settingsFilePath, '.');
            return false;
        }

        auto settings = result.getResult();

        sRootDirectory = settings.rootDirectory;

        Log::init(getRootDirectory() + settings.logFilePath, std::cout, settings.logConsoleLevel, settings.logFileLevel);

#ifdef OTTO_DYNAMIC
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        // Had to copy everything from C:\Program Files (x86)\Windows Kits\10\bin\10.0.19041.0\x64 to 
        // CORE_ROOT_DIRECTORY + "MSVC/" + settings.msvcVersion + "/bin/Hostx86/x64/cl.exe"
        // for the linker to work
        // !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        DllReloader::Settings dllSettings;
        dllSettings.compilerExePath = CORE_ROOT_DIRECTORY + "MSVC/" + settings.msvcVersion + "/bin/Hostx86/x64/cl.exe";
        dllSettings.linkerExePath = CORE_ROOT_DIRECTORY + "MSVC/" + settings.msvcVersion + "/bin/Hostx86/x64/link.exe";
        dllSettings.tmpDir = sRootDirectory + ".tmp/";
        dllSettings.includeDirs = { 
            "C:/dev/Otto/Otto/src",
            sRootDirectory,
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

        if (!DllReloader::init(dllSettings))
        {
            Log::error("Failed to initialize DllReloader.");
            return false;
        }

        if (!SceneLoader::reloadDll(settings.applicationPackage))
        {
            Log::error("Failed to reload Client Dll.");
            return false;
        }

        if (!SceneManager::setScene(sRootDirectory + settings.startScene + ".otto"))
            return false;

#endif
        bool windowInitialized = Window::init(settings.rootDirectory + settings.windowSettingsPath, {
            Application::_onKeyPressed,
            Application::_onKeyReleased,
            Application::_onMouseButtonPressed,
            Application::_onMouseButtonReleased,
            Application::_onMouseMoved,
            Application::_onMouseDragged,
            Application::_onMouseScrolled,
            Application::_onWindowClosed,
            Application::_onWindowResized,
            Application::_onWindowGainedFocus,
            Application::_onWindowLostFocus,
        });

        if (!windowInitialized)
        {
            Log::error("Failed to initialize Window.");
            return false;
        }

        Log::trace("Initializing scene...");

        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneInitHandle, _getClientDllPath(), Scene, void, init);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnInitHandle, _getClientDllPath(), Scene, void, _onInit, const _InitEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneInitHandle);
        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnInitHandle, _InitEvent());

        Log::trace("Done initializing scene.");

        sInitialized = true;

        Log::info("Initialization complete.");

        return true;
    }

    void Application::run()
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnUpdateHandle, _getClientDllPath(), Scene, void, _onUpdate, const _UpdateEvent&);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnRebufferHandle, _getClientDllPath(), Scene, void, _onRebuffer, const _RebufferEvent&);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnRenderHandle, _getClientDllPath(), Scene, void, _onRender, const _RenderEvent&);

        sRunning = true;

        float64 totalDelta = 0.0;
        float64 startTime = Time::getTime64();

#ifdef OTTO_COUNT_FPS
        float64 fpsTimer = 0.0;
        uint32 frames = 0;
#endif

        while (sRunning)
        {
            if (totalDelta >= sSecondsPerUpdate)
            {
                OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnUpdateHandle, _UpdateEvent{ static_cast<float32>(totalDelta) });
                OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnRebufferHandle, _RebufferEvent());

                Window::pollEvents();

                totalDelta = 0.0;
            }

            Window::clear();
            OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnRenderHandle, _RenderEvent());
            Window::swapBuffers();

            float64 endTime = Time::getTime64();
            float64 delta = endTime - startTime;

#ifdef OTTO_COUNT_FPS
            frames++;
            fpsTimer += delta;

            if (fpsTimer >= SECONDS_PER_FPS_UPDATE)
            {
                fpsTimer -= SECONDS_PER_FPS_UPDATE;
                Log::debug("FPS: ", frames);
                frames = 0;
            }
#endif

            totalDelta += delta;
            startTime = endTime;
        }
    }

    const FilePath& Application::getRootDirectory()
    {
        return sRootDirectory;
    }

    const FilePath& Application::getCoreRootDirectory()
    {
        return CORE_ROOT_DIRECTORY;
    }

    void Application::stop()
    {
        sRunning = false;
    }

    void Application::destroy()
    {
        Log::info("Destroying Application...");

        IconLoader::destroy();

        Window::saveSettings();
        Window::destroy();

#ifdef OTTO_DYNAMIC
        DllReloader::destroy();
#endif

        Log::info("Destruction complete.");
    }

    Result<Application::Settings, Application::SettingsError> Application::_loadSettings(const FilePath& filePath)
    {
        Application::Settings settings;

        auto result = Serializer::load(filePath);
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

        if (!file.contains("windowSettingsPath"))
            return SettingsError::WINDOW_SETTINGS_PATH_NOT_FOUND;
        else
            settings.windowSettingsPath = file.get<String>("windowSettingsPath");

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

        if (!file.contains("StartScene"))
            return SettingsError::START_SCENE_NOT_FOUND;
        else
            settings.startScene = file.get<String>("StartScene");

        auto package = PackageLoader::loadPackage(file, CORE_ROOT_DIRECTORY);
        if (package.hasError())
            return SettingsError::PACKAGE_ERROR;
        else
            settings.applicationPackage = package.getResult();

        if (!settings.applicationPackage.events.contains("otto/events/InitEvent"))
            settings.applicationPackage.events.add("otto/events/InitEvent");
        if (!settings.applicationPackage.events.contains("otto/events/UpdateEvent"))
            settings.applicationPackage.events.add("otto/events/UpdateEvent");
        if (!settings.applicationPackage.events.contains("otto/events/RebufferEvent"))
            settings.applicationPackage.events.add("otto/events/RebufferEvent");
        if (!settings.applicationPackage.events.contains("otto/events/RenderEvent"))
            settings.applicationPackage.events.add("otto/events/RenderEvent");

        return settings;
    }

    void Application::_onKeyPressed(const _KeyPressedEvent& e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnKeyPressedHandle, _getClientDllPath(), Scene, void, _onKeyPressed, const _KeyPressedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnKeyPressedHandle, e);
    }

    void Application::_onKeyReleased(const _KeyReleasedEvent& e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnKeyReleasedHandle, _getClientDllPath(), Scene, void, _onKeyReleased, const _KeyReleasedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnKeyReleasedHandle, e);
    }

    void Application::_onMouseButtonPressed(const _MouseButtonPressedEvent& e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnMouseButtonPressedHandle, _getClientDllPath(), Scene, void, _onMouseButtonPressed, const _MouseButtonPressedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnMouseButtonPressedHandle, e);
    }

    void Application::_onMouseButtonReleased(const _MouseButtonReleasedEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnMouseButtonReleasedHandle, _getClientDllPath(), Scene, void, _onMouseButtonReleased, const _MouseButtonReleasedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnMouseButtonReleasedHandle, e);
    }

    void Application::_onMouseMoved(const _MouseMovedEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnMouseMovedHandle, _getClientDllPath(), Scene, void, _onMouseMoved, const _MouseMovedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnMouseMovedHandle, e);
    }

    void Application::_onMouseDragged(const _MouseDraggedEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnMouseDraggedHandle, _getClientDllPath(), Scene, void, _onMouseDragged, const _MouseDraggedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnMouseDraggedHandle, e);
    }

    void Application::_onMouseScrolled(const _MouseScrolledEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnMouseScrolledHandle, _getClientDllPath(), Scene, void, _onMouseScrolled, const _MouseScrolledEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnMouseScrolledHandle, e);
    }

    void Application::_onWindowClosed(const _WindowClosedEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnWindowClosedHandle, _getClientDllPath(), Scene, void, _onWindowClosed, const _WindowClosedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnWindowClosedHandle, e);

        Application::stop();
    }

    void Application::_onWindowResized(const _WindowResizedEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnWindowResizedHandle, _getClientDllPath(), Scene, void, _onWindowResized, const _WindowResizedEvent&);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnRebufferHandle, _getClientDllPath(), Scene, void, _onRebuffer, const _RebufferEvent&);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnRenderHandle, _getClientDllPath(), Scene, void, _onRender, const _RenderEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnWindowResizedHandle, e);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnRebufferHandle, _RebufferEvent());

        Window::clear();

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnRenderHandle, _RenderEvent());

        Window::swapBuffers();
    }

    void Application::_onWindowGainedFocus(const _WindowGainedFocusEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnWindowGainedFocusHandle, _getClientDllPath(), Scene, void, _onWindowGainedFocus, const _WindowGainedFocusEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnWindowGainedFocusHandle, e);
    }

    void Application::_onWindowLostFocus(const _WindowLostFocusEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnWindowLostFocusHandle, _getClientDllPath(), Scene, void, _onWindowLostFocus, const _WindowLostFocusEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnWindowLostFocusHandle, e);
    }

#ifdef OTTO_DYNAMIC
    FilePath Application::_getClientDllPath()
    {
        return sRootDirectory + ".tmp/client.dll";
    }
#endif

} // namespace otto
