#pragma once

#include <limits>
#include <algorithm>
#include <initializer_list>

#include "otto/base.h"

namespace otto {

    template<typename T>
    class DynamicArray
    {
    private:
        static constexpr double CAPACITY_MULTIPLIER_FACTOR = 1.15;

    public:
        constexpr DynamicArray()
            : mSize(0), mCapacity(0), mData(nullptr)
        {
        }

        DynamicArray(uint64 capacity)
            : mSize(0), mCapacity(capacity), mData(_allocateMemory(capacity))
        {
        }

        DynamicArray(T* data, uint64 elementCount)
            : mSize(elementCount), mCapacity(elementCount), mData(_allocateMemory(mCapacity))
        {
            for (uint64 i = 0; i < elementCount; i++)
                new(mData + i) T(std::move(data[i]));
        }

        DynamicArray(const std::initializer_list<T>& values)
            : mSize(values.size()), mCapacity(values.size()), mData(_allocateMemory(mCapacity))
        {
            for (uint64 i = 0, size = values.size(); i < size; i++)
                new(mData + i) T(std::move(*(values.begin() + i)));
        }

        DynamicArray(const DynamicArray& other)
            : mSize(0), mCapacity(0), mData(nullptr)
        {
            *this = other;
        }

        DynamicArray(DynamicArray&& other) noexcept
            : mSize(0), mCapacity(0), mData(nullptr)
        {
            *this = std::move(other);
        }

        ~DynamicArray()
        {
            for (auto& value : *this)
                value.~T();

            _freeMemory(mData);
        }

        DynamicArray& operator=(const DynamicArray& other)
        {
            if (this != &other)
            {
                for (auto& value : *this)
                    value.~T();

                _freeMemory(mData);

                mCapacity = other.mCapacity;
                mSize = other.mSize;
                mCursor = other.mCursor;
                mData = _allocateMemory(mCapacity);

                for (uint64 i = 0; i < other.mSize; i++)
                    new(mData + i) T(other.get(i));
            }

            return *this;
        }

        DynamicArray& operator=(DynamicArray&& other) noexcept
        {
            if (this != &other)
            {
                for (auto& value : *this)
                    value.~T();

                _freeMemory(mData);

                mCapacity = other.mCapacity;
                mSize = other.mSize;
                mCursor = other.mCursor;
                mData = other.mData;

                other.mCapacity = 0;
                other.mSize = 0;
                other.mData = nullptr;
                other.mCursor = 0;
            }

            return *this;
        }

        template<typename... Args>
        void operator+=(Args&&... args)
        {
            add(std::forward<Args>(args)...);
        }

        void operator+=(const T& value)
        {
            add(value);
        }

        T& operator[](uint64 index)
        {
            OTTO_ASSERT(index < mSize, "Index out of range");

            return mData[index];
        }

        const T& operator[](uint64 index) const
        {
            OTTO_ASSERT(index < mSize, "Index out of range");

            return mData[index];
        }

        template<typename... Args>
        void add(Args&&... args)
        {
            if (mSize == mCapacity)
                setCapacity(_calculateNewCapacity());

            new(mData + mSize) T(std::forward<Args>(args)...);

            mSize++;
        }

        void add(const T& value)
        {
            if (mSize == mCapacity)
                setCapacity(_calculateNewCapacity());

            new(mData + mSize) T(value);

            mSize++;
        }

        template<typename... Args>
        void insert(uint64 index, Args&&... args)
        {
            OTTO_ASSERT(index <= mSize, "Cannot insert after end of DynamicArray");

            if (mSize == mCapacity)
            {
                mCapacity = _calculateNewCapacity();

                T* newData = _allocateMemory(mCapacity);
                for (uint64 i = 0; i < index; i++)
                {
                    new(newData + i) T(std::move(mData[i]));
                    mData[i].~T();
                }

                new(newData + index) T(std::forward<Args>(args)...);

                for (uint64 i = index; i < mSize; i++)
                {
                    new(newData + i + 1) T(std::move(mData[i]));
                    mData[i].~T();
                }

                _freeMemory(mData);
                mData = newData;
            }
            else
            {
                new (mData + mSize) T(std::move(mData[mSize - 1]));

                for (uint64 i = mSize - 1; i > index; i--)
                    mData[i] = std::move(mData[i - 1]);

                mData[index].~T();
                new(mData + index) T(std::forward<Args>(args)...);
            }

            mSize++;
        }

        template<typename F>
        uint64 indexOf(const F& value) const
        {
            for (uint64 i = 0; i < mSize; i++)
            {
                if (mCursor == mSize)
                    mCursor = 0;

                if (mData[mCursor] == value)
                    return mCursor;

                mCursor++;
            }

            return mSize;
        }

        template<typename F>
        bool contains(const F& value) const
        {
            return indexOf(value) != mSize;
        }

        void remove(uint64 index)
        {
            OTTO_ASSERT(index < mSize, "Index out of range");

            for (uint64 i = index; i < mSize - 1; i++)
                mData[i] = std::move(mData[i + 1]);

            mData[mSize - 1].~T();

            mSize--;
        }

        template<typename F>
        void forEach(F Function)
        {
            for (uint64 i = 0; i < mSize; i++)
                Function(mData[i]);
        }

        void setSize(uint64 size)
        {
            if (size > mSize)
            {
                if (size > mCapacity)
                    setCapacity(size);

                if (size > mSize)
                {
                    for (uint64 i = mSize; i < size; i++)
                        new(mData + i) T();
                }
            }
            else
            {
                for (uint64 i = size; i < mSize; i++)
                    mData[i].~T();
            }

            mSize = size;
        }

        void setCapacity(uint64 capacity)
        {
            if (capacity < mSize)
            {
                for (uint64 i = capacity; i < mSize; i++)
                    mData[i].~T();

                mSize = capacity;
            }

            mCapacity = capacity;

            T* newData = _allocateMemory(mCapacity);
            for (uint64 i = 0; i < mSize; i++)
            {
                new(newData + i) T(std::move(mData[i]));
                mData[i].~T();
            }

            _freeMemory(mData);
            mData = newData;
        }

        void shrink()
        {
            setCapacity(mSize);
        }

        void clear()
        {
            for (auto& value : *this)
                value.~T();

            mSize = 0;
        }

        T* begin()
        {
            return mData;
        }

        T* end()
        {
            return mData + mSize;
        }

        const T* begin() const
        {
            return mData;
        }

        const T* end() const
        {
            return mData + mSize;
        }

        T* getData()
        {
            return mData;
        }

        const T* getData() const
        {
            return mData;
        }

        uint64 getSize() const
        {
            return mSize;
        }

        uint64 getCapacity() const
        {
            return mCapacity;
        }

        T& get(uint64 index)
        {
            OTTO_ASSERT(index < mSize, "Index out of range");

            return mData[index];
        }

        const T& get(uint64 index) const
        {
            OTTO_ASSERT(index < mSize, "Index out of range");

            return mData[index];
        }

    private:
        static T* _allocateMemory(uint64 capacity)
        {
            return reinterpret_cast<T*>(operator new(capacity * sizeof(T)));
        }

        static void _freeMemory(T* data)
        {
            operator delete(data);
        }

        uint64 _calculateNewCapacity()
        {
            return static_cast<uint64>(static_cast<double>(mCapacity) * CAPACITY_MULTIPLIER_FACTOR + 1.0);
        }

        uint64 mSize;
        uint64 mCapacity;
        T* mData;

        mutable uint64 mCursor = 0;
    };

} // namespace otto
