#pragma once

#include <iostream>

#include "otto/base.h"

namespace otto
{
    template<typename T>
    class Vec4
    {
    public:
        constexpr Vec4() = default;

        constexpr Vec4(T t)
            : mX(t), mY(t), mZ(t), mW(t)
        {
        }
        
        constexpr Vec4(T x, T y, T z, T w)
            : mX(x), mY(y), mZ(z), mW(w)
        {
        }

        Vec4(const Vec4& other) = default;

        Vec4& operator=(const Vec4& other) = default;

        template<typename F>
        Vec4& operator+=(const Vec4<F>& v)
        {
            mX += v.mX;
            mY += v.mY;
            mZ += v.mZ;
            mW += v.mW;
        }

        template<typename F>
        typename std::enable_if<std::is_integral<F>::value, Vec4&>::type operator+=(F f)
        {
            mX += f;
            mY += f;
            mZ += f;
            mW += f;
        }

        template<typename F>
        typename std::enable_if<std::is_integral<F>::value, Vec4&>::type operator*=(F f)
        {
            mX *= f;
            mY *= f;
            mZ *= f;
            mW *= f;
        }
        
        template<typename F, typename G>
        friend Vec4 operator+(const Vec4<F>& v1, const Vec4<G>& v2)
        {
            return { v1.mX + v2.mX, v1.mY + v2.mY, v1.mZ + v2.mZ, v1.mW + v2.mW };
        }

        T getX() const requires std::is_integral_v<T>
        {
            return mX;
        }

        const T& getX() const
        {
            return mX;
        }

        T getY() const requires std::is_integral_v<T>
        {
            return mY;
        }

        const T& getY() const
        {
            return mY;
        }

        T getZ() const requires std::is_integral_v<T>
        {
            return mZ;
        }

        const T& getZ() const
        {
            return mZ;
        }

        T getW() const requires std::is_integral_v<T>
        {
            return mW;
        }

        const T& getW() const
        {
            return mW;
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral_v<F>, Vec4>::type operator+(F f, const Vec4& v)
        {
            return { f + v.mX, f + v.mY, f + v.mZ, f + v.mW };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral_v<F>, Vec4>::type operator+(const Vec4& v, F f)
        {
            return { v.mX + f, v.mY + f, v.mZ + f, v.mW + f };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral_v<F>, Vec4>::type operator*(F f, const Vec4& v)
        {
            return { f * v.mX, f * v.mY, f * v.mZ, f * v.mW };
        }

        template<typename F>
        friend typename std::enable_if<std::is_integral_v<F>, Vec4>::type operator*(const Vec4& v, F f)
        {
            return { v.mX * f, v.mY * f, v.mZ * f, v.mW * f };
        }

        friend std::ostream& operator<<(std::ostream& stream, const Vec4& v)
        {
            return (stream << '(' << v.mX << ", " << v.mY << ", " << v.mZ << ", " << v.mW << ')');
        }
    
    private:
        T mX = T();
        T mY = T();
        T mZ = T();
        T mW = T();
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
