#pragma once

#include "otto/math/mat.h"
#include "otto/math/vec4.h"
#include "otto/math/functions.h"
#include "otto/util/static_array.h"
#include "otto/math/vec_functions.h"

namespace otto
{
    template<typename T> requires isIntegral<T>
    struct Mat<4, 4, T> 
    {
        Mat()
            : mData(StaticArray<T, 16>(T(0)))
        {
        }

        template<typename F>
        explicit Mat(F value) requires isIntegral<F>
            : m11(T(value)), m12(T(0)),     m13(T(0)),     m14(T(0)),
              m21(T(0)),     m22(T(value)), m23(T(0)),     m24(T(0)),
              m31(T(0)),     m32(T(0)),     m33(T(value)), m34(T(0)),
              m41(T(0)),     m42(T(0)),     m43(T(0)),     m44(T(value))
        {
        }

        template<typename F>
        Mat(F m11, F m12, F m13, F m14,
            F m21, F m22, F m23, F m24,
            F m31, F m32, F m33, F m34, 
            F m41, F m42, F m43, F m44) requires isIntegral<F>
            : m11(T(m11)), m12(T(m12)), m13(T(m13)), m14(T(m14)),
              m21(T(m21)), m22(T(m22)), m23(T(m23)), m24(T(m24)),
              m31(T(m31)), m32(T(m32)), m33(T(m33)), m34(T(m34)),
              m41(T(m41)), m42(T(m42)), m43(T(m43)), m44(T(m44))
        {
        }

        template<typename F>
        Mat(const Vec<4, F>& col0, const Vec<4, F>& col1, const Vec<4, F>& col2, const Vec<4, F>& col3)
            : mCols({ Vec<4, T>(col0), Vec<4, T>(col1), Vec<4, T>(col2), Vec<4, T>(col3) })
        {
        }

        template<typename F>
        Mat(const Mat<4, 4, F>& other)
        {
            *this = other;
        }

        ~Mat()
        {
            mData.~StaticArray<T, 16>();
        }

        template<typename F>
        Mat& operator=(const Mat<4, 4, F>& other)
        {
            mCols[0] = other.mCols[0];
            mCols[1] = other.mCols[1];
            mCols[2] = other.mCols[2];
            mCols[3] = other.mCols[3];

            return *this;
        }

        template<typename F>
        Mat& operator+=(const Mat<4, 4, F>& mat)
        {
            mCols[0] += mat.mCols[0];
            mCols[1] += mat.mCols[1];
            mCols[2] += mat.mCols[2];
            mCols[3] += mat.mCols[3];

            return *this;
        }

        template<typename F>
        Mat& operator-=(const Mat<4, 4, F>& mat)
        {
            mCols[0] -= mat.mCols[0];
            mCols[1] -= mat.mCols[1];
            mCols[2] -= mat.mCols[2];
            mCols[3] -= mat.mCols[3];

            return *this;
        }

        template<typename F>
        Mat& operator*=(F value) requires isIntegral<F>
        {
            mCols[0] *= value;
            mCols[1] *= value;
            mCols[2] *= value;
            mCols[3] *= value;

            return *this;
        }

        template<typename F>
        Mat& operator*=(const Mat<4, 4, F>& mat)
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

        template<typename F> requires (isFloat<F> && isFloat<T>)
        static Mat orthographicProjection(F left, F right, F bottom, F top, F near, F far) 
        {
            Mat result = identity();

            result.mCols[0][0] = T(2) / (right - left);
            result.mCols[1][1] = T(2) / (top - bottom);
            result.mCols[2][2] = T(2) / (far - near);
            result.mCols[3][0] = -(right + left) / (right - left);
            result.mCols[3][1] = -(top + bottom) / (top - bottom);
            result.mCols[3][2] = -(far + near) / (far - near);

            return result;
        }

        template<typename F> requires isIntegral<F>
        static Mat rotation(F angle, const Vec<3, T>& axis)
        {
            T sine = sin(angle);
            T cosine = cos(angle);

            Vec<3, T> axisNormalized = normalize(axis);
            Vec<3, T> temp = (T(1) - cosine) * axisNormalized;

            Mat rotation = identity();
            rotation.mCols[0][0] = cosine + temp[0] * axisNormalized[0];
            rotation.mCols[0][1] = temp[0] * axisNormalized[1] + sine * axisNormalized[2];
            rotation.mCols[0][2] = temp[0] * axisNormalized[2] - sine * axisNormalized[1];

            rotation.mCols[1][0] = temp[1] * axisNormalized[0] - sine * axisNormalized[2];
            rotation.mCols[1][1] = cosine + temp[1] * axisNormalized[1];
            rotation.mCols[1][2] = temp[1] * axisNormalized[2] + sine * axisNormalized[0];

            rotation.mCols[2][0] = temp[2] * axisNormalized[0] + sine * axisNormalized[1];
            rotation.mCols[2][1] = temp[2] * axisNormalized[1] - sine * axisNormalized[0];
            rotation.mCols[2][2] = cosine + temp[2] * axisNormalized[2];

            return rotation;
        }

