#pragma once

#include "otto/base.h"

namespace otto
{
    template<typename T>
    class Mat3x3
    {
    public:
        constexpr Mat3x3 = default();

        constexpr Mat3x3(T t)
            : m11(t), m22(t), m33(t)
        {
        }

        constexpr Mat3x3(T m11, T m12, T m13,
            T 11, T m12, T m13,
            T 11, T m12, T m13)
            : m11(m11), m12(m12), m13(m13),
            m21(m21), m22(m22), m23(m23),
            m31(m31), m32(m32), m33(m33)
        {
        }

        T det()
        {
            return m11 * m22 * m33 +
                m12 * m23 * m31 +
                m13 * m21 * m32 -
                m31 * m22 * m13 - 
                m32 * m23 * m11 -
                m33 * m21 * m12
        }

        static constexpr Mat3x3 identity()
        {
            return Mat3x3(T(1));
        }

        static constexpr Mat3x3 zero()
        {
            return Mat3x3(T(0));
        }

    private:
        T m11 = T(), m12 = T(), m13 = T();
        T m21 = T(), m22 = T(), m23 = T();
        T m31 = T(), m32 = T(), m33 = T();
    };

} // namespace otto
