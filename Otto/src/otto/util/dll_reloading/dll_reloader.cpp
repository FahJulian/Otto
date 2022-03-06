#include "dll_reloader.h"

#include <Windows.h>
#include <ImageHlp.h>

#include "otto/debug/log/log.h"

#include "otto/util/map.h"
#include "otto/util/dynamic_array.h"
#include "otto/util/platform/file_utils.h"

namespace otto
{
    namespace
    {
        struct Dll
        {
            struct Symbol
            {
                String nativeName;
                ptr64* pointer;
            };

            String name;
            HINSTANCE win32Handle;
            Map<String, Symbol> functions;
        };

        constexpr const char* COMPILER_COMMAND = "%COMPILER_PATH "
            "/c /ZI /JMC /nologo /W3 /WX- /diagnostics:column /sdl /Gm- /EHsc /MDd /GS /fp:precise "
            "/permissive- /Zc:wchar_t /Zc:forScope /Zc:inline /std:c++20 /Gd /TP /FC /errorReport:queue "
            "/D OTTO_DLL_EXPORT /D OTTO_DLL_EXPORT_DEBUG /D _CONSOLE /D _WINDLL /D _UNICODE /D UNICODE ";

        constexpr const char* LINKER_COMMAND = "%LINKER_PATH "
            "/ERRORREPORT:QUEUE /INCREMENTAL /NOLOGO /MANIFEST /MANIFESTUAC:\"level='asInvoker' uiAccess='false'\" "
            "/manifest:embed /DEBUG:NONE /SUBSYSTEM:CONSOLE /TLBID:1 /DYNAMICBASE /NXCOMPAT /MACHINE:X64 /DLL "
            "kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib ";

        Map<String, Dll> sDlls;
        DllReloader::Settings sSettings;
        bool8 sInitialized = false;

        String _generateCompilerCommand(const String& dllName, const DynamicArray<FilePath>& sourceFiles, const DynamicArray<String>& prepocessorDefinitions,
            const DynamicArray<String>& compilerOptions)
        {
            String command = COMPILER_COMMAND;
            command.replaceFirst("%COMPILER_PATH", sSettings.compilerExePath.toString());
            command += sSettings.configuration == DllReloader::Configuration::DEBUG ? "/Od " : "/02 ";

            for (auto& option : compilerOptions)
                command += option + " ";

            for (auto& preprocessorDefinition : prepocessorDefinitions)
                command += "/D " + preprocessorDefinition + " ";

            command += "/Fo\"" + sSettings.tmpDir.toString() + dllName + "\\\\\" ";

            for (auto& includeDir : sSettings.includeDirs)
            {
                String dir = String::removeAll(includeDir.toString(), '\"');
                if (dir.endsWith('\\'))
                    dir.removeLast('\\');

                command += "/I \"" + dir + "\" ";
            }

            for (auto& sourceFile : sourceFiles)
                command += sourceFile.toString() + " "; 

            return command;
        }

        String _generateLinkerCommand(const String& dllName, const DynamicArray<FilePath> objFiles, const DynamicArray<String>& linkerOptions)
        {
            String command = LINKER_COMMAND;
            command.replaceFirst("%LINKER_PATH", sSettings.linkerExePath.toString());
            command += "/IMPLIB:\"" + sSettings.tmpDir.toString() + dllName + "\\" + dllName + ".lib\" ";
            command += "/OUT:\"" + sSettings.tmpDir.toString() + dllName + "\\" + dllName + ".dll\" ";

            for (auto& lib : sSettings.libs)
                command += lib + " ";

            for (auto& option : linkerOptions)
                command += option + " ";

            for (auto& libDir : sSettings.libDirs)
                command += "/LIBPATH:\"" + libDir.toString() + "\" ";

            for (auto& objFile : objFiles)
                command += sSettings.tmpDir.toString() + dllName + "\\" + objFile.toString() + " ";

            return command;
        }