        template<typename F> requires isIntegral<F>
        static Mat rotationX(F angle)
        {
            return rotation(angle, { T(1), T(0), T(0) });
        }

        template<typename F> requires isIntegral<F>
        static Mat rotationY(F angle)
        {
            return rotation(angle, { T(0), T(1), T(0) });
        }

        template<typename F> requires isIntegral<F>
        static Mat rotationZ(F angle)
        {
            return rotation(angle, { T(0), T(0), T(1) });
        }

        template<typename F> requires isIntegral<F>
        static Mat translation(const Vec<3, F>& v)
        {
            Mat result = identity();
            result.mCols[3][0] = v[0];
            result.mCols[3][1] = v[1];
            result.mCols[3][2] = v[2];

            return result;
        }

        template<typename F> requires isIntegral<F>
        static Mat translation(const Vec<2, F>& v)
        {
            return translation(Vec<3, F>{ v.x, v.y, F(0) });
        }

        template<typename F> requires isIntegral<F>
        static Mat translation(F x, F y, F z)
        {
            return translation(Vec<3, F>{ x, y, z });
        }

        template<typename F> requires isIntegral<F>
        static Mat translation(F x, F y)
        {
            return translation(Vec<3, F>{ x, y, F(0) });
        }

        union
        {
            struct
            {
                T m11, m21, m31, m41;       // col 1
                T m12, m22, m32, m42;       // col 2
                T m13, m23, m33, m43;       // col 3
                T m14, m24, m34, m44;       // col 4
            };

            StaticArray<T, 16> mData;
            StaticArray<Vec<4, T>, 4> mCols;
        };
    };

    template<typename T, typename F>
    Mat<4, 4, T> operator*(F value, const Mat<4, 4, T>& mat) requires isIntegral<F>
    {
        return {
            value * mat.m11, value * mat.m12, value * mat.m13, value * mat.m14,
            value * mat.m21, value * mat.m22, value * mat.m23, value * mat.m24,
            value * mat.m31, value * mat.m32, value * mat.m33, value * mat.m34,
            value * mat.m41, value * mat.m42, value * mat.m43, value * mat.m44,
        };
    }

    template<typename T, typename F>
    Mat<4, 4, T> operator*(const Mat<4, 4, T>& mat, F value) requires isIntegral<F>
    {
        return value * mat;
    }

    template<typename T, typename F>
    Mat<4, 4, T> operator+(const Mat<4, 4, T>& a, const Mat<4, 4, F>& b)
    {
        return {
            a.m11 + b.m11, a.m12 + b.m12, a.m13 + b.m13, a.m14 + b.m14,
            a.m21 + b.m21, a.m22 + b.m22, a.m23 + b.m23, a.m24 + b.m24,
            a.m31 + b.m31, a.m32 + b.m32, a.m33 + b.m33, a.m34 + b.m34,
            a.m41 + b.m41, a.m42 + b.m42, a.m43 + b.m43, a.m44 + b.m44,
        };
    }

