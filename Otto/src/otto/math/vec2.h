#pragma once

#include <iostream>

#include "otto/base.h"

namespace otto
{
    template<typename T>
    struct Vec2
    {
        constexpr Vec2()
            : Vec2(T(0))
        {
        }

        constexpr Vec2(T t)
            : x(t), y(t)
        {
        }

        constexpr Vec2(T x, T y)
            : x(x), y(y)
        {
        }

        Vec2(const Vec2& other) = default;

        Vec2& operator=(const Vec2& other) = default;

        template<typename F>
        Vec2& operator+=(const Vec2<F>& v)
        {
            x += v.x;
            y += v.y;
        }

        template<typename F>
        Vec2& operator+=(F f) requires isIntegral<F>
        {
            x += f;
            y += f;
        }

        template<typename F>
        Vec2& operator*=(F f) requires isIntegral<F>
        {
            x *= f;
            y *= f;
        }

        template<typename F, typename G>
        friend Vec2 operator+(const Vec2<F>& v1, const Vec2<G>& v2)
        {
            return { v1.x + v2.x, v1.y + v2.y };
        }

        template<typename F>
        friend typename Vec2 operator+(F f, const Vec2& v) requires isIntegral<F>
        {
            return { f + v.x, f + v.y };
        }

        template<typename F>
        friend typename Vec2 operator+(const Vec2& v, F f) requires isIntegral<F>
        {
            return { v.x + f, v.y + f };
        }

        template<typename F>
        friend typename Vec2 operator*(F f, const Vec2& v) requires isIntegral<F>
        {
            return { f * v.x, f * v.y };
        }

        template<typename F>
        friend typename Vec2 operator*(const Vec2& v, F f) requires isIntegral<F>
        {
            return { v.x * f, v.y * f };
        }

        friend std::ostream& operator<<(std::ostream& stream, const Vec2& v)
        {
            return (stream << '(' << v.x << ", " << v.y << ')');
        }

        T x = T();
        T y = T();
    };

    using Vec2f32 = Vec2<float32>;
    using Vec2f64 = Vec2<float64>;
    using Vec2ui8 = Vec2<uint8>;
    using Vec2ui16 = Vec2<uint16>;
    using Vec2ui32 = Vec2<uint32>;
    using Vec2ui64 = Vec2<uint64>;
    using Vec2i8 = Vec2<int8>;
    using Vec2i16 = Vec2<int16>;
    using Vec2i32 = Vec2<int32>;
    using Vec2i64 = Vec2<int64>;

} // namespace otto
