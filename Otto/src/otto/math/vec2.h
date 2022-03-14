#pragma once

#include "otto/base.h"
#include "otto/math/vec.h"
#include "otto/util/string.h"
#include "otto/math/functions.h"
#include "otto/util/static_array.h"

namespace otto
{
    template<typename T> requires isIntegral<T>
    struct Vec<2, T> 
    {
        Vec()
            : mData(StaticArray<T, 2>(T(0)))
        {
        }

        template<typename F>
        explicit Vec(F value) requires isIntegral<F>
            : mData(StaticArray<T, 2>(T(value)))
        {
        }

        template<typename F>
        Vec(F v1, F v2) requires isIntegral<F>
            : mData({ T(v1), T(v2) })
        {
        }

        template<typename F>
        Vec(const Vec<2, F>& other)
        {
            *this = other;
        }

        ~Vec()
        {
            mData.~StaticArray<T, 2>();
        }

        template<typename F>
        Vec& operator=(const Vec<2, F>& other)
        {
            mData[0] = other.mData[0];
            mData[1] = other.mData[1];

            return *this;
        }

        template<typename F>
        Vec& operator+=(const Vec<2, F>& other)
        {
            mData[0] += other.mData[0];
            mData[1] += other.mData[1];

            return *this;
        }

        template<typename F>
        Vec& operator-=(const Vec<2, F>& other)
        {
            mData[0] -= other.mData[0];
            mData[1] -= other.mData[1];

            return *this;
        }

        template<typename F>
        Vec& operator*=(F value) requires isIntegral<F>
        {
            mData[0] *= value;
            mData[1] *= value;

            return *this;
        }

        T& operator[](uint64 index)
        {
            return mData[index];
        }

        const T& operator[](uint64 index) const
        {
            return mData[index];
        }

        T* getData()
        {
            return mData.getData();
        }

        const T* getData() const
        {
            return mData.getData();
        }

        String toString() const
        {
            return String::valueOf(x) + ", " + String::valueOf(y);
        }

        union
        {
            struct
            {
                T x;
                T y;
            };

            struct
            {
                T width;
                T height;
            };

            StaticArray<T, 2> mData;
        };
    };

    template<typename T, typename F>
    Vec<2, T> operator+(const Vec<2, T>& v1, const Vec<2, F>& v2)
    {
        return {
            v1.mData[0] + v2.mData[0],
            v1.mData[1] + v2.mData[1],
        };
    }

    template<typename T, typename F>
    Vec<2, T> operator-(const Vec<2, T>& v1, const Vec<2, F>& v2)
    {
        return {
            v1.mData[0] - v2.mData[0],
            v1.mData[1] - v2.mData[1],
        };
    }

    template<typename T, typename F>
    Vec<2, T> operator*(F value, const Vec<2, T>& vec) requires isIntegral<F>
    {
        return {
            vec.mData[0] * value,
            vec.mData[1] * value,
        };
    }

    template<typename T, typename F>
    Vec<2, T> operator*(const Vec<2, T>& vec, F value) requires isIntegral<F>
    {
        return value * vec;
    }

    template<typename T, typename F>
    bool8 operator==(const Vec<2, T>& v1, const Vec<2, F>& v2)
    {
        return v1.mData[0] == v2.mData[0] &&
            v2.mData[1] == v2.mData[1];
    }

    template<typename T, typename F>
    bool8 operator!=(const Vec<2, T>& v1, const Vec<2, F>& v2)
    {
        return !(v1 == v2);
    }

    template<typename T>
    T length(const Vec<2, T>& vec)
    {
        return sqrt(vec.x * vec.x + vec.y * vec.y);
    }

    template<typename T, typename F>
    T dot(const Vec<2, T>& v1, const Vec<2, F>& v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template<typename T, typename F>
    T distance(const Vec<2, T>& v1, const Vec<2, F>& v2)
    {
        return length(v2 - v1);
    }

    template<typename T>
    Vec<2, T> normalize(const Vec<2, T>& vec)
    {
        return (1 / length(vec)) * vec;
    }

} // namespace otto
