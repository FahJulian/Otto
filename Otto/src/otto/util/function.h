#pragma once

#include <cstdint>
#include <utility>

#include "otto/base.h"

namespace otto
{
    template<typename R, typename... Args>
    class Function
    {
        static constexpr bool8 ALWAYS_FALSE = false;
        static_assert(ALWAYS_FALSE, "otto::Function can only be used with function typenames");
    };


    template<typename R, typename... Args>
    class Function<R(Args...)>
    {
    private:
        template<typename T>
        using MemberFunction = R(T::*)(Args...) const;

        using NonMemberFunction = R(* const)(Args...);

        using FunctionCaller = R(Function::*)(Args&&...) const;

    public:
        constexpr Function()
            : mObject(0), mFunction(0), mFunctionCaller(0)
        {
        }

        Function(NonMemberFunction Function)
            : mObject(0), mFunction(*reinterpret_cast<const ptr64*>(&Function)),
            mFunctionCaller(Function != nullptr ? &Function::_callFunction : nullptr)
        {
        }

        template<typename T>
        Function(T* object, MemberFunction<T> memberFunction)
            : mObject(reinterpret_cast<ptr64>(object)), mFunction(*reinterpret_cast<ptr64*>(&memberFunction)),
            mFunctionCaller((object != nullptr && memberFunction != nullptr) ? &Function::_callMemberFunction<T> : nullptr)
        {
        }

        R operator()(Args... args) const
        {
            return (this->*mFunctionCaller)(std::forward<Args>(args)...);
        }

        operator bool8() const
        {
            return isValid();
        }

        bool8 isValid() const
        {
            return mFunctionCaller != nullptr;
        }

        bool8 isFunction() const
        {
            return mFunctionCaller == &Function::_callFunction;
        }

        bool8 isMemberFunction() const
        {
            return isValid() && !isFunction();
        }

        template<typename R1, typename... Args1>
        bool8 isFunction(R1(*Function)(Args...)) const
        {
            return Function != nullptr && mFunction == *reinterpret_cast<ptr64*>(&Function);
        }

        template<typename T, typename R1, typename... Args1>
        bool8 isMemberFunction(R1(T::* memberFunction)(Args1...)) const
        {
            return memberFunction != nullptr && mFunction == *reinterpret_cast<ptr64*>(&memberFunction);
        }

        template<typename T>
        bool8 isMemberFunctionOfObject(T* object) const
        {
            return object != nullptr && mObject == reinterpret_cast<ptr64>(object);
        }

    private:
        R _callFunction(Args&&... args) const
        {
            NonMemberFunction Function = *reinterpret_cast<NonMemberFunction*>(&mFunction);
            return Function(std::forward<Args>(args)...);
        }

        template<typename T>
        R _callMemberFunction(Args&&... args) const
        {
            T* object = reinterpret_cast<T*>(mObject);
            MemberFunction<T> memberFunction = *reinterpret_cast<MemberFunction<T>*>(&mFunction);

            return (object->*memberFunction)(std::forward<Args>(args)...);
        }

        ptr64 mObject;
        ptr64 mFunction;
        FunctionCaller mFunctionCaller;
    };

} // namespace otto
