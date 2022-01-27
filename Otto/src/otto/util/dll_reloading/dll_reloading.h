#pragma once

#include <utility>
#include <type_traits>

#if !defined(OTTO_DLL_EXPORT)
#include "dll_reloader.h"
#endif

#include <iostream>

#if defined(OTTO_DLL_EXPORT)

#define OTTO_RCR_API __declspec(dllexport)
#define OTTO_RCR_FUNCTION(returnType, name, arguments, argumentTypes, argumensForwarded, ...) __declspec(dllexport) returnType name##arguments
#define OTTO_RCR_MEMBER_FUNCTION(returnType, name, arguments, argumentTypes, argumentsForwarded, ...) __declspec(dllexport) returnType name##arguments

#else

#define OTTO_RCR_API

#if !defined(OTTO_RCR_NAMESPACE)
#define OTTO_RCR_NAMESPACE 
#endif

#if !defined(OTTO_RCR_CLASS)
#define OTTO_RCR_CLASS 
#endif

#if !defined(OTTO_RCR_DLL_PATH)
#define OTTO_RCR_DLL_PATH ""
#endif

#define OTTO_RCR_FUNCTION(returnType, name, arguments, argumentTypes, argumentsForwarded, ...) \
        static returnType name arguments \
        { \
            static returnType(*functionPointer)(argumentTypes) = otto::DllReloader::registerFunctionPointer(OTTO_RCR_DLL_PATH, &functionPointer, \
                otto::DllReloader::getTypeNames<returnType>(), OTTO_RCR_NAMESPACE "::" OTTO_RCR_CLASS "::", #name, otto::DllReloader::getTypeNames<__VA_ARGS__>(), otto::DllReloader::getTypeNames<argumentTypes>(true)); \
            \
            return (*functionPointer) argumentsForwarded; \
        }

#define OTTO_RCR_MEMBER_FUNCTION(returnType, name, arguments, argumentTypes, argumentsForwarded, ...) \
        returnType name arguments \
        { \
            static returnType(std::remove_reference<decltype(*this)>::type::*memberFunctionPointer)(argumentTypes) = otto::DllReloader::registerMemberFunctionPointer( \
                OTTO_RCR_DLL_PATH, &memberFunctionPointer, otto::DllReloader::getTypeNames<returnType>(), #name, otto::DllReloader::getTypeNames<__VA_ARGS__>(), otto::DllReloader::getTypeNames<argumentTypes>(true)); \
            \
            return (this->*memberFunctionPointer) argumentsForwarded; \
        }

#endif


#define OTTO_COMMA ,

#define OTTO_RCR_FUNCTION_0_ARGS(returnType, name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (), , (), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_1_ARGS(returnType, name, arg0Type, arg0Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name), arg0Type, (std::forward<arg0Type>(arg0Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_2_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name), arg0Type OTTO_COMMA arg1Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_3_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_4_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_5_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_6_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_7_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_8_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_9_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_10_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_11_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_12_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_13_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_14_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_15_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_16_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_17_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_18_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_19_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_20_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_21_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_22_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_23_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_24_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_25_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_26_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA \
        arg24Type OTTO_COMMA arg25Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_27_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA \
        arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_28_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA \
        arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_29_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA \
        arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_30_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA \
        arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_31_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA \
        arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA \
        arg29Type OTTO_COMMA arg30Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_32_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA \
        arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA \
        arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_33_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA \
        arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA \
        arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_34_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA \
        arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA \
        arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_35_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA \
        arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA \
        arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_36_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA \
        arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA \
        arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA \
        arg34Type OTTO_COMMA arg35Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_37_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA \
        arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA \
        arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA \
        arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_38_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA \
        arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA \
        arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA \
        arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_39_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA \
        arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA \
        arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA \
        arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_40_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA \
        arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA \
        arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA \
        arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_41_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA \
        arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA \
        arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA \
        arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA \
        arg39Type OTTO_COMMA arg40Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), \
        std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_42_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA \
        arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA \
        arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA \
        arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA \
        arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA arg41Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), \
        std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_43_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA \
        arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA \
        arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA \
        arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA \
        arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA arg41Type OTTO_COMMA arg42Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), \
        std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_44_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA \
        arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA \
        arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA \
        arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA \
        arg41Type OTTO_COMMA arg42Type OTTO_COMMA arg43Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), \
        std::forward<arg43Type>(arg43Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_45_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA \
        arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA \
        arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA \
        arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA \
        arg40Type OTTO_COMMA arg41Type OTTO_COMMA arg42Type OTTO_COMMA arg43Type OTTO_COMMA arg44Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), \
        std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_46_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA \
        arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA \
        arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA \
        arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA \
        arg39Type OTTO_COMMA arg40Type OTTO_COMMA arg41Type OTTO_COMMA arg42Type OTTO_COMMA arg43Type OTTO_COMMA \
        arg44Type OTTO_COMMA arg45Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), \
        std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), \
        std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_47_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA \
        arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA \
        arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA \
        arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA \
        arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA arg41Type OTTO_COMMA arg42Type OTTO_COMMA \
        arg43Type OTTO_COMMA arg44Type OTTO_COMMA arg45Type OTTO_COMMA arg46Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), \
        std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), \
        std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_48_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA \
        arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA \
        arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA \
        arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA \
        arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA arg41Type OTTO_COMMA \
        arg42Type OTTO_COMMA arg43Type OTTO_COMMA arg44Type OTTO_COMMA arg45Type OTTO_COMMA arg46Type OTTO_COMMA arg47Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), \
        std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), \
        std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_49_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, \
    arg48Type, arg48Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name, arg48Type arg48Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA \
        arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA \
        arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA \
        arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA \
        arg41Type OTTO_COMMA arg42Type OTTO_COMMA arg43Type OTTO_COMMA arg44Type OTTO_COMMA arg45Type OTTO_COMMA \
        arg46Type OTTO_COMMA arg47Type OTTO_COMMA arg48Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), \
        std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name), \
        std::forward<arg48Type>(arg48Name)), __VA_ARGS__)

