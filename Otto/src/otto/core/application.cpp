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
#include "otto/util/optional.h"
#include "otto/window/window.h"
#include "otto/debug/log/log.h"
#include "otto/scene/scene.h"
#include "otto/core/events.h"
#include "otto/util/file.h"

namespace otto
{
    static Application* sInstance = nullptr;

    static const FilePath CORE_ROOT_DIRECTORY = "C:/ProgramData/Otto/";

#ifdef OTTO_COUNT_FPS
    static const float64 FPS_UPDATES_PER_SECOND = 1.0;
    static const float64 SECONDS_PER_FPS_UPDATE = 1.0 / FPS_UPDATES_PER_SECOND;
#endif
        
    namespace
    {
        Optional<uint8> _parseLogLevel(const String& logLevel)
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

            return NULLOPT;
        }

    } // namespace

    Application* Application::getInstance()
    {   
        return sInstance;
    }

    bool8 Application::init(Application* mainApplication, Window* mainWindow, Log* mainLog)
    {
        if (sInstance != nullptr)
            return false;

        Log::init(mainLog);
        Window::init(mainWindow);

        sInstance = mainApplication;
        return true;
    }

    bool8 Application::init(const FilePath& settingsFilePath)
    {
        static Application application;

        if (sInstance != nullptr)
            return false;

        sInstance = &application;

        Log::_pushMessage(std::cout, Log::INFO, "Initializing Application...");

        Time::init();

        auto result = _loadSettings(settingsFilePath);
        if (result.hasError())
        {
            Log::_pushMessage(std::cout, Log::ERROR, "Failed to initialize Application: Could not load Settings from ", settingsFilePath, '.');
            return false;
        }

        auto settings = result.getResult();

        sInstance->mRootDirectory = settings.rootDirectory;
        sInstance->mSecondsPerUpdate = 1.0 / settings.updateCap;
        sInstance->mAdditionalSettings = settings.additionalSettings;

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
        dllSettings.tmpDir = sInstance->mRootDirectory + ".tmp/";
        dllSettings.includeDirs = { 
            "C:/dev/Otto/Otto/src",
            "C:/dev/Otto/Otto/dependencies/glew/include",
            sInstance->mRootDirectory,
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
#endif

        if (!Window::init(settings.rootDirectory + settings.windowSettingsPath))
        {
            Log::error("Failed to initialize Window.");
            return false;
        }

        SceneLoader::initClient(Application::getInstance(), Window::getInstance(), Log::getInstance(), Window::getInstance()->mSettings.clearColor);

        if (!SceneManager::setScene(sInstance->mRootDirectory + settings.startScene + ".otto"))
            return false;

        Window::setListeners({
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

        Log::trace("Initializing scene...");

        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneInitHandle, _getClientDllPath(), Scene, void, init);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneInitHandle);

        Log::trace("Done initializing scene.");
        Log::info("Initialization complete.");

        return true;
    }

    void Application::run()
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneUpdateHandle, _getClientDllPath(), Scene, void, update, float32);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneRebufferHandle, _getClientDllPath(), Scene, void, rebuffer);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneRenderHandle, _getClientDllPath(), Scene, void, render);

        sInstance->mRunning = true;

        float64 totalDelta = 0.0;
        float64 startTime = Time::getTime64();

#ifdef OTTO_COUNT_FPS
        float64 fpsTimer = 0.0;
        uint32 frames = 0;
