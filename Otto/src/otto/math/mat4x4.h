#pragma once

#include "otto/base.h"
#include "otto/math/mat3x3.h"

namespace otto
{
    template<typename T>
    class Mat4x4
    {
    public:
        constexpr Mat4x4()
            : Mat4x4(T(0))
        {
        }

        constexpr Mat4x4(T t)
            : m11(t), m22(t), m33(t), m44(t)
        {
        }

        constexpr Mat4x4(T m11, T m12, T m13, T m14,
                T m21, T m22, T m23, T m24,
                T m31, T m32, T m33, T m34,
                T m41, T m42, T m43, T m44)
            : m11(m11), m12(m12), m13(m13), m14(m14),
            m21(m21), m22(m22), m23(m23), m24(m24),
            m31(m31), m32(m32), m33(m33), m34(m34),
            m41(m41), m42(m42), m43(m43), m44(m44)
        {
        }

        const T* getData()
        {
            return &m11;
        }

        T det()
        {
            return m11 * Mat3x3<T>(m22, m23, m24,
                                   m32, m33, m34,
                                   m42, m43, m44).det() -
                m12 * Mat3x3<T>(m21, m23, m24,
                                m31, m33, m34,
                                m41, m43, m44).det() + 
                m13 * Mat3x3<T>(m21, m22, m24,
                                m31, m32, m34,
                                m41, m42, m44).det() -
                m14 * Mat3x3<T>(m21, m22, m23,
                                m31, m32, m33,
                                m41, m42, m43).det();
        }

        static constexpr Mat4x4 identity()
        {
            return Mat4x4(T(1));
        }

        static constexpr Mat4x4 zero()
        {
            return Mat4x4(T(0));
        }

    private:
        T m11 = T(), m12 = T(), m13 = T(), m14 = T();
        T m21 = T(), m22 = T(), m23 = T(), m24 = T();
        T m31 = T(), m32 = T(), m33 = T(), m34 = T();
        T m41 = T(), m42 = T(), m43 = T(), m44 = T();
    };

    using Mat4x4f32 = Mat4x4<float32>;
    using Mat4x4f64 = Mat4x4<float64>;
    using Mat4x4i8 = Mat4x4<uint8>;
    using Mat4x4i16 = Mat4x4<uint16>;
    using Mat4x4i32 = Mat4x4<uint32>;
    using Mat4x4i64 = Mat4x4<uint64>;
    using Mat4x4ui8 = Mat4x4<int8>;
    using Mat4x4ui16 = Mat4x4<int16>;
    using Mat4x4ui32 = Mat4x4<int32>;
    using Mat4x4ui64 = Mat4x4<int64>;

} // namespace otto
