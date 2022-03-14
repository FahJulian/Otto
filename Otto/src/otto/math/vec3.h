#pragma once

#include "otto/base.h"
#include "otto/math/vec.h"
#include "otto/math/vec2.h"
#include "otto/util/string.h"
#include "otto/math/functions.h"
#include "otto/util/static_array.h"

namespace otto
{
    template<typename T> requires isIntegral<T>
    struct Vec<3, T>
    {
        Vec()
            : mData(StaticArray<T, 3>(T(0)))
        {
        }

        template<typename F>
        explicit Vec(F value) requires isIntegral<F>
            : mData(StaticArray<T, 3>(T(value)))
        {
        }

        template<typename F>
        Vec(F v1, F v2, F v3) requires isIntegral<F>
            : mData({ T(v1), T(v2), T(v3) })
        {
        }

        template<typename F>
        Vec(const Vec<2, F>& vec, F v3 = F(0))
            : mData({ T(vec.x), T(vec.y), T(v3) })
        {
        }

        template<typename F>
        Vec(F v1, const Vec<2, F>& vec)
            : mData({ T(v1), T(vec.x), T(vec.y) })
        {
        }

        template<typename F>
        Vec(const Vec<3, F>& other)
        {
            *this = other;
        }

        ~Vec()
        {
            mData.~StaticArray<T, 3>();
        }

        template<typename F>
        Vec& operator=(const Vec<3, F>& other)
        {
            mData[0] = other.mData[0];
            mData[1] = other.mData[1];
            mData[2] = other.mData[2];

            return *this;
        }

        template<typename F>
        Vec& operator+=(const Vec<3, F>& other)
        {
            mData[0] += other.mData[0];
            mData[1] += other.mData[1];
            mData[2] += other.mData[2];

            return *this;
        }

        template<typename F>
        Vec& operator-=(const Vec<3, F>& other)
        {
            mData[0] -= other.mData[0];
            mData[1] -= other.mData[1];
            mData[2] -= other.mData[2];

            return *this;
        }

        template<typename F>
        Vec& operator*=(F value) requires isIntegral<F>
        {
            mData[0] *= value;
            mData[1] *= value;
            mData[2] *= value;

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
            return String::valueOf(x) + ", " + String::valueOf(y) + ", " + String::valueOf(z);
        }

        union
        {
            struct
            {
                T x;
                T y;
                T z;
            };

            struct
            {
                T width;
                T height;
                T depth;
            };

            StaticArray<T, 3> mData;
        };
    };

    template<typename T, typename F>
    Vec<3, T> operator+(const Vec<3, T>& v1, const Vec<3, F>& v2)
    {
        return {
            v1.mData[0] + v2.mData[0],
            v1.mData[1] + v2.mData[1],
            v1.mData[2] + v2.mData[2],
        };
    }

    template<typename T, typename F>
    Vec<3, T> operator-(const Vec<3, T>& v1, const Vec<3, F>& v2)
    {
        return {
            v1.mData[0] - v2.mData[0],
            v1.mData[1] - v2.mData[1],
            v1.mData[2] - v2.mData[2],
        };
    }

    template<typename T, typename F>
    Vec<3, T> operator*(F value, const Vec<3, T>& vec) requires isIntegral<F>
    {
        return {
            vec.mData[0] * value,
            vec.mData[1] * value,
            vec.mData[2] * value,
        };
    }

    template<typename T, typename F>
    Vec<3, T> operator*(const Vec<3, T>& vec, F value) requires isIntegral<F>
    {
        return value * vec;
    }

    template<typename T, typename F>
    bool8 operator==(const Vec<3, T>& v1, const Vec<3, F>& v2)
    {
        return v1.mData[0] == v2.mData[0] &&
            v2.mData[1] == v2.mData[1] &&
            v2.mData[2] == v2.mData[2];
    }

    template<typename T, typename F>
    bool8 operator!=(const Vec<3, T>& v1, const Vec<3, F>& v2)
    {
        return !(v1 == v2);
    }

} // namespace otto
