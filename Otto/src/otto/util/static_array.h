#pragma once

#include <utility>

#include "otto/base.h"

namespace otto
{
    template<typename T, uint64 N>
    class StaticArray
    {
    public:
        StaticArray() = default;

        StaticArray(const std::initializer_list<T>& data)
        {
            OTTO_ASSERT(data.size() <= N, "To many values for StaticArray");

            std::move(data.begin(), data.end(), mData);
        }

        T& operator[](uint64 index) { return get(index); }
        const T& operator[](uint64 index) const { return get(index); }

        T* begin()
        {
            return mData;
        }

        T* end()
        {
            return mData + N;
        }

        const T* begin() const
        {
            return mData;
        }

        const T* end() const
        {
            return mData + N;
        }

        template<typename F>
        void forEach(F Function)
        {
            for (uint64 i = 0; i < N; i++)
                Function(mData[i]);
        }

        T& get(uint64 index)
        {
            OTTO_ASSERT(index < N, "Array index out of range");

            return mData[index];
        }

        const T& get(uint64 index) const
        {
            OTTO_ASSERT(index < N, "Array index out of range");

            return mData[index];
        }

        uint64 getSize() const
        {
            return N;
        }

        T* getData()
        {
            return mData;
        }

        const T* getData() const
        {
            return mData;
        }

    private:
        T mData[N];
    };

}