        bool8 _compilationSuccessfull(const Dll& dll)
        {
            return (FileUtils::fileExists(sSettings.tmpDir + dll.name + "\\" + dll.name + ".dll") &&
                FileUtils::fileExists(sSettings.tmpDir + dll.name + "\\" + dll.name + ".lib"));
        }

        Map<String, String> _extractSymbols(const String& dllPath)
        {
            Map<String, String> symbols;
            LOADED_IMAGE loadedImage;
            DWORD directorySize;

            if (MapAndLoad(dllPath.getData(), NULL, &loadedImage, NULL, TRUE))
            {
                IMAGE_EXPORT_DIRECTORY* imageExportDirectory = reinterpret_cast<IMAGE_EXPORT_DIRECTORY*>(
                    ImageDirectoryEntryToData(loadedImage.MappedAddress, FALSE, IMAGE_DIRECTORY_ENTRY_EXPORT, &directorySize));

                if (imageExportDirectory)
                {
                    DWORD* symbolsVirtualAddresses = reinterpret_cast<DWORD*>(ImageRvaToVa(loadedImage.FileHeader,
                        loadedImage.MappedAddress, imageExportDirectory->AddressOfNames, NULL));

                    constexpr uint64 symbolNameBufferSize = 0xfff;
                    char8 symbolNameBuffer[symbolNameBufferSize];

                    for (uint64 i = 0; i < imageExportDirectory->NumberOfNames; i++)
                    {
                        char* symbolAdress = reinterpret_cast<char*>(ImageRvaToVa(loadedImage.FileHeader, loadedImage.MappedAddress, symbolsVirtualAddresses[i], NULL));
                        constexpr DWORD flags = UNDNAME_NO_ACCESS_SPECIFIERS | UNDNAME_NO_ALLOCATION_LANGUAGE | UNDNAME_NO_ALLOCATION_MODEL | UNDNAME_NO_SPECIAL_SYMS |
                            UNDNAME_NO_RETURN_UDT_MODEL | UNDNAME_NO_MS_THISTYPE | UNDNAME_NO_MS_KEYWORDS;

                        std::memset(symbolNameBuffer, 0, symbolNameBufferSize);
                        UnDecorateSymbolName(symbolAdress, symbolNameBuffer, symbolNameBufferSize, flags);

                        symbols.insert(String(symbolNameBuffer).replaceAll("(void)", "()").replaceAll(" >", ">").replaceAll("static ", ""), symbolAdress);
                    }
                }

                UnMapAndLoad(&loadedImage);
            }

            return symbols;
        }

        bool8 _reloadDll(Dll& dll, const String& path)
        {
            FreeLibrary(dll.win32Handle);
            dll.win32Handle = LoadLibraryA((path + dll.name + ".dll").getData());

            if (!dll.win32Handle)
                return false;

            Map<String, String> symbols = _extractSymbols(path + dll.name + ".dll");

            for (auto& [name, function] : dll.functions)
            {
                if (!symbols.containsKey(name))
                {
                    Log::error("Error reloading dll: Function ", name, " not found");
                    return false;
                }

                if (function.pointer)
                    *function.pointer = reinterpret_cast<ptr64>(GetProcAddress(dll.win32Handle, symbols[name].getData()));
                symbols.remove(name);
            }

            // Not yet registered functions
            for (auto& [name, symbol] : symbols)
                dll.functions.insert(name, { symbol, nullptr });

            return true;
        }

        bool8 _swapFiles(const Dll& dll, const String& path)
        {
            if (String oldDllFileName = path + dll.name + ".dll";
                FileUtils::fileExists(oldDllFileName))
            {
                FileUtils::renameFile(oldDllFileName, path + dll.name + "-old.dll");
            }

            if (String oldLibFileName = path + dll.name + ".lib";
                FileUtils::fileExists(oldLibFileName))
            {
                FileUtils::renameFile(oldLibFileName, path + dll.name + "-old.lib");
            }

            return (FileUtils::moveFile(sSettings.tmpDir.toString() + dll.name + '\\' + dll.name + ".dll", path + dll.name + ".dll") &&
                FileUtils::moveFile(sSettings.tmpDir.toString() + dll.name + '\\' + dll.name + ".lib", path + dll.name + ".lib"));
        }

