#pragma once

#if defined(OTTO_DLL_EXPORT)
#error Exporting dlls should not use RCR to load another dll
#endif

#include "otto/util/string.h"
#include "otto/util/dynamic_array.h"

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
            String compilerExePath;
            String linkerExePath;
            String tmpDir;
            DynamicArray<String> includeDirs;
            DynamicArray<String> libDirs;
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
        static String getTypeNames(bool isArgumentTypes)
        {
            String string;
            _appendTypeNames<Args...>(string, isArgumentTypes);
            return string;
        }

        template<>
        static otto::String getTypeNames(bool isArgumentTypes)
        {
            return { };
        }

        template<typename R, typename... Args>
        static Functionptr64<R, Args...> registerFunctionPointer(const String& dllPath,
            Functionptr64<R, Args...>* functionptr64,
            const String& returnType,
            const String& namespaceName,
            const String& functionName,
            const String& templateTypes,
            const String& argumentTypes)
        {
            String fullFunctionName = returnType + ' ' + ((namespaceName != "::") ? namespaceName + functionName : functionName);

            if (!templateTypes.isEmpty())
                fullFunctionName += '<' + templateTypes + '>';

            fullFunctionName += '(' + argumentTypes + ')';

            ptr64 function = _registerFunction(dllPath, reinterpret_cast<ptr64*>(functionptr64), fullFunctionName);
            return *reinterpret_cast<Functionptr64<R, Args...>*>(&function);
        }

        template<typename R, typename T, typename... Args>
        static MemberFunctionptr64<R, T, Args...> registerMemberFunctionPointer(const String& dllPath,
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

        static bool init(const Settings& settings);

        static bool reloadDll(String dllPath, bool forceRecompile, std::initializer_list<String> sourceFiles,
            std::initializer_list<String> additionalPreprocessorDefinitions = {});

        static void destroy();

    private:
        static ptr64 _registerFunction(const String& dllPath, ptr64* functionptr64, const String& functionName);

        template<typename T>
        static void _appendTypeNames(String& string, bool isArgumentTypes, bool start = true)
        {
            if (!start)
                string.append(',');

            string.append(_getTypeName<T>(isArgumentTypes));
        }

        template<typename T, typename F, typename... Args>
        static void _appendTypeNames(String& string, bool isArugmentTypes, bool start = true)
        {
            _appendTypeNames<T>(string, isArugmentTypes, start);
            _appendTypeNames<F, Args...>(string, isArugmentTypes, false);
        }

        template<typename T>
        static constexpr String _getTypeName(bool ignoreModifiers)
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
