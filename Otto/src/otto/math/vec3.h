#pragma once

#include <iostream>

#include "otto/base.h"

namespace otto
{
    template<typename T>
    struct Vec3
    {
        constexpr Vec3() = default;

        constexpr Vec3(T t)
            : mX(t), mY(t), mZ(t)
        {
        }

        constexpr Vec3(T x, T y, T z)
            : mX(x), mY(y), mZ(z)
        {
        }

        Vec3(const Vec3& other) = default;

        Vec3& operator=(const Vec3& other) = default;

        template<typename F>
        Vec3& operator+=(const Vec3<F>& v)
        {
            mX += v.mX;
            mY += v.mY;
            mZ += v.mZ;
        }

        template<typename F>
        typename std::enable_if<std::is_integral<F>::value, Vec3&>::type operator+=(F f)
        {
            mX += f;
            mY += f;
            mZ += f;
        }

        template<typename F>
        typename std::enable_if<std::is_integral<F>::value, Vec3&>::type operator*=(F f)
        {
            mX *= f;
            mY *= f;
            mZ *= f;
        }

        template<typename F, typename G>
        friend Vec3 operator+(const Vec3<F>& v1, const Vec3<G>& v2)
        {
            return { v1.mX + v2.mX, v1.mY + v2.mY, v1.mZ + v2.mZ };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec3>::type operator+(F f, const Vec3& v)
        {
            return { f + v.mX, f + v.mY, f + v.mZ };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec3>::type operator+(const Vec3& v, F f)
        {
            return { v.mX + f, v.mY + f, v.mZ + f };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec3>::type operator*(F f, const Vec3& v)
        {
            return { f * v.mX, f * v.mY, f * v.mZ };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral<F>::value, Vec3>::type operator*(const Vec3& v, F f)
        {
            return { v.mX * f, v.mY * f, v.mZ * f };
        }

        friend std::ostream& operator<<(std::ostream& stream, const Vec3& v)
        {
            return (stream << '(' << v.mX << ", " << v.mY << ", " << v.mZ << ')');
        }

    private:
        T mX = T();
        T mY = T();
        T mZ = T();
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