        bool8 _reloadOldDll(Dll& dll, const String& path)
        {
            return (FileUtils::renameFile(path + dll.name + "-old.dll", path + dll.name + ".dll") &&
                FileUtils::renameFile(path + dll.name + "-old.lib", path + dll.name + ".lib") &&
                _reloadDll(dll, path));
        }

        void _compileDll(const String& dllName, const DynamicArray<FilePath>& cppFiles, const DynamicArray<String>& preprocessorDefinitions, const DynamicArray<String>& compilerOptions)
        {
            String compilerCommand = _generateCompilerCommand(dllName, cppFiles, preprocessorDefinitions, compilerOptions);

            Log::trace("Recompiling dll ", dllName, "...");

            std::system(compilerCommand.getData());
        }

        void _linkDll(const String& dllName, const DynamicArray<FilePath>& objFiles, const DynamicArray<String>& linkerOptions)
        {
            String linkerCommand = _generateLinkerCommand(dllName, objFiles, linkerOptions);

            Log::trace("Linking dll ", dllName, "...");

            std::system(linkerCommand.getData());
        }

        void _deleteTemporaryFiles(const Dll& dll, const String& path, const DynamicArray<FilePath>& objFiles)
        {
            String tmpDir = sSettings.tmpDir.toString() + dll.name + '\\';

            FileUtils::deleteFile(path + dll.name + "-old.dll");
            FileUtils::deleteFile(path + dll.name + "-old.lib");

            FileUtils::deleteFile(tmpDir + dll.name + ".dll");
            FileUtils::deleteFile(tmpDir + dll.name + ".lib");
            FileUtils::deleteFile(tmpDir + dll.name + ".exp");
            FileUtils::deleteFile(tmpDir + dll.name + ".ilk");

            for (auto& objFile : objFiles)
                FileUtils::deleteFile(tmpDir + objFile);

            FileUtils::deleteEmptyDirectoryRecursively(tmpDir);
        }

        bool8 _recompileAndReloadDll(Dll& dll, const String& path, DynamicArray<FilePath> files, DynamicArray<String> preprocessorDefinitions,
            const DynamicArray<String>& compilerOptions, const DynamicArray<String>& linkerOptions)
        {
            String tmpDir = sSettings.tmpDir.toString() + dll.name + '\\';
            if (!FileUtils::directoryExists(tmpDir))
                FileUtils::createDirectoryRecursively(tmpDir);

            for (auto& file : files)
            {
                if (!file.toString().endsWith(".cpp"))
                {
                    Log::error("Can't recompile dll ", dll.name, ": invalid source file ", file, ". Only .cpp files are allowed.");
                    return false;
                }
            }

            _compileDll(dll.name, files, preprocessorDefinitions, compilerOptions);

            // Turn .cpp files into .obj files
            for (auto& file : files)
                file = String::subString(file.toString(), file.toString().findLastOf('\\') + 1, file.toString().findFirstOf(".cpp")) + ".obj";

            _linkDll(dll.name, files, linkerOptions);

            if (!_compilationSuccessfull(dll))
            {
                Log::error("Could not recompile dll ", dll.name);
                return false;
            }

            Log::trace("Done. Extracting symbols from dll ", dll.name, "...");

            if (!_swapFiles(dll, path) || !_reloadDll(dll, path))
            {
                Log::error("Could not extract symbols from dll ", dll.name);
                Log::error("trying to reload old dll...");

                if (!_reloadOldDll(dll, path))
                {
                    Log::error("Failed to extract symbols.");

                    _deleteTemporaryFiles(dll, path, files);
                    return false;
                }

                Log::trace("Done extracting symbols.");
            }

            Log::info("Done reloading Dll.");

            _deleteTemporaryFiles(dll, path, files);
            return true;
        }

    } // namespace