#endif

        while (sInstance->mRunning)
        {
            if (totalDelta >= sInstance->mSecondsPerUpdate)
            {
                OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneUpdateHandle, static_cast<float32>(totalDelta));
                OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneRebufferHandle);

                Window::pollEvents();

                totalDelta = 0.0;
            }

            //Window::clear();
            OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneRenderHandle);
            //Window::swapBuffers();

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
        return sInstance->mRootDirectory;
    }

    const FilePath& Application::getCoreRootDirectory()
    {
        return CORE_ROOT_DIRECTORY;
    }

    const Map<String, Serialized>& Application::getSettings()
    {
        return sInstance->mAdditionalSettings;
    }

    void Application::stop()
    {
        sInstance->mRunning = false;
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

        if (!file.contains("rootDirectory"))
            return SettingsError::ROOT_DIRECTORY_NOT_FOUND;

        if (!file.contains("logFilePath"))
            return SettingsError::LOG_FILE_PATH_NOT_FOUND;

        if (!file.contains("msvcVersion"))
            return SettingsError::MSVC_VERSION_NOT_FOUND;
        
        if (!file.contains("windowSettingsPath"))
            return SettingsError::WINDOW_SETTINGS_PATH_NOT_FOUND;

        if (!file.contains("startScene"))
            return SettingsError::START_SCENE_NOT_FOUND;

        for (auto& [name, serialized] : file.getDictionary())
        {
            if (name.equalsIgnoreCase("updateCap"))
                settings.updateCap = serialized.get<float64>();
            if (name.equalsIgnoreCase("rootDirectory"))
                settings.rootDirectory = serialized.get<String>();
            else if (name.equalsIgnoreCase("logFilePath"))
                settings.logFilePath = serialized.get<String>();
            else if (name.equalsIgnoreCase("msvcVersion"))
                settings.msvcVersion = serialized.get<String>();
            else if (name.equalsIgnoreCase("windowSettingsPath"))
                settings.windowSettingsPath = serialized.get<String>();
            else if (name.equalsIgnoreCase("startScene"))
                settings.startScene = serialized.get<String>();
            else if (name.equalsIgnoreCase("logConsoleLevel"))
            {
                auto logConsoleLevel = _parseLogLevel(file.get<String>("LogConsoleLevel"));
                if (logConsoleLevel.hasValue())
                    settings.logConsoleLevel = logConsoleLevel.getValue();
            }
            else if (name.equalsIgnoreCase("logFileLevel"))
            {
                auto logFileLevel = _parseLogLevel(file.get<String>("logFileLevel"));
                if (logFileLevel.hasValue())
                    settings.logFileLevel = logFileLevel.getValue();
            }
            else if (!name.equalsIgnoreCase("dependencies") && !name.equalsIgnoreCase("components") &&
                !name.equalsIgnoreCase("events") && !name.equalsIgnoreCase("behaviours") && !name.equalsIgnoreCase("systems"))
            {
                settings.additionalSettings.insert(name, serialized);
            }
        }

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
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnKeyPressedHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_KeyPressedEvent>, const _KeyPressedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnKeyPressedHandle, e);
    }

    void Application::_onKeyReleased(const _KeyReleasedEvent& e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnKeyReleasedHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_KeyReleasedEvent>, const _KeyReleasedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnKeyReleasedHandle, e);
    }

    void Application::_onMouseButtonPressed(const _MouseButtonPressedEvent& e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnMouseButtonPressedHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_MouseButtonPressedEvent>, const _MouseButtonPressedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnMouseButtonPressedHandle, e);
    }

    void Application::_onMouseButtonReleased(const _MouseButtonReleasedEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnMouseButtonReleasedHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_MouseButtonReleasedEvent>, const _MouseButtonReleasedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnMouseButtonReleasedHandle, e);
    }

    void Application::_onMouseMoved(const _MouseMovedEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnMouseMovedHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_MouseMovedEvent>, const _MouseMovedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnMouseMovedHandle, e);
    }

    void Application::_onMouseDragged(const _MouseDraggedEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnMouseDraggedHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_MouseDraggedEvent>, const _MouseDraggedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnMouseDraggedHandle, e);
    }

    void Application::_onMouseScrolled(const _MouseScrolledEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnMouseScrolledHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_MouseScrolledEvent>, const _MouseScrolledEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnMouseScrolledHandle, e);
    }

    void Application::_onWindowClosed(const _WindowClosedEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnWindowClosedHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_WindowClosedEvent>, const _WindowClosedEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnWindowClosedHandle, e);

        Application::stop();
    }

    void Application::_onWindowResized(const _WindowResizedEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnWindowResizedHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_WindowResizedEvent>, const _WindowResizedEvent&);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnRebufferHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_RebufferEvent>, const _RebufferEvent&);
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnRenderHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_RenderEvent>, const _RenderEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnWindowResizedHandle, e);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnRebufferHandle, _RebufferEvent());

        //Window::clear();

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnRenderHandle, _RenderEvent());

        //Window::swapBuffers();
    }

    void Application::_onWindowGainedFocus(const _WindowGainedFocusEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnWindowGainedFocusHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_WindowGainedFocusEvent>, const _WindowGainedFocusEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnWindowGainedFocusHandle, e);
    }

    void Application::_onWindowLostFocus(const _WindowLostFocusEvent & e)
    {
        OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(sceneOnWindowLostFocusHandle, _getClientDllPath(), Scene, void, 
            dispatchEvent<struct otto::_WindowLostFocusEvent>, const _WindowLostFocusEvent&);

        OTTO_CALL_DLL_MEMBER_FUNCTION(*SceneManager::sCurrentScene.get(), sceneOnWindowLostFocusHandle, e);
    }

#ifdef OTTO_DYNAMIC
    FilePath Application::_getClientDllPath()
    {
        return sInstance->mRootDirectory + ".tmp/client.dll";
    }
#endif

} // namespace otto
