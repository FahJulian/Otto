#pragma once

#include "otto/util/dynamic_array.h"
#include "otto/util/pair.h"
#include "otto/base.h"

namespace otto {

    template<typename K, typename V>
    class Map
    {
    public:
        constexpr Map() = default;

        Map(uint64 capacity)
            : mData(capacity)
        {
        }

        Map(const std::initializer_list<Pair<K, V>> values)
            : mData(values)
        {
        }

        Map(const Map& other)
            : mData(other.mData)
        {
        }

        Map(Map&& other) noexcept
            : mData(std::move(other.mData))
        {
        }

        ~Map() = default;

        Map& operator=(const Map& other)
        {
            mData = other.mData;

            return *this;
        }

        Map& operator=(Map&& other) noexcept
        {
            mData = std::move(other.mData);

            return *this;
        }

        template<typename T>
        V& operator[](const T& key)
        {
            return mData.get(mData.indexOf(key)).mSecond;
        }

        template<typename T>
        const V& operator[](const T& key) const
        {
            return mData.get(mData.indexOf(key)).mSecond;
        }

        template<typename T, typename F>
        void insert(const T& key, const F& value)
        {
            if (mData.contains(key))
                mData.get(mData.indexOf(key)).mSecond = value;
            else
                mData.add(key, value);
        }

        void insert(const K& key, const V& value)
        {
            if (mData.contains(key))
                mData.get(mData.indexOf(key)).mSecond = value;
            else
                mData.add(key, value);
        }

        void insert(const Pair<K, V>& pair)
        {
            if (mData.contains(pair.mFirst))
                mData.get(mData.indexOf(pair.mFirst)).mSecond = pair.mSecond;
            else
                mData.add(pair.mFirst, pair.mSecond);
        }

        template<typename T>
        bool containsKey(const T& key) const
        {
            return mData.contains(key);
        }

        template<typename T>
        bool containsValue(const T& value) const
        {
            for (auto& [k, v] : *this)
            {
                if (v == value)
                    return true;
            }

            return false;
        }

        template<typename T>
        void remove(const T& key)
        {
            mData.remove(mData.indexOf(key));
        }

        template<typename F>
        void forEach(F Function)
        {
            mData.forEach(Function);
        }

        void setSize(uint64 size)
        {
            mData.setSize(size);
        }

        void setCapacity(uint64 capacity)
        {
            mData.setSize(capacity);
        }

        void shrink()
        {
            mData.shrink();
        }

        void clear()
        {
            mData.clear();
        }

        Pair<K, V>* begin()
        {
            return mData.begin();
        }

        Pair<K, V>* end()
        {
            return mData.end();
        }

        const Pair<K, V>* begin() const
        {
            return mData.begin();
        }

        const Pair<K, V>* end() const
        {
            return mData.end();
        }

        Pair<K, V>* getData()
        {
            return mData.getData();
        }

        const Pair<K, V>* getData() const
        {
            return mData.getData();
        }

        uint64 getSize() const
        {
            return mData.getSize();
        }

        uint64 getCapacity() const
        {
            return mData.getCapacity();
        }

        template<typename T>
        V& get(const T& key)
        {
            return mData.get(mData.indexOf(key)).value;
        }

        template<typename T>
        const V& get(const T& key) const
        {
            return mData.get(mData.indexOf(key)).value;
        }

    private:
        DynamicArray<Pair<K, V>> mData;
    };

} // namespace otto