    bool8 DllReloader::init(const Settings& settings)
    {
        Log::trace("Initializing DllReloader...");

        sSettings = settings;

        if (!sSettings.compilerExePath.toString().endsWithIgnoreCase("\\cl.exe"))
        {
            Log::error("[DllReloader] Invalid cl.exe path: ", sSettings.compilerExePath.toString());
            return false;
        }

        if (!sSettings.linkerExePath.toString().endsWithIgnoreCase("\\link.exe"))
        {
            Log::error("[DllReloader] Invalid link.exe path: ", sSettings.linkerExePath.toString());
            return false;
        }

        if (!FileUtils::fileExists(sSettings.tmpDir))
            FileUtils::createDirectoryRecursively(sSettings.tmpDir);

        sInitialized = true;

        Log::trace("Done initializing DllReloader");
        return true;
    }

    bool8 DllReloader::reloadDll(const FilePath& dllPath, bool8 forceRecompile, const DynamicArray<FilePath>& sourceFiles,
        const DynamicArray<String>& additionalPreprocessorDefinitions, const DynamicArray<String>& compilerOptions, const DynamicArray<String>& linkerOptions)
    {
        Log::info("Reloading Dll ", dllPath, "...");

        if (!sInitialized)
        {
            Log::warn("Can't recompile dll ", dllPath, ": DllReloader has not been initialized");
            return false;
        }

        FilePath dllDir = dllPath.getParentDirectory();

        if (!sDlls.containsKey(dllPath))
            sDlls.insert(dllPath.toString(), { String::subString(dllPath.toString(), dllPath.toString().findLastOf('\\') + 1, dllPath.toString().findLastOf('.')) });

        Dll& dll = sDlls[dllPath.toString()];

        if (forceRecompile)
        {
            if (!_recompileAndReloadDll(dll, dllDir.toString(), sourceFiles, additionalPreprocessorDefinitions, compilerOptions, linkerOptions))
                return false;
        }
        else
        {
            if (!_reloadDll(dll, dllDir.toString()))
            {
                Log::info("Reloading dll ", dllPath.toString(), " failed, trying to recompile...");

                if (!_recompileAndReloadDll(dll, dllDir.toString(), sourceFiles, additionalPreprocessorDefinitions, compilerOptions, linkerOptions))
                    return false;
            }
        }

        return true;
    }

    void DllReloader::destroy()
    {
        Log::trace("Destroying Dll Reloader...");

        for (auto& [name, dll] : sDlls)
        {
            Log::info("Freeing dll ", name, ".dll");

            FreeLibrary(dll.win32Handle);
        }
    }

    ptr64 DllReloader::_registerFunction(const FilePath& dllPath, ptr64* functionPointer, const String& functionName)
    {
        if (!sDlls.containsKey(dllPath.toString()))
        {
            Log::warn("Can't register function ", functionName, " for Dll ", dllPath, ": Dll is not registered for RCR");
            return 0;
        }

        Dll& dll = sDlls[dllPath.toString()];
        if (!dll.win32Handle)
        {
            Log::warn("Can't register function ", functionName, " for Dll ", dllPath, ": Dll has not been loaded.");
            return 0;
        }

        if (!dll.functions.containsKey(functionName))
        {
            Log::warn("Can't register function ", functionName, " for Dll ", dllPath, ": Dll does not cotain function");
            return 0;
        }

        dll.functions[functionName].pointer = functionPointer;
        return reinterpret_cast<ptr64>(GetProcAddress(dll.win32Handle, dll.functions[functionName].nativeName.getData()));
    }

} // namespace otto
