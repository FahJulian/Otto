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

        explicit Serialized(Type type, bool hasBrackets = false)
            : mType(type), mHasBrackets(hasBrackets)
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

        const DynamicArray<Serialized>& getList() const
        {
            return mList;
        }

        template<typename T>
        DynamicArray<T> getList() const
        {
            DynamicArray<T> list = DynamicArray<T>(mList.getSize());

            for (auto& element : mList)
                list.add(element.get<T>());

            return list;
        }

        const Map<String, Serialized>& getDictionary() const
        {
            return mDictionary;
        }

        template<typename T>
        Map<String, T> getDictionary() const
        {
            Map<String, T> dictionary = Map<String, T>(mDictionary.getSize());

            for (auto& [key, element] : mDictionary)
                dictionary.insert(key, element.get<T>());

            return dictionary;
        }

        bool contains(const String& entryName) const
        {
            for (auto& [key, _] : mDictionary)
            {
                if (key.equalsIgnoreCase(entryName))
                    return true;
            }

            return false;
        }

        const Serialized& get(const String& entryName) const
        {
            for (auto& [key, _] : mDictionary)
            {
                if (key.equalsIgnoreCase(entryName))
                    return mDictionary.get(key);
            }

            return mDictionary.get(entryName); // Will cause error
        }

        template<typename T> requires std::is_integral<T>::value
            T get() const
        {
            return mValue.to<T>();
        }

        template<typename T>
        T get() const
        {
            return deserialize<T>(*this);
        }

        template<>
        String get() const
        {
            return mValue;
        }

        template<typename T> requires std::is_integral<T>::value
            T get(const String& entryName) const
        {
            for (auto& [key, _] : mDictionary)
            {
                if (key.equalsIgnoreCase(entryName))
                    return mDictionary.get(key).get<T>();
            }

            return mDictionary.get(entryName).get<T>(); // Will cause error
        }

        template<typename T>
        T get(const String& entryName) const
        {
            for (auto& [key, _] : mDictionary)
            {
                if (key.equalsIgnoreCase(entryName))
                    return mDictionary.get(key).get<T>();
            }

            return mDictionary.get(entryName).get<T>(); // Will cause error
        }

        const Serialized& get(uint64 index)
        {
            return mList[index];
        }

        template<typename T> requires std::is_integral<T>::value
            void insert(const String& name, T value)
        {
            if (mType == Type::VOID)
                mType = Type::DICTIONARY;

            for (auto& [key, _] : mDictionary)
            {
                if (key.equalsIgnoreCase(name))
                {
                    mDictionary[key] = String::valueOf(value);
                    return;
                }
            }

            mDictionary.insert(name, String::valueOf(value));
        }

        template<typename T>
        void insert(const String& name, const T& value)
        {
            if (mType == Type::VOID)
                mType = Type::DICTIONARY;

            for (auto& [key, _] : mDictionary)
            {
                if (key.equalsIgnoreCase(name))
                {
                    mDictionary[key] = serialize<T>(value);
                    return;
                }
            }

            mDictionary.insert(name, serialize<T>(value));
        }

        String toString() const
        {
            String s;

            switch (mType)
            {
            case Type::VALUE:
                return mValue;

            case Type::DICTIONARY:
                if (!mHasBrackets)
                    s = "{\n";

                for (auto& [key, value] : mDictionary)
                    s.append(key).append(": ").append(value.toString()).append('\n');

                if (!mHasBrackets)
                    s.append('}');

                return s;

            case Type::LIST:
                if (!mHasBrackets)
                    s = "[\n";

                for (auto& value : mList)
                    s.append(value.toString()).append('\n');

                if (!mHasBrackets)
                    s.append(']');

                return s;

            default:
                return s;
            }
        }

        bool isValue() const { return mType == Type::VALUE; }
        bool isList() const { return mType == Type::LIST; }
        bool isDictionary() const { return mType == Type::DICTIONARY; }

    private:
        Type mType;
        bool mHasBrackets = false;

        String mValue;
        DynamicArray<Serialized> mList;
        Map<String, Serialized> mDictionary;

        friend class Serializer;
    };

    template<typename T>
    Serialized serialize(const T& t)
    {
        OTTO_ASSERT(false, "Type is not serializable");
    }

    template<typename T>
    T deserialize(const Serialized& serialized)
    {
        OTTO_ASSERT(false, "Type is not deserializable");
    }

    //std::ostream& operator<<(std::ostream& stream, const Serialized& serialized);
    //{
    //    return (stream << serialized.toString());
    //}

} // namespace otto
