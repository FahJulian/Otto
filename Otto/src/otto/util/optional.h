#pragma once

namespace otto
{
    struct _Nullopt
    {
    };

    template<typename T>
    class Optional
    {
    public:
        constexpr Optional()
            : mHasValue(false), _()
        {
        }

        constexpr Optional(_Nullopt _)
            : mHasValue(false), _()
        {
        }

        Optional(const T& value)
            : mHasValue(true), mValue(value)
        {
        }

        ~Optional()
        {
            if (mHasValue)
                mValue.~T();
        }

        operator bool8() const
        {
            return mHasValue;
        }

        T* operator->()
        {
            return &mValue;
        }

        const T* operator->() const
        {
            return &mValue;
        }

        bool8 hasValue() const
        {
            return mHasValue;
        }

        T& getValue()
        {
            return mValue;
        }

        const T& getValue() const
        {
            return mValue;
        }

    private:
        bool8 mHasValue;

        union
        {
            T mValue;
            char8 _;
        };
    };

    constexpr _Nullopt NULLOPT = { };

} // namespace otto
