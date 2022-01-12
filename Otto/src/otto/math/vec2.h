#pragma once

#include "otto/base.h"

namespace otto
{
    template<typename T>
    class Vec2
    {
    public:

        template<typename T>
        Vec2<T> operator+=(T value)
        {
            mX += value;
            mY += value;
            return *this;
        }

        template<typename T>
        Vec2<T> operator-=(T value)
        {
            mX -= value;
            mY -= value;
            return *this;
        }

        template<typename T>
        Vec2<T> operator+=(const Vec2<T>& value)
        {
            mX += value.x;
            mY += value.y;
            return *this;
        }

        template<typename T>
        Vec2<T> operator-=(const Vec2<T>& value)
        {
            mX -= value.x;
            mY -= value.y;
            return *this;
        }

        template<typename T>
        Vec2<T> operator*=(T value)
        {
            mX *= value.x;
            mY *= value.y;
            return *this;
        }

        T getX() const
        {
            return mX;
        }

        T getY() const
        {
            return mY;
        }

        void setX(T x)
        {
            mX = x;
        }

        void setY(T y)
        {
            mY = y;
        }

        T dot(const Vec2& other) const
        {
            return mX * other.mX + mY * other.mY;
        }

        T length() const
        {
            return dot(*this);
        }

    private:
        union
        {
            T mX;
        };

        union
        {
            T mY;
        };
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
}
