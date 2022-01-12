#pragma once

namespace otto
{
    template<typename K, typename V>
    struct Pair
    {
        Pair() = default;

        Pair(const K& mFirst, const V& mSecond)
            : mFirst(mFirst), mSecond(mSecond)
        {
        }

        template<typename T, typename F>
        Pair(const T& mFirst, const F& mSecond)
            : mFirst(mFirst), mSecond(mSecond)
        {
        }

        template<typename T, typename F>
        Pair(Pair<T, F>&& other)
        {
            *this = std::move(other);
        }

        template<typename T, typename F>
        Pair(const Pair<T, F>& other)
        {
            *this = other;
        }

        Pair(const Pair& other)
        {
            *this = other;
        }

        ~Pair() = default;

        template<typename T>
        Pair& operator=(T&& otherValue)
        {
            mSecond = std::move(otherValue);

            return *this;
        }

        template<typename T, typename F>
        Pair& operator=(const Pair<T, F>& other)
        {
            mFirst = other.mFirst;
            mSecond = other.mSecond;

            return *this;
        }

        Pair& operator=(const Pair& other)
        {
            mFirst = other.mFirst;
            mSecond = other.mSecond;

            return *this;
        }

        template<typename T, typename F>
        Pair& operator=(Pair<T, F>&& other)
        {
            mFirst = std::move(other.mFirst);
            mSecond = std::move(other.mSecond);

            return *this;
        }

        Pair& operator=(Pair&& other)
        {
            mFirst = std::move(other.mFirst);
            mSecond = std::move(other.mSecond);

            return *this;
        }

        template<typename T, typename F>
        bool operator==(const Pair<T, F>& other) const
        {
            return mFirst == other.mFirst;
        }

        template<typename T>
        bool operator==(const T& otherKey) const
        {
            return mFirst == otherKey;
        }

        K mFirst;
        V mSecond;
    };

} // namespace otto