#define OTTO_RCR_FUNCTION_50_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, \
    arg48Type, arg48Name, arg49Type, arg49Name, ...) \
        OTTO_RCR_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name, arg48Type arg48Name, arg49Type arg49Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA \
        arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA \
        arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA \
        arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA \
        arg40Type OTTO_COMMA arg41Type OTTO_COMMA arg42Type OTTO_COMMA arg43Type OTTO_COMMA arg44Type OTTO_COMMA \
        arg45Type OTTO_COMMA arg46Type OTTO_COMMA arg47Type OTTO_COMMA arg48Type OTTO_COMMA arg49Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), \
        std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), \
        std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name), std::forward<arg48Type>(arg48Name), std::forward<arg49Type>(arg49Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_0_ARGS(returnType, name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (), , (), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_1_ARGS(returnType, name, arg0Type, arg0Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name), arg0Type, (std::forward<arg0Type>(arg0Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_2_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name), arg0Type OTTO_COMMA arg1Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_3_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_4_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_5_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_6_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_7_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_8_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_9_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_10_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_11_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_12_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_13_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_14_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_15_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_16_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_17_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_18_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_19_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_20_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_21_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_22_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_23_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_24_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_25_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_26_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA \
        arg24Type OTTO_COMMA arg25Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_27_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA \
        arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_28_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA \
        arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_29_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA \
        arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_30_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA \
        arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_31_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA \
        arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA \
        arg29Type OTTO_COMMA arg30Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_32_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA \
        arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA \
        arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_33_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA \
        arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA \
        arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_34_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA \
        arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA \
        arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_35_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA \
        arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA \
        arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_36_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA \
        arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA \
        arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA \
        arg34Type OTTO_COMMA arg35Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_37_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA \
        arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA \
        arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA \
        arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_38_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA \
        arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA \
        arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA \
        arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_39_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA \
        arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA \
        arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA \
        arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_40_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA \
        arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA \
        arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA \
        arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_41_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA \
        arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA \
        arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA \
        arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA \
        arg39Type OTTO_COMMA arg40Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), \
        std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_42_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA \
        arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA \
        arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA \
        arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA \
        arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA arg41Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), \
        std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_43_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA \
        arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA \
        arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA \
        arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA \
        arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA arg41Type OTTO_COMMA arg42Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), \
        std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_44_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA \
        arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA \
        arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA \
        arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA \
        arg41Type OTTO_COMMA arg42Type OTTO_COMMA arg43Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), \
        std::forward<arg43Type>(arg43Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_45_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA \
        arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA \
        arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA \
        arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA \
        arg40Type OTTO_COMMA arg41Type OTTO_COMMA arg42Type OTTO_COMMA arg43Type OTTO_COMMA arg44Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), \
        std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_46_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA \
        arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA \
        arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA \
        arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA \
        arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA \
        arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA \
        arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA \
        arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA \
        arg39Type OTTO_COMMA arg40Type OTTO_COMMA arg41Type OTTO_COMMA arg42Type OTTO_COMMA arg43Type OTTO_COMMA \
        arg44Type OTTO_COMMA arg45Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), \
        std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), \
        std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), \
        std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), \
        std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), \
        std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), \
        std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), \
        std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), \
        std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), \
        std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_47_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA \
        arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA \
        arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA \
        arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA \
        arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA \
        arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA \
        arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA \
        arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA \
        arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA arg41Type OTTO_COMMA arg42Type OTTO_COMMA \
        arg43Type OTTO_COMMA arg44Type OTTO_COMMA arg45Type OTTO_COMMA arg46Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), \
        std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), \
        std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), \
        std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), \
        std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), \
        std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), \
        std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), \
        std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), \
        std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), \
        std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_48_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name), arg0Type OTTO_COMMA arg1Type OTTO_COMMA \
        arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA arg6Type OTTO_COMMA \
        arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA arg11Type OTTO_COMMA \
        arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA arg16Type OTTO_COMMA \
        arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA arg21Type OTTO_COMMA \
        arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA arg26Type OTTO_COMMA \
        arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA arg31Type OTTO_COMMA \
        arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA arg36Type OTTO_COMMA \
        arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA arg41Type OTTO_COMMA \
        arg42Type OTTO_COMMA arg43Type OTTO_COMMA arg44Type OTTO_COMMA arg45Type OTTO_COMMA arg46Type OTTO_COMMA arg47Type, ( \
        std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), \
        std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), \
        std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), \
        std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), \
        std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), \
        std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), \
        std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), \
        std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), \
        std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), \
        std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_49_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, \
    arg48Type, arg48Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name, arg48Type arg48Name), arg0Type OTTO_COMMA \
        arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA arg5Type OTTO_COMMA \
        arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA arg10Type OTTO_COMMA \
        arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA arg15Type OTTO_COMMA \
        arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA arg20Type OTTO_COMMA \
        arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA arg25Type OTTO_COMMA \
        arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA arg30Type OTTO_COMMA \
        arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA arg35Type OTTO_COMMA \
        arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA arg40Type OTTO_COMMA \
        arg41Type OTTO_COMMA arg42Type OTTO_COMMA arg43Type OTTO_COMMA arg44Type OTTO_COMMA arg45Type OTTO_COMMA \
        arg46Type OTTO_COMMA arg47Type OTTO_COMMA arg48Type, (std::forward<arg0Type>(arg0Name), std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), \
        std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), \
        std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), \
        std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), \
        std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), \
        std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), \
        std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), \
        std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), \
        std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), \
        std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name), \
        std::forward<arg48Type>(arg48Name)), __VA_ARGS__)

