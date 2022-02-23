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
            : mX(t), mY(t)
        {
        }

        constexpr Vec2(T x, T y)
            : mX(x), mY(y)
        {
        }

        Vec2(const Vec2& other) = default;

        Vec2& operator=(const Vec2& other) = default;

        template<typename F>
        Vec2& operator+=(const Vec2<F>& v)
        {
            mX += v.mX;
            mY += v.mY;
        }

        template<typename F>
        typename std::enable_if<std::is_integral<F>::value, Vec2&>::type operator+=(F f)
        {
            mX += f;
            mY += f;
        }

        template<typename F>
        typename std::enable_if<std::is_integral<F>::value, Vec2&>::type operator*=(F f)
        {
            mX *= f;
            mY *= f;
        }

        template<typename F, typename G>
        friend Vec2 operator+(const Vec2<F>& v1, const Vec2<G>& v2)
        {
            return { v1.mX + v2.mX, v1.mY + v2.mY };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec2>::type operator+(F f, const Vec2& v)
        {
            return { f + v.mX, f + v.mY };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec2>::type operator+(const Vec2& v, F f)
        {
            return { v.mX + f, v.mY + f };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec2>::type operator*(F f, const Vec2& v)
        {
            return { f * v.mX, f * v.mY };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec2>::type operator*(const Vec2& v, F f)
        {
            return { v.mX * f, v.mY * f };
        }

        friend std::ostream& operator<<(std::ostream& stream, const Vec2& v)
        {
            return (stream << '(' << v.mX << ", " << v.mY << ')');
        }

    private:
        T mX = T();
        T mY = T();
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
