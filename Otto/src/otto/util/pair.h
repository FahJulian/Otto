#pragma once

#include "otto/base.h"

namespace otto
{
    template<typename K, typename V>
    struct Pair
    {
        Pair() = default;

        Pair(const K& first, const V& second)
            : first(first), second(second)
        {
        }

        template<typename T, typename F>
        Pair(const T& first, const F& second)
            : first(first), second(second)
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
            second = std::move(otherValue);

            return *this;
        }

        template<typename T, typename F>
        Pair& operator=(const Pair<T, F>& other)
        {
            first = other.first;
            second = other.second;

            return *this;
        }

        Pair& operator=(const Pair& other)
        {
            first = other.first;
            second = other.second;

            return *this;
        }

        template<typename T, typename F>
        Pair& operator=(Pair<T, F>&& other)
        {
            first = std::move(other.first);
            second = std::move(other.second);

            return *this;
        }

        Pair& operator=(Pair&& other) noexcept
        {
            first = std::move(other.first);
            second = std::move(other.second);

            return *this;
        }

        template<typename T, typename F>
        bool operator==(const Pair<T, F>& other) const
        {
            return first == other.first;
        }

        template<typename T>
        bool operator==(const T& otherKey) const
        {
            return first == otherKey;
        }

        K first;
        V second;
    };

} // namespace otto

