#pragma once

#include <iostream>

#include "otto/base.h"

namespace otto
{
    template<typename T>
    struct Vec3
    {
        constexpr Vec3()
            : Vec3(T(0))
        {
        }

        constexpr Vec3(T t)
            : x(t), y(t), z(t)
        {
        }

        constexpr Vec3(T x, T y, T z)
            : x(x), y(y), z(z)
        {
        }

        Vec3(const Vec3& other) = default;

        Vec3& operator=(const Vec3& other) = default;

        template<typename F>
        Vec3& operator+=(const Vec3<F>& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
        }

        template<typename F>
        typename std::enable_if<std::is_integral<F>::value, Vec3&>::type operator+=(F f)
        {
            x += f;
            y += f;
            z += f;
        }

        template<typename F>
        typename std::enable_if<std::is_integral<F>::value, Vec3&>::type operator*=(F f)
        {
            x *= f;
            y *= f;
            z *= f;
        }

        template<typename F, typename G>
        friend Vec3 operator+(const Vec3<F>& v1, const Vec3<G>& v2)
        {
            return { v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec3>::type operator+(F f, const Vec3& v)
        {
            return { f + v.x, f + v.y, f + v.z };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec3>::type operator+(const Vec3& v, F f)
        {
            return { v.x + f, v.y + f, v.z + f };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec3>::type operator*(F f, const Vec3& v)
        {
            return { f * v.x, f * v.y, f * v.z };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec3>::type operator*(const Vec3& v, F f)
        {
            return { v.x * f, v.y * f, v.z * f };
        }

        friend std::ostream& operator<<(std::ostream& stream, const Vec3& v)
        {
            return (stream << '(' << v.x << ", " << v.y << ", " << v.z << ')');
        }

        T x = T();
        T y = T();
        T z = T();
    };

    using Vec3f32 = Vec3<float32>;
    using Vec3f64 = Vec3<float64>;
    using Vec3ui8 = Vec3<uint8>;
    using Vec3ui16 = Vec3<uint16>;
    using Vec3ui32 = Vec3<uint32>;
    using Vec3ui64 = Vec3<uint64>;
    using Vec3i8 = Vec3<int8>;
    using Vec3i16 = Vec3<int16>;
    using Vec3i32 = Vec3<int32>;
    using Vec3i64 = Vec3<int64>;

} // namespace otto
