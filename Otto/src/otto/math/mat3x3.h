#pragma once

#include "otto/math/mat.h"
#include "otto/math/vec3.h"
#include "otto/math/functions.h"
#include "otto/util/static_array.h"

namespace otto
{
    template<typename T> requires isIntegral<T>
    struct Mat<3, 3, T> 
    {
        Mat()
            : mData(StaticArray<T, 9>(T(0)))
        {
        }

        template<typename F>
        explicit Mat(F value) requires isIntegral<F>
            : m11(T(value)), m12(T(0)),     m13(T(0)),
              m21(T(0)),     m22(T(value)), m23(T(0)),
              m31(T(0)),     m32(T(0)),     m33(T(value))
        {
        }

        template<typename F>
        Mat(F m11, F m12, F m13, 
            F m21, F m22, F m23,
            F m31, F m32, F m33) requires isIntegral<F>
            : m11(T(m11)), m12(T(m12)), m13(T(m13)),
              m21(T(m21)), m22(T(m22)), m23(T(m23)),
              m31(T(m31)), m32(T(m32)), m33(T(m33))
        {
        }

        template<typename F>
        Mat(const Vec<3, F>& col0, const Vec<3, F>& col1, const Vec<3, F>& col2)
            : mCols({ Vec<3, T>(col0), Vec<3, T>(col1), Vec<3, T>(col2) })
        {
        }

        template<typename F>
        Mat(const Mat<3, 3, F>& other)
        {
            *this = other;
        }

        ~Mat()
        {
            mData.~StaticArray<T, 9>();
        }

        template<typename F>
        Mat& operator=(const Mat<3, 3, F>& other)
        {
            mCols[0] = other.mCols[0];
            mCols[1] = other.mCols[1];
            mCols[2] = other.mCols[2];

            return *this;
        }

        template<typename F>
        Mat& operator+=(const Mat<3, 3, F>& mat)
        {
            mCols[0] += mat.mCols[0];
            mCols[1] += mat.mCols[1];
            mCols[2] += mat.mCols[2];

            return *this;
        }

        template<typename F>
        Mat& operator-=(const Mat<3, 3, F>& mat)
        {
            mCols[0] -= mat.mCols[0];
            mCols[1] -= mat.mCols[1];
            mCols[2] -= mat.mCols[2];

            return *this;
        }

        template<typename F>
        Mat& operator*=(F value) requires isIntegral<F>
        {
            mCols[0] *= value;
            mCols[1] *= value;
            mCols[2] *= value;

            return *this;
        }

        template<typename F>
        Mat& operator*=(const Mat<3, 3, F>& mat)
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
                T m11, m21, m31;        // col 1
                T m12, m22, m32;        // col 2
                T m13, m23, m33;        // col 3
            };

            StaticArray<T, 9> mData;
            StaticArray<Vec<3, T>, 3> mCols;
        };
    };

    template<typename T, typename F>
    Mat<3, 3, T> operator*(F value, const Mat<3, 3, T>& mat) requires isIntegral<F>
    {
        return {
            value * mat.m11, value * mat.m12, value * mat.m13,
            value * mat.m21, value * mat.m22, value * mat.m23,
            value * mat.m31, value * mat.m32, value * mat.m33,
        };
    }

    template<typename T, typename F>
    Mat<3, 3, T> operator*(const Mat<3, 3, T>& mat, F value) requires isIntegral<F>
    {
        return value * mat;
    }

    template<typename T, typename F>
    Mat<3, 3, T> operator+(const Mat<3, 3, T>& a, const Mat<3, 3, F>& b)
    {
        return {
            a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13,
            a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23,
            a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33,
        };
    }

    template<typename T, typename F>
    Mat<3, 3, T> operator*(const Mat<3, 3, T>& a, const Mat<3, 3, F>& b)
    {
        T srcA00 = a.mCols[0][0];
        T srcA01 = a.mCols[0][1];
        T srcA02 = a.mCols[0][2];
        T srcA10 = a.mCols[1][0];
        T srcA11 = a.mCols[1][1];
        T srcA12 = a.mCols[1][2];
        T srcA20 = a.mCols[2][0];
        T srcA21 = a.mCols[2][1];
        T srcA22 = a.mCols[2][2];

        T srcB00 = b.mCols[0][0];
        T srcB01 = b.mCols[0][1];
        T srcB02 = b.mCols[0][2];
        T srcB10 = b.mCols[1][0];
        T srcB11 = b.mCols[1][1];
        T srcB12 = b.mCols[1][2];
        T srcB20 = b.mCols[2][0];
        T srcB21 = b.mCols[2][1];
        T srcB22 = b.mCols[2][2];

        Mat<3, 3, T> result;
        result.mCols[0][0] = srcA00 * srcB00 + srcA10 * srcB01 + srcA20 * srcB02;
        result.mCols[0][1] = srcA01 * srcB00 + srcA11 * srcB01 + srcA21 * srcB02;
        result.mCols[0][2] = srcA02 * srcB00 + srcA12 * srcB01 + srcA22 * srcB02;
        result.mCols[1][0] = srcA00 * srcB10 + srcA10 * srcB11 + srcA20 * srcB12;
        result.mCols[1][1] = srcA01 * srcB10 + srcA11 * srcB11 + srcA21 * srcB12;
        result.mCols[1][2] = srcA02 * srcB10 + srcA12 * srcB11 + srcA22 * srcB12;
        result.mCols[2][0] = srcA00 * srcB20 + srcA10 * srcB21 + srcA20 * srcB22;
        result.mCols[2][1] = srcA01 * srcB20 + srcA11 * srcB21 + srcA21 * srcB22;
        result.mCols[2][2] = srcA02 * srcB20 + srcA12 * srcB21 + srcA22 * srcB22;

        return result;
    }

    template<typename T, typename F>
    Vec<3, T> operator*(const Mat<3, 3, T>& mat, const Vec<3, F>& vec)
    {
        return {
            mat.mCols[0][0] * vec.x + mat.mCols[1][0] * vec.y + mat.mCols[2][0] * vec.z,
            mat.mCols[0][1] * vec.x + mat.mCols[1][1] * vec.y + mat.mCols[2][1] * vec.z,
            mat.mCols[0][2] * vec.x + mat.mCols[1][2] * vec.y + mat.mCols[2][2] * vec.z,
        };
    }

    template<typename T, typename F>
    Vec<3, T> operator*(const Vec<3, F>& vec, const Mat<3, 3, T>& mat)
    {
        return {
            mat.mCols[0][0] * vec.x + mat.mCols[0][1] * vec.y + mat.mCols[0][2] * vec.z,
            mat.mCols[1][0] * vec.x + mat.mCols[1][1] * vec.y + mat.mCols[1][2] * vec.z,
            mat.mCols[2][0] * vec.x + mat.mCols[2][1] * vec.y + mat.mCols[2][2] * vec.z,
        };
    }

    template<typename T, typename F>
    bool8 operator==(const Mat<3, 3, T>& a, const Mat<3, 3, F>& b)
    {
        return a.mCols[0] == b.mCols[0] && a.mCols[1] == b.mCols[1] && a.mCols[2] == b.mCols[2];
    }

    template<typename T, typename F>
    bool8 operator!=(const Mat<3, 3, T>& a, const Mat<3, 3, F>& b)
    {
        return !(a == b);
    }

} // namespace otto
