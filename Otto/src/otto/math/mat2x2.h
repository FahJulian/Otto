#pragma once

#include "otto/math/mat.h"
#include "otto/math/functions.h"
#include "otto/util/static_array.h"

namespace otto
{
    template<typename T> requires isIntegral<T>
    struct Mat<2, 2, T> 
    {
        Mat()
            : mData(StaticArray<T, 4>(T(0)))
        {
        }

        template<typename F>
        explicit Mat(F value) requires isIntegral<F>
            : m11(T(value)), m12(T(0)),
              m21(T(0)),     m22(T(value))
        {
        }

        template<typename F>
        Mat(F m11, F m12, 
            F m21, F m22) requires isIntegral<F>
            : m11(T(m11)), m12(T(m12)),
              m21(T(m21)), m22(T(m22))
        {
        }

        template<typename F>
        Mat(const Vec<2, F>& col0, const Vec<2, F>& col1)
            : mCols({ Vec<2, T>(col0), Vec<2, T>(col1) })
        {
        }

        template<typename F>
        Mat(const Mat<2, 2, F>& other)
        {
            *this = other;
        }

        ~Mat()
        {
            mData.~StaticArray<T, 4>();
        }

        template<typename F>
        Mat& operator=(const Mat<2, 2, F>& other)
        {
            mCols[0] = other.mCols[0];
            mCols[1] = other.mCols[1];

            return *this;
        }

        template<typename F>
        Mat& operator+=(const Mat<2, 2, F>& mat)
        {
            mCols[0] += mat.mCols[0];
            mCols[1] += mat.mCols[1];

            return *this;
        }

        template<typename F>
        Mat& operator-=(const Mat<2, 2, F>& mat)
        {
            mCols[0] -= mat.mCols[0];
            mCols[1] -= mat.mCols[1];

            return *this;
        }

        template<typename F>
        Mat& operator*=(F value) requires isIntegral<F>
        {
            mCols[0] *= value;
            mCols[1] *= value;

            return *this;
        }

        template<typename F>
        Mat& operator*=(const Mat<2, 2, F>& mat)
        {
            *this = *this * mat;
            return *this;
        }

        T* getData()
        {
            return mData.getData();
        }

        const T* getData() const
        {
            return mData.getData();
        }

        static Mat zero()
        {
            return Mat(T(0));
        }

        static Mat identity()
        {
            return Mat(T(1));
        }

        union
        {
            struct
            {
                T m11, m21;     // col 1
                T m12, m22;     // col 2
            };

            StaticArray<T, 4> mData;
            StaticArray<Vec<2, T>, 2> mCols;
        };
    };

    template<typename T, typename F>
    Mat<2, 2, T> operator*(F value, const Mat<2, 2, T>& mat) requires isIntegral<F>
    {
        return {
            value * mat.m11, value * mat.m12,
            value * mat.m21, value * mat.m22,
        };
    }

    template<typename T, typename F>
    Mat<2, 2, T> operator*(const Mat<2, 2, T>& mat, F value) requires isIntegral<F>
    {
        return value * mat;
    }

    template<typename T, typename F>
    Mat<2, 2, T> operator+(const Mat<2, 2, T>& a, const Mat<2, 2, F>& b)
    {
        return {
            a.m11 + b.m11, a.m12 + b.m12,
            a.m21 + b.m21, a.m22 + b.m22,
        };
    }

    template<typename T, typename F>
    Mat<2, 2, T> operator*(const Mat<2, 2, T>& a, const Mat<2, 2, F>& b)
    {
        return {
            a.mCols[0][0] * b.mCols[0][0] + a.mCols[1][0] * b.mCols[0][1],
            a.mCols[0][0] * b.mCols[1][0] + a.mCols[1][0] * b.mCols[1][1],
            a.mCols[0][1] * b.mCols[0][0] + a.mCols[1][1] * b.mCols[0][1],
            a.mCols[0][1] * b.mCols[1][0] + a.mCols[1][1] * b.mCols[1][1],
        };
    }

    template<typename T, typename F>
    Vec<2, T> operator*(const Mat<2, 2, T>& mat, const Vec<2, F>& vec)
    {
        return {
            mat.mCols[0][0] * vec.x + mat.mCols[1][0] * vec.y,
            mat.mCols[0][1] * vec.x + mat.mCols[1][1] * vec.y,
        };
    }

    template<typename T, typename F>
    Vec<2, T> operator*(const Vec<2, T>& vec, const Mat<2, 2, F>& mat)
    {
        return {
            vec.x * mat.mCols[0][0] + vec.y * mat.mCols[0][1],
            vec.x * mat.mCols[1][0] + vec.y * mat.mCols[1][1],
        };
    }

    template<typename T, typename F>
    bool8 operator==(const Mat<2, 2, T>& a, const Mat<2, 2, F>& b)
    {
        return a.m[0] == b.m[0] && a.m[1] == b.m[1];
    }

    template<typename T, typename F>
    bool8 operator!=(const Mat<2, 2, T>& a, const Mat<2, 2, F>& b)
    {
        return !(a == b);
    }

} // namespace otto
