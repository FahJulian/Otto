#pragma once

#include <iostream>

#include "otto/math/vec2.h"

namespace otto
{
    template<typename T>
    class Mat2x2
    {
    public:
        constexpr Mat2x2()
            : Mat2x2(T(0))
        {
        }

        constexpr Mat2x2(T value)
            : m11(value), m22(value)
        {
        }

        constexpr Mat2x2(T m11, T m12, T m21, T m22)
            : m11(m11), m12(m12), m21(m21), m22(m22)
        {
        }

        Mat2x2& operator+=(const Mat2x2& other)
        {
            m11 += other.m11;
            m12 += other.m12;
            m21 += other.m21;
            m22 += other.m22;
        }

        Mat2x2& operator*=(const Mat2x2& other)
        {
            m11 = m11 * other.m11 + m12 * other.m21;
            m12 = m11 * other.m12 + m12 * other.m22;
            m21 = m21 * other.m11 + m22 * other.m21;
            m22 = m21 * other.m12 + m22 * other.m22;

            return *this;
        }

        T det()
        {
            return m11 * m22 - m21 * m12;
        }

        constexpr static Mat2x2 identity()
        {
            return {
                T(1), T(0),
                T(0), T(1)
            };
        }

        constexpr static Mat2x2 zero()
        {
            return {
                T(0), T(0),
                T(0), T(0)
            };
        }

    private:
        T m11 = T(), m12 = T();
        T m21 = T(), m22 = T();
    };

    using Mat2x2i8 = Mat2x2<int8>;
    using Mat2x2i16 = Mat2x2<int16>;
    using Mat2x2i32 = Mat2x2<int32>;
    using Mat2x2i64 = Mat2x2<int64>;
    using Mat2x2ui8 = Mat2x2<uint8>;
    using Mat2x2ui16 = Mat2x2<uint16>;
    using Mat2x2ui32 = Mat2x2<uint32>;
    using Mat2x2ui64 = Mat2x2<uint64>;
    using Mat2x2f32 = Mat2x2<float32>;
    using Mat2x2f64 = Mat2x2<float64>;

    template<typename T>
    Vec2<T> operator*(const Mat2x2<T>& mat, const Vec2<T>& vec)
    {
        return { 
            mat.m11 * vec.mX + mat.m12 * vec.mY,
            mat.m21 * vec.mX + mat.m22 * vec.mY
        };
    }

    template<typename T>
    Mat2x2<T> operator*(const Mat2x2<T>& mat1, const Mat2x2<T>& mat2)
    {
        return {
            mat1.m11 * mat2.m11 + mat1.m12 * mat2.m21,
            mat1.m11 * mat2.m12 + mat1.m12 * mat2.m22,
            mat1.m21 * mat2.m11 + mat1.m22 * mat2.m21,
            mat1.m21 * mat2.m12 + mat1.m22 * mat2.m22
        };
    }

    template <typename T>
    std::ostream& operator<<(std::ostream& stream, const Mat2x2<T>& mat)
    {
        return (stream << mat.m11 << ' ' << mat.m12 << std::endl << mat.m21 << ' ' << mat.m22);
    }
}