    template<typename T, typename F>
    Mat<4, 4, T> operator*(const Mat<4, 4, T>& a, const Mat<4, 4, F>& b)
    {
        Vec<4, T> srcA0 = a.mCols[0];
        Vec<4, T> srcA1 = a.mCols[1];
        Vec<4, T> srcA2 = a.mCols[2];
        Vec<4, T> srcA3 = a.mCols[3];

        Vec<4, T> srcB0 = b.mCols[0];
        Vec<4, T> srcB1 = b.mCols[1];
        Vec<4, T> srcB2 = b.mCols[2];
        Vec<4, T> srcB3 = b.mCols[3];

        Mat<4, 4, T> result;
        result.mCols[0][0] = srcA0[0] * srcB0[0] + srcA1[0] * srcB0[1] + srcA2[0] * srcB0[2] + srcA3[0] * srcB0[3];
        result.mCols[0][1] = srcA0[1] * srcB0[0] + srcA1[1] * srcB0[1] + srcA2[1] * srcB0[2] + srcA3[1] * srcB0[3];
        result.mCols[0][2] = srcA0[2] * srcB0[0] + srcA1[2] * srcB0[1] + srcA2[2] * srcB0[2] + srcA3[2] * srcB0[3];
        result.mCols[0][3] = srcA0[3] * srcB0[0] + srcA1[3] * srcB0[1] + srcA2[3] * srcB0[2] + srcA3[3] * srcB0[3];

        result.mCols[1][0] = srcA0[0] * srcB1[0] + srcA1[0] * srcB1[1] + srcA2[0] * srcB1[2] + srcA3[0] * srcB1[3];
        result.mCols[1][1] = srcA0[1] * srcB1[0] + srcA1[1] * srcB1[1] + srcA2[1] * srcB1[2] + srcA3[1] * srcB1[3];
        result.mCols[1][2] = srcA0[2] * srcB1[0] + srcA1[2] * srcB1[1] + srcA2[2] * srcB1[2] + srcA3[2] * srcB1[3];
        result.mCols[1][3] = srcA0[3] * srcB1[0] + srcA1[3] * srcB1[1] + srcA2[3] * srcB1[2] + srcA3[3] * srcB1[3];

        result.mCols[2][0] = srcA0[0] * srcB2[0] + srcA1[0] * srcB2[1] + srcA2[0] * srcB2[2] + srcA3[0] * srcB2[3];
        result.mCols[2][1] = srcA0[1] * srcB2[0] + srcA1[1] * srcB2[1] + srcA2[1] * srcB2[2] + srcA3[1] * srcB2[3];
        result.mCols[2][2] = srcA0[2] * srcB2[0] + srcA1[2] * srcB2[1] + srcA2[2] * srcB2[2] + srcA3[2] * srcB2[3];
        result.mCols[2][3] = srcA0[3] * srcB2[0] + srcA1[3] * srcB2[1] + srcA2[3] * srcB2[2] + srcA3[3] * srcB2[3];

        result.mCols[3][0] = srcA0[0] * srcB3[0] + srcA1[0] * srcB3[1] + srcA2[0] * srcB3[2] + srcA3[0] * srcB3[3];
        result.mCols[3][1] = srcA0[1] * srcB3[0] + srcA1[1] * srcB3[1] + srcA2[1] * srcB3[2] + srcA3[1] * srcB3[3];
        result.mCols[3][2] = srcA0[2] * srcB3[0] + srcA1[2] * srcB3[1] + srcA2[2] * srcB3[2] + srcA3[2] * srcB3[3];
        result.mCols[3][3] = srcA0[3] * srcB3[0] + srcA1[3] * srcB3[1] + srcA2[3] * srcB3[2] + srcA3[3] * srcB3[3];

        return result;
    }

    template<typename T, typename F>
    Vec<4, T> operator*(const Mat<4, 4, T>& mat, const Vec<4, F>& vec)
    {
        return {
            mat.mCols[0][0] * vec.x + mat.mCols[1][0] * vec.y + mat.mCols[2][0] * vec.z + mat.mCols[3][0] * vec.w,
            mat.mCols[0][1] * vec.x + mat.mCols[1][1] * vec.y + mat.mCols[2][1] * vec.z + mat.mCols[3][1] * vec.w,
            mat.mCols[0][2] * vec.x + mat.mCols[1][2] * vec.y + mat.mCols[2][2] * vec.z + mat.mCols[3][2] * vec.w,
            mat.mCols[0][3] * vec.x + mat.mCols[1][3] * vec.y + mat.mCols[2][3] * vec.z + mat.mCols[3][3] * vec.w,
        };
    }

    template<typename T, typename F>
    Vec<4, T> operator*(const Vec<4, F>& vec, const Mat<4, 4, T>& mat)
    {
        return {
            mat.mCols[0][0] * vec.x + mat.mCols[0][1] * vec.y + mat.mCols[0][2] * vec.z + mat.mCols[0][3] * vec.w,
            mat.mCols[1][0] * vec.x + mat.mCols[1][1] * vec.y + mat.mCols[1][2] * vec.z + mat.mCols[1][3] * vec.w,
            mat.mCols[2][0] * vec.x + mat.mCols[2][1] * vec.y + mat.mCols[2][2] * vec.z + mat.mCols[2][3] * vec.w,
            mat.mCols[3][0] * vec.x + mat.mCols[3][1] * vec.y + mat.mCols[3][2] * vec.z + mat.mCols[3][3] * vec.w,
        };
    }

    template<typename T, typename F>
    bool8 operator==(const Mat<4, 4, T>& a, const Mat<4, 4, F>& b)
    {
        return a.mCols[0] == b.mCols[0] && a.mCols[1] == b.mCols[1] && a.mCols[2] == b.mCols[2] && a.mCols[3] == b.mCols[3];
    }

    template<typename T, typename F>
    bool8 operator!=(const Mat<4, 4, T>& a, const Mat<4, 4, F>& b)
    {
        return !(a == b);
    }

} // namespace otto
