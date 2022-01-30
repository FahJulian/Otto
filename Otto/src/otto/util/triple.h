#pragma once

#include "otto/base.h"

namespace otto
{
    template<typename V1, typename V2, typename V3>
    struct Triple
    {
        Triple() = default;

        Triple(const V1& first, const V2& second, const V3& third)
            : first(first), second(second), third(third)
        {
        }

        template<typename T, typename F, typename G>
        Triple(const T& first, const F& second, const G& third)
            : first(first), second(second), third(third)
        {
        }

        template<typename T, typename F, typename G>
        Triple(Triple<T, F, G>&& other)
        {
            *this = std::move(other);
        }

        template<typename T, typename F, typename G>
        Triple(const Triple<T, F, G>& other)
        {
            *this = other;
        }

        Triple(const Triple& other)
        {
            *this = other;
        }

        ~Triple() = default;

        template<typename T, typename F, typename G>
        Triple& operator=(const Triple<T, F, G>& other)
        {
            first = other.first;
            second = other.second;

            return *this;
        }

        Triple& operator=(const Triple& other)
        {
            first = other.first;
            second = other.second;

            return *this;
        }

        template<typename T, typename F, typename G>
        Triple& operator=(Triple<T, F, G>&& other)
        {
            first = std::move(other.first);
            second = std::move(other.second);

            return *this;
        }

        Triple& operator=(Triple&& other)
        {
            first = std::move(other.first);
            second = std::move(other.second);

            return *this;
        }

        template<typename T, typename F, typename G>
        bool operator==(const Triple<T, F, G>& other) const
        {
            return first == other.first;
        }

        V1 first;
        V2 second;
        V3 third;
    };

} // namespace otto
