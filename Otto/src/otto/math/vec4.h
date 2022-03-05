#pragma once

#include <iostream>

#include "otto/base.h"

namespace otto
{
    template<typename T>
    class Vec4
    {
    public:
        constexpr Vec4()
            : Vec4(T(0))
        {
        }

        constexpr Vec4(T t)
            : x(t), y(t), z(t), w(t)
        {
        }
        
        constexpr Vec4(T x, T y, T z, T w)
            : x(x), y(y), z(z), w(w)
        {
        }

        Vec4(const Vec4& other) = default;

        Vec4& operator=(const Vec4& other) = default;

        template<typename F>
        Vec4& operator+=(const Vec4<F>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
        }

        template<typename F>
        typename std::enable_if<std::is_integral<F>::value, Vec4&>::type operator+=(F f)
        {
            x += f;
            y += f;
            z += f;
            w += f;
        }

        template<typename F>
        typename std::enable_if<std::is_integral<F>::value, Vec4&>::type operator*=(F f)
        {
            x *= f;
            y *= f;
            z *= f;
            w *= f;
        }
        
        template<typename F, typename G>
        friend Vec4 operator+(const Vec4<F>& v1, const Vec4<G>& v2)
        {
            return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral_v<F>, Vec4>::type operator+(F f, const Vec4& v)
        {
            return { f + v.x, f + v.y, f + v.z, f + v.w };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral_v<F>, Vec4>::type operator+(const Vec4& v, F f)
        {
            return { v.x + f, v.y + f, v.z + f, v.w + f };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral_v<F>, Vec4>::type operator*(F f, const Vec4& v)
        {
            return { f * v.x, f * v.y, f * v.z, f * v.w };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral_v<F>, Vec4>::type operator*(const Vec4& v, F f)
        {
            return { v.x * f, v.y * f, v.z * f, v.w * f };
        }

        friend std::ostream& operator<<(std::ostream& stream, const Vec4& v)
        {
            return (stream << '(' << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ')');
        }
    
        T x = T();
        T y = T();
        T z = T();
        T w = T();
    };

    using Vec4f32 = Vec4<float32>;
    using Vec4f64 = Vec4<float64>;
    using Vec4ui8 = Vec4<uint8>;
    using Vec4ui16 = Vec4<uint16>;
    using Vec4ui32 = Vec4<uint32>;
    using Vec4ui64 = Vec4<uint64>;
    using Vec4i8 = Vec4<int8>;
    using Vec4i16 = Vec4<int16>;
    using Vec4i32 = Vec4<int32>;
    using Vec4i64 = Vec4<int64>;
                           
} // namespace otto