#define OTTO_RCR_MEMBER_FUNCTION_50_ARGS(returnType, name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name, \
    arg3Type, arg3Name, arg4Type, arg4Name, arg5Type, arg5Name, arg6Type, arg6Name, arg7Type, arg7Name, \
    arg8Type, arg8Name, arg9Type, arg9Name, arg10Type, arg10Name, arg11Type, arg11Name, arg12Type, arg12Name, \
    arg13Type, arg13Name, arg14Type, arg14Name, arg15Type, arg15Name, arg16Type, arg16Name, arg17Type, arg17Name, \
    arg18Type, arg18Name, arg19Type, arg19Name, arg20Type, arg20Name, arg21Type, arg21Name, arg22Type, arg22Name, \
    arg23Type, arg23Name, arg24Type, arg24Name, arg25Type, arg25Name, arg26Type, arg26Name, arg27Type, arg27Name, \
    arg28Type, arg28Name, arg29Type, arg29Name, arg30Type, arg30Name, arg31Type, arg31Name, arg32Type, arg32Name, \
    arg33Type, arg33Name, arg34Type, arg34Name, arg35Type, arg35Name, arg36Type, arg36Name, arg37Type, arg37Name, \
    arg38Type, arg38Name, arg39Type, arg39Name, arg40Type, arg40Name, arg41Type, arg41Name, arg42Type, arg42Name, \
    arg43Type, arg43Name, arg44Type, arg44Name, arg45Type, arg45Name, arg46Type, arg46Name, arg47Type, arg47Name, \
    arg48Type, arg48Name, arg49Type, arg49Name, ...) \
        OTTO_RCR_MEMBER_FUNCTION(returnType, name, (arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, arg3Type arg3Name, arg4Type arg4Name, \
        arg5Type arg5Name, arg6Type arg6Name, arg7Type arg7Name, arg8Type arg8Name, arg9Type arg9Name, \
        arg10Type arg10Name, arg11Type arg11Name, arg12Type arg12Name, arg13Type arg13Name, arg14Type arg14Name, \
        arg15Type arg15Name, arg16Type arg16Name, arg17Type arg17Name, arg18Type arg18Name, arg19Type arg19Name, \
        arg20Type arg20Name, arg21Type arg21Name, arg22Type arg22Name, arg23Type arg23Name, arg24Type arg24Name, \
        arg25Type arg25Name, arg26Type arg26Name, arg27Type arg27Name, arg28Type arg28Name, arg29Type arg29Name, \
        arg30Type arg30Name, arg31Type arg31Name, arg32Type arg32Name, arg33Type arg33Name, arg34Type arg34Name, \
        arg35Type arg35Name, arg36Type arg36Name, arg37Type arg37Name, arg38Type arg38Name, arg39Type arg39Name, \
        arg40Type arg40Name, arg41Type arg41Name, arg42Type arg42Name, arg43Type arg43Name, arg44Type arg44Name, \
        arg45Type arg45Name, arg46Type arg46Name, arg47Type arg47Name, arg48Type arg48Name, arg49Type arg49Name), \
        arg0Type OTTO_COMMA arg1Type OTTO_COMMA arg2Type OTTO_COMMA arg3Type OTTO_COMMA arg4Type OTTO_COMMA \
        arg5Type OTTO_COMMA arg6Type OTTO_COMMA arg7Type OTTO_COMMA arg8Type OTTO_COMMA arg9Type OTTO_COMMA \
        arg10Type OTTO_COMMA arg11Type OTTO_COMMA arg12Type OTTO_COMMA arg13Type OTTO_COMMA arg14Type OTTO_COMMA \
        arg15Type OTTO_COMMA arg16Type OTTO_COMMA arg17Type OTTO_COMMA arg18Type OTTO_COMMA arg19Type OTTO_COMMA \
        arg20Type OTTO_COMMA arg21Type OTTO_COMMA arg22Type OTTO_COMMA arg23Type OTTO_COMMA arg24Type OTTO_COMMA \
        arg25Type OTTO_COMMA arg26Type OTTO_COMMA arg27Type OTTO_COMMA arg28Type OTTO_COMMA arg29Type OTTO_COMMA \
        arg30Type OTTO_COMMA arg31Type OTTO_COMMA arg32Type OTTO_COMMA arg33Type OTTO_COMMA arg34Type OTTO_COMMA \
        arg35Type OTTO_COMMA arg36Type OTTO_COMMA arg37Type OTTO_COMMA arg38Type OTTO_COMMA arg39Type OTTO_COMMA \
        arg40Type OTTO_COMMA arg41Type OTTO_COMMA arg42Type OTTO_COMMA arg43Type OTTO_COMMA arg44Type OTTO_COMMA \
        arg45Type OTTO_COMMA arg46Type OTTO_COMMA arg47Type OTTO_COMMA arg48Type OTTO_COMMA arg49Type, (std::forward<arg0Type>(arg0Name), \
        std::forward<arg1Type>(arg1Name), std::forward<arg2Type>(arg2Name), std::forward<arg3Type>(arg3Name), std::forward<arg4Type>(arg4Name), std::forward<arg5Type>(arg5Name), \
        std::forward<arg6Type>(arg6Name), std::forward<arg7Type>(arg7Name), std::forward<arg8Type>(arg8Name), std::forward<arg9Type>(arg9Name), std::forward<arg10Type>(arg10Name), \
        std::forward<arg11Type>(arg11Name), std::forward<arg12Type>(arg12Name), std::forward<arg13Type>(arg13Name), std::forward<arg14Type>(arg14Name), std::forward<arg15Type>(arg15Name), \
        std::forward<arg16Type>(arg16Name), std::forward<arg17Type>(arg17Name), std::forward<arg18Type>(arg18Name), std::forward<arg19Type>(arg19Name), std::forward<arg20Type>(arg20Name), \
        std::forward<arg21Type>(arg21Name), std::forward<arg22Type>(arg22Name), std::forward<arg23Type>(arg23Name), std::forward<arg24Type>(arg24Name), std::forward<arg25Type>(arg25Name), \
        std::forward<arg26Type>(arg26Name), std::forward<arg27Type>(arg27Name), std::forward<arg28Type>(arg28Name), std::forward<arg29Type>(arg29Name), std::forward<arg30Type>(arg30Name), \
        std::forward<arg31Type>(arg31Name), std::forward<arg32Type>(arg32Name), std::forward<arg33Type>(arg33Name), std::forward<arg34Type>(arg34Name), std::forward<arg35Type>(arg35Name), \
        std::forward<arg36Type>(arg36Name), std::forward<arg37Type>(arg37Name), std::forward<arg38Type>(arg38Name), std::forward<arg39Type>(arg39Name), std::forward<arg40Type>(arg40Name), \
        std::forward<arg41Type>(arg41Name), std::forward<arg42Type>(arg42Name), std::forward<arg43Type>(arg43Name), std::forward<arg44Type>(arg44Name), std::forward<arg45Type>(arg45Name), \
        std::forward<arg46Type>(arg46Name), std::forward<arg47Type>(arg47Name), std::forward<arg48Type>(arg48Name), std::forward<arg49Type>(arg49Name)), __VA_ARGS__)
