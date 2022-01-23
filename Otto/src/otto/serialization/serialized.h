#pragma once

#include "otto/base.h"
#include "otto/util/map.h"
#include "otto/util/string.h"
#include "otto/util/static_array.h"
#include "otto/util/dynamic_array.h"

namespace otto
{
    class Serialized
    {
        enum class Type : uint8
        {
            VOID,
            VALUE,
            LIST,
            DICTIONARY,
        };

    public:
        Serialized()
            : mType(Type::VOID)
        {
        }

        explicit Serialized(Type type)
            : mType(type)
        {
        }

        template<typename T> requires std::is_integral_v<T>
        Serialized(T value)
            : mType(Type::VALUE), mValue(otto::String::toString(mValue))
        {
        }

        Serialized(const String& value)
            : mType(Type::VALUE), mValue(value)
        {
        }

        template<typename T>
        Serialized(const T& value)
            : mType(Type::VALUE), mValue(serialize(value))
        {
        }

        Serialized(const DynamicArray<Serialized>& list)
            : mType(Type::LIST), mList(list)
        {
        }

        Serialized(const Map<String, Serialized>& dictionary)
            : mType(Type::DICTIONARY), mDictionary(dictionary)
        {
        }

        template<typename T>
        T get()
        {
            return deserialize<T>(*this);
        }

        template<>
        bool get()
        {
            return mValue.to<bool>();
        }

        template<>
        uint8 get()
        {
            return mValue.to<uint8>();
        }

        template<>
        uint16 get()
        {
            return mValue.to<uint16>();
        }

        template<>
        uint32 get()
        {
            return mValue.to<uint32>();
        }

        template<>
        uint64 get()
        {
            return mValue.to<uint64>();
        }

        template<>
        int8 get()
        {
            return mValue.to<int8>();
        }

        template<>
        int16 get()
        {
            return mValue.to<int16>();
        }

        template<>
        int32 get()
        {
            return mValue.to<int32>();
        }

        template<>
        int64 get()
        {
            return mValue.to<int64>();
        }

        template<>
        float32 get()
        {
            return mValue.to<float32>();
        }

        template<>
        float64 get()
        {
            return mValue.to<float64>();
        }

        template<>
        String get()
        {
            return mValue;
        }

        template<typename T>
        DynamicArray<T> get()
        {
            return NONE;
        }

        template<typename T, uint64 N>
        StaticArray<T, N> get()
        {
            return NONE;
        }

        template<typename T, typename F>
        Map<F, T> get()
        {
            return NONE;
        }

        bool contains(const String& entryName) const
        {
            return mDictionary.containsKey(entryName);
        }

        Serialized& get(const String& entryName)
        {
            mDictionary.get(entryName);
        }

        const Serialized& get(uint64 index)
        {
            return mList[index];
        }

        String toString() const
        {
            String s;

            switch (mType)
            {
            case Type::VALUE:
                return mValue;

            case Type::DICTIONARY:
                s = "{\n";

                for (auto& [key, value] : mDictionary)
                    s.append(key).append(": ").append(value.toString()).append('\n');

                s.append('}');

                return s;

            case Type::LIST:
                s = "[\n";

                for (auto& value : mList)
                    s.append(value.toString()).append('\n');

                s.append(']');

                return s;

            default:
                return s;
            }
        }

        bool isValue() const { return mType == Type::VALUE; }
        bool isList() const { return mType == Type::LIST; }
        bool isDictionary() const { return mType == Type::DICTIONARY; }

    //private:
        Type mType;

        String mValue;
        DynamicArray<Serialized> mList;
        Map<String, Serialized> mDictionary;

        friend struct OttoFile;
        friend class OttoFileLoader;
    };

} // namespace otto
