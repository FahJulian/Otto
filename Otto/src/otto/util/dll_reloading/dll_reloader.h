#pragma once

#include "otto/base.h"

#include "otto/util/string.h"
#include "otto/util/file_path.h"
#include "otto/util/dynamic_array.h"

#define OTTO_DECLARE_DLL_MEMBER_FUNCTION_HANDLE(handleName, dllPath, classType, returnType, name, ...) \
        static returnType(classType::* handleName)(__VA_ARGS__) = \
            otto::DllReloader::registerMemberFunctionPointer(dllPath, &handleName, otto::DllReloader::getTypeNames<returnType>(), \
            #name, otto::DllReloader::getTypeNames<>(), otto::DllReloader::getTypeNames<__VA_ARGS__>(true)) \

#define OTTO_DECLARE_DLL_FUNCTION_HANDLE(handleName, dllPath, returnType, name, ...) \
            static returnType(*handleName)(__VA_ARGS__) = \
                otto::DllReloader::registerFunctionPointer(dllPath, &handleName, otto::DllReloader::getTypeNames<returnType>(), \
                #name, otto::DllReloader::getTypeNames<>(), otto::DllReloader::getTypeNames<__VA_ARGS__>(true)) \

#define OTTO_CALL_DLL_MEMBER_FUNCTION(object, memberFunctionHandle, ...) \
    ((object).*memberFunctionHandle)(__VA_ARGS__)

#define OTTO_CALL_DLL_FUNCTION(functionHandle, ...) \
    (*functionHandle)(__VA_ARGS__)

#ifdef OTTO_DYNAMIC
#ifdef OTTO_EXPORT
#define OTTO_DLL_FUNC __declspec(dllexport)
#else
#define OTTO_DLL_FUNC __declspec(dllimport)
#endif
#else
#define OTTO_DLL_FUNC extern
#endif

namespace otto
{
    class DllReloader
    {
    public:
        template<typename R, typename... Args>
        using Functionptr64 = R(*)(Args...);

        template<typename R, typename T, typename... Args>
        using MemberFunctionptr64 = R(T::*)(Args...);

        enum class Configuration : uint8
        {
            DEBUG,
            RELEASE
        };

        struct Settings
        {
            FilePath compilerExePath;
            FilePath linkerExePath;
            FilePath tmpDir;
            DynamicArray<FilePath> includeDirs;
            DynamicArray<FilePath> libDirs;
            DynamicArray<String> libs;
            Configuration configuration = Configuration::DEBUG;
        };

        template<typename... Args>
        static String getTypeNames()
        {
            String string;
            _appendTypeNames<Args...>(string, false);
            return string;
        }

        template<>
        static String getTypeNames()
        {
            return { };
        }

        template<typename... Args>
        static String getTypeNames(bool8 isArgumentTypes)
        {
            String string;
            _appendTypeNames<Args...>(string, isArgumentTypes);
            return string;
        }

        template<>
        static otto::String getTypeNames(bool8 isArgumentTypes)
        {
            return { };
        }

        template<typename R, typename... Args>
        static Functionptr64<R, Args...> registerFunctionPointer(const FilePath& dllPath,
            Functionptr64<R, Args...>* functionptr64,
            const String& returnType,
            const String& functionName,
            const String& templateTypes,
            const String& argumentTypes)
        {
            String fullFunctionName = returnType + ' ' + functionName;

            if (!templateTypes.isEmpty())
                fullFunctionName += '<' + templateTypes + '>';

            fullFunctionName += '(' + argumentTypes + ')';

            ptr64 function = _registerFunction(dllPath, reinterpret_cast<ptr64*>(functionptr64), fullFunctionName);
            return *reinterpret_cast<Functionptr64<R, Args...>*>(&function);
        }

        template<typename R, typename T, typename... Args>
        static MemberFunctionptr64<R, T, Args...> registerMemberFunctionPointer(const FilePath& dllPath,
            MemberFunctionptr64<R, T, Args...>* memberFunctionptr64,
            const String& returnType,
            const String& memberFunctionName,
            const String& templateTypes,
            const String& argumentTypes)
        {
            String className = getTypeNames<T>(false);
            className.toSubString(className.findFirstOf(' ') + 1);    // remove the "class " or "struct " prefix

            String fullMemberFunctionName = returnType + ' ' + className + "::" + memberFunctionName;

            if (!templateTypes.isEmpty())
                fullMemberFunctionName += '<' + templateTypes + '>';

            fullMemberFunctionName += '(' + argumentTypes + ')';

            ptr64 function = _registerFunction(dllPath, reinterpret_cast<ptr64*>(memberFunctionptr64), fullMemberFunctionName);
            return *reinterpret_cast<MemberFunctionptr64<R, T, Args...>*>(&function);
        }

        static bool8 init(const Settings& settings);

        static bool8 reloadDll(const FilePath& dllPath, bool8 forceRecompile, const DynamicArray<FilePath>& sourceFiles,
            const DynamicArray<String>& additionalPreprocessorDefinitions = { }, const DynamicArray<String>& compilerOptions = { }, const DynamicArray<String>& linkerOptions = { });

        static void destroy();

    private:
        static ptr64 _registerFunction(const FilePath& dllPath, ptr64* functionptr64, const String& functionName);

        template<typename T>
        static void _appendTypeNames(String& string, bool8 isArgumentTypes, bool8 start = true)
        {
            if (!start)
                string.append(',');

            string.append(_getTypeName<T>(isArgumentTypes));
        }

        template<typename T, typename F, typename... Args>
        static void _appendTypeNames(String& string, bool8 isArugmentTypes, bool8 start = true)
        {
            _appendTypeNames<T>(string, isArugmentTypes, start);
            _appendTypeNames<F, Args...>(string, isArugmentTypes, false);
        }

        template<typename T>
        static constexpr String _getTypeName(bool8 ignoreModifiers)
        {
            if (std::is_pointer<T>::value)
            {
                String valueTypeName = _getTypeName<std::remove_pointer<T>::type>(false);
                if (std::is_const<T>::value)
                {
                    if (std::is_volatile<T>::value)
                        return valueTypeName + " * const volatile";
                    else
                        return valueTypeName + " * const";
                }
                else
                {
                    if (std::is_volatile<T>::value)
                        return valueTypeName + " * volatile";
                    else
                        return valueTypeName + " *";
                }
            }
            else if (std::is_lvalue_reference<T>::value)
            {
                String valueTypeName = _getTypeName<std::remove_reference<T>::type>(false);
                return valueTypeName + " &";
            }
            else if (std::is_rvalue_reference<T>::value)
            {
                String valueTypeName = _getTypeName<std::remove_reference<T>::type>(false);
                return valueTypeName + " &&";
            }
            else
            {
                static String valueTypeName = String(typeid(T).name())
                    .replaceAll(" const ", " ")
                    .replaceAll(" volatile ", " ")
                    .replaceAll(" * __ptr64", "");

                if (std::is_const<T>::value)
                {
                    if (std::is_volatile<T>::value)
                        return ignoreModifiers ? valueTypeName : (valueTypeName + " const volatile");
                    else
                        return ignoreModifiers ? valueTypeName : (valueTypeName + " const");
                }
                else
                {
                    if (std::is_volatile<T>::value)
                        return ignoreModifiers ? valueTypeName : (valueTypeName + " volatile");
                    else
                        return valueTypeName;
                }
            }
        }
    };

} // namespace otto
