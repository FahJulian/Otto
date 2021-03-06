#include "String.h"

#include <cmath>
#include <string>
#include <istream>
#include <charconv>
#include <stdlib.h>

#include "otto/base.h"
#include "otto/util/static_array.h"

namespace otto
{
    namespace
    {
        StaticArray<char, 8 * sizeof(long long)> STRING_CAST_BUFFER;

        template<typename T>
        String _floatToString(T value, bool8 scientific)
        {
            auto result = std::to_chars(STRING_CAST_BUFFER.begin(), STRING_CAST_BUFFER.end(),
                value, scientific ? std::chars_format::scientific : std::chars_format::fixed, 2);

            return String(STRING_CAST_BUFFER.begin(), result.ptr - STRING_CAST_BUFFER.begin());
        }

        template<typename T>
        String _integerToString(T value, int32 base)
        {
            auto result = std::to_chars(STRING_CAST_BUFFER.begin(), STRING_CAST_BUFFER.end(), value, base);
            return String(STRING_CAST_BUFFER.begin(), result.ptr - STRING_CAST_BUFFER.begin());
        }

        char8 _toLowerCase(char8 c)
        {
            return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
        }

        char8 _toUpperCase(char8 c)
        {
            return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
        }

        bool8 _equalsIgnoreCase(const char8* c1, const char8* c2, uint64 len)
        {
            for (uint64 i = 0; i < len; i++)
            {
                if (_toLowerCase(c1[i]) != _toLowerCase(c2[i]))
                    return false;
            }

            return true;
        }

        bool8 _isWhitespace(char8 c)
        {
            return c <= ' ';
        }

    } // namespace

    constexpr String::String(char8 c)
        : mSize(uint64(1)), mData(new char8[2] { c, '\0' })
    {
    }

    constexpr String::String() noexcept
        : mSize(0), mData(new char('\0'))
    {
    }

    String::String(const char8* data)
        : mSize(std::strlen(data)), mData(new char8[mSize + 1])
    {
        std::memcpy(mData, data, mSize);
        mData[mSize] = '\0';
    }

    String::String(const char8* data, uint64 size)
        : mSize(size), mData(new char8[mSize + 1])
    {
        if (data != nullptr)
            std::memcpy(mData, data, mSize);

        mData[mSize] = '\0';
    }

    String::String(uint64 size)
        : String(nullptr, size)
    {
    }

    String::String(const String& other)
        : mSize(other.mSize), mData(new char8[mSize + 1])
    {
        std::memcpy(mData, other.mData, mSize);
        mData[mSize] = '\0';
    }

    String::String(String&& other) noexcept
        : mSize(other.mSize), mData(other.mData)
    {
        other.mSize = 0;
        other.mData = nullptr;
    }

    String::~String()
    {
        delete[] mData;
    }

    String& String::operator=(char8 c)
    {
        if (mSize != 1)
        {
            delete[] mData;

            mData = new char8[1]{ c };
            mSize = 1;
        }
        else
        {
            *mData = c;
        }

        return *this;
    }

    String& String::operator=(const char8* other)
    {
        uint64 len = std::strlen(other);

        if (mSize != len)
        {
            delete[] mData;

            mData = new char8[len + 1];
            std::memcpy(mData, other, len + 1);

            mSize = len;
        }
        else
        {
            std::memcpy(mData, other, len);
        }

        return *this;
    }

    String& String::operator=(const String& other)
    {
        if (this != &other)
        {
            mSize = other.mSize;

            delete[] mData;
            mData = new char8[mSize + 1];

            std::memcpy(mData, other.mData, mSize);
            mData[mSize] = '\0';
        }

        return *this;
    }

    String& String::operator=(String&& other) noexcept
    {
        if (this != &other)
        {
            mSize = other.mSize;

            delete[] mData;
            mData = other.mData;

            other.mSize = 0;
            other.mData = nullptr;
        }

        return *this;
    }

    String& String::append(char8 c)
    {
        char8* newData = new char8[mSize + 2];
        std::memcpy(newData, mData, mSize);

        mSize++;

        newData[mSize - 1] = c;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;

        return *this;
    }

    String& String::append(const String& string)
    {
        char8* newData = new char8[mSize + string.mSize + 1];
        std::memcpy(newData, mData, mSize);
        std::memcpy(newData + mSize, string.mData, string.mSize);

        mSize += string.mSize;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;

        return *this;
    }

    String& String::append(const char8* string)
    {
        uint64 len = std::strlen(string);

        char8* newData = new char8[mSize + len + 1];
        std::memcpy(newData, mData, mSize);
        std::memcpy(newData + mSize, string, len);

        mSize += len;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;

        return *this;
    }

    String& String::insert(uint64 index, char8 c)
    {
        OTTO_ASSERT(index <= mSize, "Index is out of bounds");

        char8* newData = new char8[mSize + 2];
        std::memcpy(newData, mData, index);
        std::memcpy(newData + index + 1, mData + index, mSize - index);

        mSize++;

        newData[index] = c;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;

        return *this;
    }

    String& String::insert(uint64 index, const String& string)
    {
        OTTO_ASSERT(index <= mSize, "Index is out of bounds");

        char8* newData = new char8[mSize + string.mSize + 1];
        std::memcpy(newData, mData, index);
        std::memcpy(newData + index, string.mData, string.mSize);
        std::memcpy(newData + index + string.mSize, mData + index, mSize - index);

        mSize += string.mSize;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;

        return *this;
    }

    String& String::insert(uint64 index, const char8* string)
    {
        OTTO_ASSERT(index <= mSize, "Index is out of bounds");

        uint64 len = std::strlen(string);

        char8* newData = new char8[mSize + len + 1];
        std::memcpy(newData, mData, index);
        std::memcpy(newData + index, string, len);
        std::memcpy(newData + index + len, mData + index, mSize - index);

        mSize += len;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;

        return *this;
    }

    String& String::trim()
    {
        uint64 beginIndex = findFirstNotOfWhiteSpace();

        if (beginIndex == mSize)    // String is all whitespace
            return (*this = String());

        return toSubString(beginIndex, findLastNotOfWhiteSpace() + 1);
    }

    String& String::toLowerCase()
    {
        for (char8* c = mData; c < mData + mSize; c++)
            *c = _toLowerCase(*c);

        return *this;
    }

    String& String::toUpperCase()
    {
        for (char8* c = mData; c < mData + mSize; c++)
            *c = _toUpperCase(*c);

        return *this;
    }

    String& String::toSubString(uint64 beginIndex)
    {
        OTTO_ASSERT(beginIndex <= mSize, "beginIndex must be less than size");

        if (beginIndex != 0)
        {
            mSize = mSize - beginIndex;

            char8* newData = new char8[mSize + 1];

            std::memcpy(newData, mData + beginIndex, mSize);
            newData[mSize] = '\0';

            delete[] mData;
            mData = newData;
        }

        return *this;
    }

    String& String::toSubString(uint64 beginIndex, uint64 endIndex)
    {
        OTTO_ASSERT(endIndex <= mSize, "endIndex must be less than size");
        OTTO_ASSERT(endIndex >= beginIndex, "endIndex must be >= beginIndex");

        if (mSize != endIndex - beginIndex)
        {
            mSize = endIndex - beginIndex;

            char8* newData = new char8[mSize + 1];

            std::memcpy(newData, mData + beginIndex, mSize);
            newData[mSize] = '\0';

            delete[] mData;
            mData = newData;
        }

        return *this;
    }

    void String::setSize(uint64 size)
    {
        char8* newData = new char8[size + 1];
        std::memcpy(newData, mData, mSize);

        mSize = size;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;
    }

    bool8 String::equals(char8 c) const
    {
        return mSize == 1 && *mData == c;
    }

    bool8 String::equals(const String& other) const
    {
        return std::strcmp(mData, other.mData) == 0;
    }

    bool8 String::equals(const char8* other) const
    {
        return std::strcmp(mData, other) == 0;
    }

    bool8 String::equalsIgnoreCase(char8 c) const
    {
        return mSize == 1 && _toLowerCase(*mData) == _toLowerCase(c);
    }

    bool8 String::equalsIgnoreCase(const String& other) const
    {
        if (mSize != other.mSize)
            return false;

        for (uint64 i = 0; i < mSize; i++)
        {
            if (_toLowerCase(mData[i]) != _toLowerCase(other.mData[i]))
                return false;
        }

        return true;
    }

    bool8 String::equalsIgnoreCase(const char8* other) const
    {
        if (mSize != std::strlen(other))
            return false;

        for (uint64 i = 0; i < mSize; i++)
        {
            if (_toLowerCase(mData[i]) != _toLowerCase(other[i]))
                return false;
        }

        return true;
    }

    bool8 String::startsWith(char8 c) const
    {
        if (isEmpty())
            return false;

        return mData[0] == c;
    }

    bool8 String::startsWith(const char8* string) const
    {
        uint64 len = std::strlen(string);

        if (len > mSize)
            return false;

        return std::strncmp(mData, string, len) == 0;
    }

    bool8 String::startsWith(const String& string) const
    {
        if (string.mSize > mSize)
            return false;

        return std::strncmp(mData, string.mData, string.mSize) == 0;
    }

    bool8 String::startsWithIgnoreCase(char8 c) const
    {
        if (isEmpty())
            return false;

        return _toLowerCase(mData[0]) == _toLowerCase(c);
    }

    bool8 String::startsWithIgnoreCase(const char8* string) const
    {
        uint64 len = std::strlen(string);

        if (len > mSize)
            return false;

        return _equalsIgnoreCase(mData, string, len);
    }

    bool8 String::startsWithIgnoreCase(const String& string) const
    {
        if (string.mSize > mSize)
            return false;

        return _equalsIgnoreCase(mData, string.mData, string.mSize);
    }

    bool8 String::endsWith(char8 c) const
    {
        if (isEmpty())
            return false;

        return mData[mSize - 1] == c;
    }

    bool8 String::endsWith(const char8* string) const
    {
        uint64 len = std::strlen(string);

        if (len > mSize)
            return false;

        return std::strncmp(mData + mSize - len, string, len) == 0;
    }

    bool8 String::endsWith(const String& string) const
    {
        if (string.mSize > mSize)
            return false;

        return std::strncmp(mData + mSize - string.mSize, string.mData, string.mSize) == 0;
    }

    bool8 String::endsWithIgnoreCase(char8 c) const
    {
        if (isEmpty())
            return false;

        return _toLowerCase(mData[mSize - 1]) == _toLowerCase(c);
    }

    bool8 String::endsWithIgnoreCase(const char8* string) const
    {
        uint64 len = std::strlen(string);

        if (len > mSize)
            return false;

        return _equalsIgnoreCase(mData + mSize - len, string, len);
    }

    bool8 String::endsWithIgnoreCase(const String& string) const
    {
        if (string.mSize > mSize)
            return false;

        return _equalsIgnoreCase(mData + mSize - string.mSize, string.mData, string.mSize);
    }

    bool8 String::contains(char8 c) const
    {
        return findFirstOf(c) != mSize;
    }

    bool8 String::contains(const String& string) const
    {
        return findFirstOf(string) != mSize;
    }

    bool8 String::contains(const char8* string) const
    {
        return findFirstOf(string) != mSize;
    }

    bool8 String::containsIgnoreCase(char8 c) const
    {
        return findFirstOfIgnoreCase(c) != mSize;
    }

    bool8 String::containsIgnoreCase(const String& string) const
    {
        return findFirstOfIgnoreCase(string) != mSize;
    }

    bool8 String::containsIgnoreCase(const char8* string) const
    {
        return findFirstOfIgnoreCase(string) != mSize;
    }

    String& String::replace(uint64 beginIndex, uint64 endIndex, char8 c)
    {
        OTTO_ASSERT(endIndex >= beginIndex, "endIndex must be greater than or equal to beginIndex");
        OTTO_ASSERT(endIndex <= mSize, "endIndex must be less than or equal to string Size")

        uint64 deltaLen = static_cast<uint64>(1) - (endIndex - beginIndex);

        if (deltaLen != 0)
        {
            char8* newData = new char8[mSize + 1 + deltaLen];
            std::memcpy(newData, mData, beginIndex);
            std::memcpy(newData + beginIndex + 1, mData + endIndex, mSize - endIndex);

            newData[beginIndex] = c;
            newData[mSize += deltaLen] = '0';

            delete[] mData;
            mData = newData;
        }
        else
        {
            mData[beginIndex] = c;
        }

        return *this;
    }

    String& String::replace(uint64 beginIndex, uint64 endIndex, const char8* string)
    {
        OTTO_ASSERT(endIndex >= beginIndex, "endIndex must be greater than or equal to beginIndex");
        OTTO_ASSERT(endIndex <= mSize, "endIndex must be less than or equal to string Size")

        uint64 len = std::strlen(string);

        uint64 deltaLen = len - (endIndex - beginIndex);

        if (deltaLen != static_cast<uint64>(0))
        {
            char8* newData = new char8[mSize + 1 + deltaLen];
            std::memcpy(newData, mData, beginIndex);
            std::memcpy(newData + beginIndex + len, mData + endIndex, mSize - endIndex);

            std::memcpy(newData + beginIndex, string, len);
            newData[mSize += deltaLen] = '0';

            delete[] mData;
            mData = newData;
        }
        else
        {
            std::memcpy(mData + beginIndex, string, len);
        }

        return *this;
    }

    String& String::replace(uint64 beginIndex, uint64 endIndex, const String& string)
    {
        OTTO_ASSERT(endIndex >= beginIndex, "endIndex must be greater than or equal to beginIndex");
        OTTO_ASSERT(endIndex <= mSize, "endIndex must be less than or equal to string Size")

        uint64 deltaLen = string.mSize - (endIndex - beginIndex);

        if (deltaLen != static_cast<uint64>(0))
        {
            char8* newData = new char8[mSize + 1 + deltaLen];
            std::memcpy(newData, mData, beginIndex);
            std::memcpy(newData + beginIndex + string.mSize, mData + endIndex, mSize - endIndex);

            std::memcpy(newData + beginIndex, string.mData, string.mSize);
            newData[mSize += deltaLen] = '0';

            delete[] mData;
            mData = newData;
        }
        else
        {
            std::memcpy(mData + beginIndex, string.mData, string.mSize);
        }

        return *this;
    }

    String& String::replaceAll(char8 oldChar, char8 newChar, uint64 startIndex)
    {
        while ((startIndex = _replaceFirst(oldChar, newChar, startIndex)) != mSize)
            ;

        return *this;
    }

    String& String::replaceAll(const char8* oldString, const char8* newString, uint64 startIndex)
    {
        while ((startIndex = _replaceFirst(oldString, newString, startIndex)) != mSize)
            ;

        return *this;
    }

    String& String::replaceAll(const String& oldString, const String& newString, uint64 startIndex)
    {
        while ((startIndex = _replaceFirst(oldString, newString, startIndex)) != mSize)
            ;

        return *this;
    }

    String& String::replaceAllIgnoreCase(char8 oldChar, char8 newChar, uint64 startIndex)
    {
        while ((startIndex = _replaceFirstIgnoreCase(oldChar, newChar, startIndex)) != mSize)
            ;

        return *this;
    }

    String& String::replaceAllIgnoreCase(const char8* oldString, const char8* newString, uint64 startIndex)
    {
        while ((startIndex = _replaceFirstIgnoreCase(oldString, newString, startIndex)) != mSize)
            ;

        return *this;
    }

    String& String::replaceAllIgnoreCase(const String& oldString, const String& newString, uint64 startIndex)
    {
        while ((startIndex = _replaceFirstIgnoreCase(oldString, newString, startIndex)) != mSize)
            ;

        return *this;
    }

    String& String::replaceFirst(char8 oldChar, char8 newChar, uint64 startIndex)
    {
        _replaceFirst(oldChar, newChar, startIndex);
        return *this;
    }

    String& String::replaceFirst(const char8* oldString, const char8* newString, uint64 startIndex)
    {
        _replaceFirst(oldString, newString, startIndex);
        return *this;
    }

    String& String::replaceFirst(const String& oldString, const String& newString, uint64 startIndex)
    {
        _replaceFirst(oldString, newString, startIndex);
        return *this;
    }

    String& String::replaceFirstIgnoreCase(char8 oldChar, char8 newChar, uint64 startIndex)
    {
        _replaceFirstIgnoreCase(oldChar, newChar, startIndex);
        return *this;
    }

    String& String::replaceFirstIgnoreCase(const char8* oldString, const char8* newString, uint64 startIndex)
    {
        _replaceFirstIgnoreCase(oldString, newString, startIndex);
        return *this;
    }

    String& String::replaceFirstIgnoreCase(const String& oldString, const String& newString, uint64 startIndex)
    {
        _replaceFirstIgnoreCase(oldString, newString, startIndex);
        return *this;
    }

    String& String::replaceLast(char8 oldChar, char8 newChar, uint64 endIndex)
    {
        uint64 index = findLastOf(oldChar, endIndex);

        if (index != mSize)
            mData[index] = newChar;

        return *this;
    }

    String& String::replaceLast(const char8* oldString, const char8* newString, uint64 endIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = findLastOf(oldString, endIndex);

        if (index == mSize)
            return *this;

        if (int32 deltaLen = static_cast<int>(newLen - oldLen);
            deltaLen != 0)
        {
            char8* newData = new char8[mSize + deltaLen + 1];

            std::memcpy(newData, mData, index);
            std::memcpy(newData + index, newString, newLen);
            std::memcpy(newData + index + newLen, mData + index + oldLen, mSize - (index + oldLen));

            delete[] mData;
            mData = newData;
            mSize += deltaLen;

            mData[mSize] = '\0';
        }
        else
        {
            std::memcpy(mData + index, newString, newLen);
        }

        return *this;
    }

    String& String::replaceLast(const String& oldString, const String& newString, uint64 endIndex)
    {
        uint64 index = findLastOf(oldString, endIndex);

        if (index == mSize)
            return *this;

        if (int32 deltaLen = static_cast<int>(newString.mSize - oldString.mSize);
            deltaLen != 0)
        {
            char8* newData = new char8[mSize + deltaLen + 1];

            std::memcpy(newData, mData, index);
            std::memcpy(newData + index, newString.mData, newString.mSize);
            std::memcpy(newData + index + newString.mSize, mData + index + oldString.mSize, mSize - (index + oldString.mSize));

            delete[] mData;
            mData = newData;
            mSize += deltaLen;

            mData[mSize] = '\0';
        }
        else
        {
            std::memcpy(mData + index, newString.mData, newString.mSize);
        }

        return *this;
    }

    String& String::replaceLastIgnoreCase(char8 oldChar, char8 newChar, uint64 endIndex)
    {
        uint64 index = findLastOfIgnoreCase(oldChar, endIndex);

        if (index != mSize)
            mData[index] = newChar;

        return *this;
    }

    String& String::replaceLastIgnoreCase(const char8* oldString, const char8* newString, uint64 endIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = findLastOfIgnoreCase(oldString, endIndex);

        if (index == mSize)
            return *this;

        if (int32 deltaLen = static_cast<int>(newLen - oldLen);
            deltaLen != 0)
        {
            char8* newData = new char8[mSize + deltaLen + 1];

            std::memcpy(newData, mData, index);
            std::memcpy(newData + index, newString, newLen);
            std::memcpy(newData + index + newLen, mData + index + oldLen, mSize - (index + oldLen));

            delete[] mData;
            mData = newData;
            mSize += deltaLen;

            mData[mSize] = '\0';
        }
        else
        {
            std::memcpy(mData + index, newString, newLen);
        }

        return *this;
    }

    String& String::replaceLastIgnoreCase(const String& oldString, const String& newString, uint64 endIndex)
    {
        uint64 index = findLastOfIgnoreCase(oldString, endIndex);

        if (index == mSize)
            return *this;

        if (int32 deltaLen = static_cast<int>(newString.mSize - oldString.mSize);
            deltaLen != 0)
        {
            char8* newData = new char8[mSize + deltaLen + 1];

            std::memcpy(newData, mData, index);
            std::memcpy(newData + index, newString.mData, newString.mSize);
            std::memcpy(newData + index + newString.mSize, mData + index + oldString.mSize, mSize - (index + oldString.mSize));

            delete[] mData;
            mData = newData;
            mSize += deltaLen;

            mData[mSize] = '\0';
        }
        else
        {
            std::memcpy(mData + index, newString.mData, newString.mSize);
        }

        return *this;
    }

    String& String::removeLast(char8 c, uint64 endIndex)
    {
        uint64 index = findLastOf(c, endIndex);
        if (index != mSize)
        {
            char8* newData = new char[(mSize - 1) + 1];
            
            std::memcpy(newData, mData, index);
            std::memcpy(newData + index, mData + index + 1, mSize - index);

            newData[(mSize - 1)] = '\0';

            delete[] mData;
            mData = newData;

            mSize -= 1;
        }

        return *this;
    }

    String& String::title()
    {
        if (!isEmpty())
            mData[0] = _toUpperCase(mData[0]);

        return *this;
    }

    String& String::untitle()
    {
        if (!isEmpty())
            mData[0] = _toLowerCase(mData[0]);

        return *this;
    }

    uint64 String::findFirstOf(char8 c, uint64 startIndex) const
    {
        if (isEmpty())
            return mSize;

        for (uint64 i = startIndex; i < mSize; i++)
        {
            if (mData[i] == c)
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstOf(const char8* string, uint64 startIndex) const
    {
        uint64 len = std::strlen(string);
        if (len > mSize)
            return mSize;

        for (uint64 i = startIndex; i <= mSize - len; i++)
        {
            if (strncmp(mData + i, string, len) == 0)
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstOf(const String& string, uint64 startIndex) const
    {
        if (string.mSize > mSize)
            return mSize;

        for (uint64 i = startIndex; i <= mSize - string.mSize; i++)
        {
            if (strncmp(mData + i, string.mData, string.mSize) == 0)
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstOfIgnoreCase(char8 c, uint64 startIndex) const
    {
        if (isEmpty())
            return mSize;

        c = _toLowerCase(c);

        for (uint64 i = startIndex; i < mSize; i++)
        {
            if (_toLowerCase(mData[i]) == c)
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstOfIgnoreCase(const char8* string, uint64 startIndex) const
    {
        uint64 len = std::strlen(string);
        if (len > mSize)
            return mSize;

        for (uint64 i = startIndex; i <= mSize - len; i++)
        {
            if (_equalsIgnoreCase(mData + i, string, len))
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstOfIgnoreCase(const String& string, uint64 startIndex) const
    {
        if (string.mSize > mSize)
            return mSize;

        for (uint64 i = startIndex; i <= mSize - string.mSize; i++)
        {
            if (_equalsIgnoreCase(mData + i, string.mData, string.mSize))
                return i;
        }

        return mSize;
    }

    uint64 String::findLastOf(char8 c, uint64 endIndex) const
    {
        if (isEmpty())
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize;

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (mData[i - 1] == c)
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastOf(const char8* string, uint64 endIndex) const
    {
        uint64 len = std::strlen(string);
        if (len > mSize)
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize - (len - 1);

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (strncmp(mData + i - 1, string, len) == 0)
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastOf(const String& string, uint64 endIndex) const
    {
        if (string.mSize > mSize)
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize - (string.mSize - 1);

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (strncmp(mData + i - 1, string.mData, string.mSize) == 0)
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastOfIgnoreCase(char8 c, uint64 endIndex) const
    {
        if (isEmpty())
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize;

        if (endIndex > mSize)
            return mSize;

        c = _toLowerCase(c);

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (_toLowerCase(mData[i - 1]) == c)
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastOfIgnoreCase(const char8* string, uint64 endIndex) const
    {
        uint64 len = std::strlen(string);
        if (len > mSize)
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize - (len - 1);

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (_equalsIgnoreCase(mData + i - 1, string, len))
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastOfIgnoreCase(const String& string, uint64 endIndex) const
    {
        if (string.mSize > mSize)
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize - (string.mSize - 1);

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (_equalsIgnoreCase(mData + i - 1, string.mData, string.mSize))
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findFirstNotOf(char8 c, uint64 startIndex) const
    {
        if (isEmpty())
            return mSize;

        for (uint64 i = startIndex; i < mSize; i++)
        {
            if (mData[i] != c)
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstNotOf(const char8* string, uint64 startIndex) const
    {
        uint64 len = std::strlen(string);
        if (len > mSize)
            return mSize;

        for (uint64 i = startIndex; i <= mSize - len; i++)
        {
            if (strncmp(mData + i, string, len) != 0)
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstNotOf(const String& string, uint64 startIndex) const
    {
        if (string.mSize > mSize)
            return mSize;

        for (uint64 i = startIndex; i <= mSize - string.mSize; i++)
        {
            if (strncmp(mData + i, string.mData, string.mSize) != 0)
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstNotOfIgnoreCase(char8 c, uint64 startIndex) const
    {
        if (isEmpty())
            return mSize;

        c = _toLowerCase(c);

        for (uint64 i = startIndex; i < mSize; i++)
        {
            if (_toLowerCase(mData[i]) != c)
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstNotOfIgnoreCase(const char8* string, uint64 startIndex) const
    {
        uint64 len = std::strlen(string);
        if (len > mSize)
            return mSize;

        for (uint64 i = startIndex; i <= mSize - len; i++)
        {
            if (!_equalsIgnoreCase(mData + i, string, len))
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstNotOfIgnoreCase(const String& string, uint64 startIndex) const
    {
        if (string.mSize > mSize)
            return mSize;

        for (uint64 i = startIndex; i <= mSize - string.mSize; i++)
        {
            if (!_equalsIgnoreCase(mData + i, string.mData, string.mSize))
                return i;
        }

        return mSize;
    }

    uint64 String::findLastNotOf(char8 c, uint64 endIndex) const
    {
        if (isEmpty())
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize;

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (mData[i - 1] != c)
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastNotOf(const char8* string, uint64 endIndex) const
    {
        uint64 len = std::strlen(string);
        if (len > mSize)
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize - (len - 1);

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (strncmp(mData + i - 1, string, len) != 0)
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastNotOf(const String& string, uint64 endIndex) const
    {
        if (string.mSize > mSize)
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize - (string.mSize - 1);

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (strncmp(mData + i - 1, string.mData, string.mSize) != 0)
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastNotOfIgnoreCase(char8 c, uint64 endIndex) const
    {
        if (isEmpty())
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize;

        if (endIndex > mSize)
            return mSize;

        c = _toLowerCase(c);

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (_toLowerCase(mData[i - 1]) != c)
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastNotOfIgnoreCase(const char8* string, uint64 endIndex) const
    {
        uint64 len = std::strlen(string);
        if (len > mSize)
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize - (len - 1);

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (!_equalsIgnoreCase(mData + i - 1, string, len))
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastNotOfIgnoreCase(const String& string, uint64 endIndex) const
    {
        if (string.mSize > mSize)
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize - (string.mSize - 1);

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (!_equalsIgnoreCase(mData + i - 1, string.mData, string.mSize))
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findFirstOfWhiteSpace(uint64 startIndex) const
    {
        if (isEmpty())
            return mSize;

        for (uint64 i = startIndex; i < mSize; i++)
        {
            if (_isWhitespace(mData[i]))
                return i;
        }

        return mSize;
    }

    uint64 String::findFirstNotOfWhiteSpace(uint64 startIndex) const
    {
        if (isEmpty())
            return mSize;

        for (uint64 i = startIndex; i < mSize; i++)
        {
            if (!_isWhitespace(mData[i]))
                return i;
        }

        return mSize;
    }

    uint64 String::findLastOfWhiteSpace(uint64 endIndex) const
    {
        if (isEmpty())
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize;

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (_isWhitespace(mData[i - 1]))
                return i - 1;
        }

        return mSize;
    }

    uint64 String::findLastNotOfWhiteSpace(uint64 endIndex) const
    {
        if (isEmpty())
            return mSize;

        if (endIndex == std::numeric_limits<uint64>::max())
            endIndex = mSize;

        if (endIndex > mSize)
            return mSize;

        for (uint64 i = endIndex; i > 0; i--)
        {
            if (!_isWhitespace(mData[i - 1]))
                return i - 1;
        }

        return mSize;
    }


    DynamicArray<String> String::split(const String& string, char8 c)
    {
        if (string.isEmpty())
            return { };

        DynamicArray<String> parts;

        const char8* nextPartStart = string.mData;
        for (const char8* i = string.mData; i < string.mData + string.mSize; i++)
        {
            if (*i == c)
            {
                if (uint64 length = i - nextPartStart;
                    length > 0)
                {
                    parts.add(nextPartStart, length);
                }

                nextPartStart = i + 1;
            }
        }

        if (uint64 length = string.mData + string.mSize - nextPartStart;
            length > 0)
        {
            parts.add(nextPartStart, length);
        }

        return parts;
    }

    DynamicArray<String> String::split(const String& string, const char8* splitString)
    {
        uint64 len = std::strlen(splitString);
        if (len > string.mSize)
            return { };

        DynamicArray<String> parts;

        const char8* i = string.mData;
        const char8* nextPartStart = string.mData;
        while (i <= string.mData + string.mSize - len)
        {
            if (std::strncmp(splitString, i, len) == 0)
            {
                if (uint64 length = i - nextPartStart;
                    length > 0)
                {
                    parts.add(nextPartStart, length);
                }

                nextPartStart = i + len;
                i += len;
            }
            else
            {
                i++;
            }
        }

        if (uint64 length = string.mData + string.mSize - nextPartStart;
            length > 0)
        {
            parts.add(nextPartStart, length);
        }

        return parts;
    }

    DynamicArray<String> String::split(const String& string, const String& splitString)
    {
        if (splitString.mSize >= string.mSize)
            return { };

        DynamicArray<String> parts;

        const char8* i = string.mData;
        const char8* nextPartStart = string.mData;
        while (i <= string.mData + string.mSize - splitString.mSize)
        {
            if (std::strncmp(splitString.mData, i, splitString.mSize) == 0)
            {
                if (uint64 length = i - nextPartStart;
                    length > 0)
                {
                    parts.add(nextPartStart, length);
                }

                nextPartStart = i + splitString.mSize;
                i += splitString.mSize;
            }
            else
            {
                i++;
            }
        }

        if (uint64 length = string.mData + string.mSize - nextPartStart;
            length > 0)
        {
            parts.add(nextPartStart, length);
        }

        return parts;
    }

    DynamicArray<String> String::splitIgnoreCase(const String& string, char8 c)
    {
        if (string.isEmpty())
            return { };

        DynamicArray<String> parts;

        c = _toLowerCase(c);

        const char8* nextPartStart = string.mData;
        for (const char8* i = string.mData; i < string.mData + string.mSize; i++)
        {
            if (_toLowerCase(*i) == c)
            {
                if (uint64 length = i - nextPartStart;
                    length > 0)
                {
                    parts.add(nextPartStart, length);
                }

                nextPartStart = i + 1;
            }
        }

        if (uint64 length = string.mData + string.mSize - nextPartStart;
            length > 0)
        {
            parts.add(nextPartStart, length);
        }

        return parts;
    }

    DynamicArray<String> String::splitIgnoreCase(const String& string, const char8* splitString)
    {
        uint64 len = std::strlen(splitString);
        if (len >= string.mSize)
            return { };

        DynamicArray<String> parts;

        const char8* i = string.mData;
        const char8* nextPartStart = string.mData;
        while (i <= string.mData + string.mSize - len)
        {
            if (_equalsIgnoreCase(splitString, i, len))
            {
                if (uint64 length = i - nextPartStart;
                    length > 0)
                {
                    parts.add(nextPartStart, length);
                }

                nextPartStart = i + len;
                i += len;
            }
            else
            {
                i++;
            }
        }

        if (uint64 length = string.mData + string.mSize - nextPartStart;
            length > 0)
        {
            parts.add(nextPartStart, length);
        }

        return parts;
    }

    DynamicArray<String> String::splitIgnoreCase(const String& string, const String& splitString)
    {
        if (splitString.mSize >= string.mSize)
            return { };

        DynamicArray<String> parts;

        const char8* i = string.mData;
        const char8* nextPartStart = string.mData;
        while (i <= string.mData + string.mSize - splitString.mSize)
        {
            if (_equalsIgnoreCase(splitString.mData, i, splitString.mSize))
            {
                if (uint64 length = i - nextPartStart;
                    length > 0)
                {
                    parts.add(nextPartStart, length);
                }

                nextPartStart = i + splitString.mSize;
                i += splitString.mSize;
            }
            else
            {
                i++;
            }
        }

        if (uint64 length = string.mData + string.mSize - nextPartStart;
            length > 0)
        {
            parts.add(nextPartStart, length);
        }

        return parts;
    }

    String String::replace(const String& string, uint64 beginIndex, uint64 endIndex, char8 c)
    {
        OTTO_ASSERT(endIndex >= beginIndex, "endIndex must be greater than or equal to beginIndex");
        OTTO_ASSERT(endIndex <= string.mSize, "endIndex must be less than or equal to string Size")

        uint64 deltaLen = uint64(1) - (endIndex - beginIndex);
        String newString = String(string.mSize + deltaLen);

        std::memcpy(newString.mData, string.mData, beginIndex);
        std::memcpy(newString.mData + beginIndex + 1, string.mData + endIndex, string.mSize - endIndex);

        newString.mData[beginIndex] = c;

        return newString;
    }

    String String::replace(const String& string, uint64 beginIndex, uint64 endIndex, const char8* replaceString)
    {
        OTTO_ASSERT(endIndex >= beginIndex, "endIndex must be greater than or equal to beginIndex");
        OTTO_ASSERT(endIndex <= string.mSize, "endIndex must be less than or equal to string Size")

        uint64 len = std::strlen(replaceString);
        uint64 deltaLen = len - (endIndex - beginIndex);
        String newString = String(string.mSize + deltaLen);

        std::memcpy(newString.mData, string.mData, beginIndex);
        std::memcpy(newString.mData + beginIndex, replaceString, len);
        std::memcpy(newString.mData + beginIndex + len, string.mData + endIndex, string.mSize - endIndex);

        return newString;
    }

    String String::replace(const String& string, uint64 beginIndex, uint64 endIndex, const String& replaceString)
    {
        OTTO_ASSERT(endIndex >= beginIndex, "endIndex must be greater than or equal to beginIndex");
        OTTO_ASSERT(endIndex <= string.mSize, "endIndex must be less than or equal to string Size")

        uint64 deltaLen = replaceString.mSize - (endIndex - beginIndex);
        String newString = String(string.mSize + deltaLen);

        std::memcpy(newString.mData, string.mData, beginIndex);
        std::memcpy(newString.mData + beginIndex, replaceString.mData, replaceString.mSize);
        std::memcpy(newString.mData + beginIndex + replaceString.mSize, string.mData + endIndex, string.mSize - endIndex);

        return newString;
    }

    String String::replaceAll(const String& string, char8 oldChar, char8 newChar, uint64 startIndex)
    {
        String newString = String(string.mSize);

        for (uint64 i = 0; i < string.mSize; i++)
            newString[i] = string[i] == oldChar ? newChar : string[i];

        return newString;
    }

    String String::replaceAll(const String& string, const char8* oldString, const char8* newString, uint64 startIndex)
    {
        return String(string).replaceAll(oldString, newString, startIndex);
    }

    String String::replaceAll(const String& string, const String& oldString, const String& newString, uint64 startIndex)
    {
        return String(string).replaceAll(oldString, newString, startIndex);
    }

    String String::replaceAllIgnoreCase(const String& string, char8 oldChar, char8 newChar, uint64 startIndex)
    {
        String newString = String(string.mSize);

        for (uint64 i = 0; i < string.mSize; i++)
            newString[i] = _toLowerCase(string[i]) == _toLowerCase(oldChar) ? newChar : string[i];

        return newString;
    }

    String String::replaceAllIgnoreCase(const String& string, const char8* oldString, const char8* newString, uint64 startIndex)
    {
        return String(string).replaceAllIgnoreCase(oldString, newString, startIndex);
    }

    String String::replaceAllIgnoreCase(const String& string, const String& oldString, const String& newString, uint64 startIndex)
    {
        return String(string).replaceAllIgnoreCase(oldString, newString, startIndex);
    }

    String String::removeAll(const String& string, char8 c, uint64 startIndex)
    {
        char8* newData = new char8[string.mSize];

        uint64 replaced = 0;
        for (uint64 i = startIndex; i < string.mSize; i++)
        {
            if (string[i] != c)
                newData[i - replaced] = string[i];
            else
                replaced++;
        }

        String newString = String(newData, string.getSize() - replaced);
        delete[] newData;

        return newString;
    }

    String String::subString(const String& string, uint64 beginIndex)
    {
        OTTO_ASSERT(beginIndex <= string.mSize, "beginIndex must be less than size");

        return String(string.mData + beginIndex, string.mSize - beginIndex);
    }

    String String::subString(const String& string, uint64 beginIndex, uint64 endIndex)
    {
        OTTO_ASSERT(endIndex <= string.mSize, "endIndex must be less than size");
        OTTO_ASSERT(endIndex >= beginIndex, "endIndex must be >= beginIndex");

        return String(string.mData + beginIndex, endIndex - beginIndex);
    }

    String String::trim(const String& string)
    {
        uint64 beginIndex = string.findFirstNotOfWhiteSpace();

        if (beginIndex == string.mSize) // string is all whitespace
            return String();

        return subString(string, beginIndex, string.findLastNotOfWhiteSpace() + 1);
    }

    String String::lowerCase(const String& string)
    {
        String newString = String(string.mSize);

        for (uint64 i = 0; i < string.mSize; i++)
            newString[i] = _toLowerCase(string.mData[i]);

        return newString;
    }

    String String::upperCase(const String& string)
    {
        String newString = String(string.mSize);

        for (uint64 i = 0; i < string.mSize; i++)
            newString[i] = _toUpperCase(string.mData[i]);

        return newString;
    }

    char& String::get(uint64 index)
    {
        OTTO_ASSERT(index < mSize, "Index out of range");

        return mData[index];
    }

    const char& String::get(uint64 index) const
    {
        OTTO_ASSERT(index < mSize, "Index out of range");

        return mData[index];
    }

    String String::replaceFirst(const String& string, char8 oldChar, char8 newChar, uint64 startIndex)
    {
        uint64 index = string.findFirstOf(oldChar, startIndex);
        String newString = String(string);

        if (index != string.mSize)
            newString[index] = newChar;

        return newString;
    }

    String String::replaceFirst(const String& string, const char8* oldString, const char8* newString, uint64 startIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = string.findFirstOf(oldString, startIndex);
        uint64 deltaLen = newLen - oldLen;

        if (index == string.mSize)
            return NONE;

        String changedString = String(string.mSize + deltaLen);

        std::memcpy(changedString.mData, string.mData, index);
        std::memcpy(changedString.mData + index, newString, newLen);
        std::memcpy(changedString.mData + index + newLen, string.mData + index + oldLen, string.mSize - (index + oldLen));

        return changedString;
    }

    String String::replaceFirst(const String& string, const String& oldString, const String& newString, uint64 startIndex)
    {
        uint64 index = string.findFirstOf(oldString, startIndex);
        uint64 deltaLen = newString.mSize - oldString.mSize;

        if (index == string.mSize)
            return NONE;

        String changedString = String(string.mSize + deltaLen);

        std::memcpy(changedString.mData, string.mData, index);
        std::memcpy(changedString.mData + index, newString.mData, newString.mSize);
        std::memcpy(changedString.mData + index + newString.mSize, string.mData + index + oldString.mSize, string.mSize - (index + oldString.mSize));

        return changedString;
    }

    String String::replaceFirstIgnoreCase(const String& string, char8 oldChar, char8 newChar, uint64 startIndex)
    {
        uint64 index = string.findFirstOfIgnoreCase(oldChar, startIndex);
        String newString = String(string);

        if (index != string.mSize)
            newString[index] = newChar;

        return newString;
    }

    String String::replaceFirstIgnoreCase(const String& string, const char8* oldString, const char8* newString, uint64 startIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = string.findFirstOfIgnoreCase(oldString, startIndex);
        uint64 deltaLen = newLen - oldLen;

        if (index == string.mSize)
            return NONE;

        String changedString = String(string.mSize + deltaLen);

        std::memcpy(changedString.mData, string.mData, index);
        std::memcpy(changedString.mData + index, newString, newLen);
        std::memcpy(changedString.mData + index + newLen, string.mData + index + oldLen, string.mSize - (index + oldLen));

        return changedString;
    }

    String String::replaceFirstIgnoreCase(const String& string, const String& oldString, const String& newString, uint64 startIndex)
    {
        uint64 index = string.findFirstOfIgnoreCase(oldString, startIndex);
        uint64 deltaLen = newString.mSize - oldString.mSize;

        if (index == string.mSize)
            return NONE;

        String changedString = String(string.mSize + deltaLen);

        std::memcpy(changedString.mData, string.mData, index);
        std::memcpy(changedString.mData + index, newString.mData, newString.mSize);
        std::memcpy(changedString.mData + index + newString.mSize, string.mData + index + oldString.mSize, string.mSize - (index + oldString.mSize));

        return changedString;
    }

    String String::replaceLast(const String& string, char8 oldChar, char8 newChar, uint64 endIndex)
    {
        uint64 index = string.findLastOf(oldChar, endIndex);
        String newString = String(string);

        if (index != string.mSize)
            newString[index] = newChar;

        return newString;
    }

    String String::replaceLast(const String& string, const char8* oldString, const char8* newString, uint64 endIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = string.findLastOf(oldString, endIndex);
        uint64 deltaLen = newLen - oldLen;

        if (index == string.mSize)
            return NONE;

        String changedString = String(string.mSize + deltaLen);

        std::memcpy(changedString.mData, string.mData, index);
        std::memcpy(changedString.mData + index, newString, newLen);
        std::memcpy(changedString.mData + index + newLen, string.mData + index + oldLen, string.mSize - (index + oldLen));

        return changedString;
    }

    String String::replaceLast(const String& string, const String& oldString, const String& newString, uint64 endIndex)
    {
        uint64 index = string.findLastOf(oldString, endIndex);
        uint64 deltaLen = newString.mSize - oldString.mSize;

        if (index == string.mSize)
            return NONE;

        String changedString = String(string.mSize + deltaLen);

        std::memcpy(changedString.mData, string.mData, index);
        std::memcpy(changedString.mData + index, newString.mData, newString.mSize);
        std::memcpy(changedString.mData + index + newString.mSize, string.mData + index + oldString.mSize, string.mSize - (index + oldString.mSize));

        return changedString;
    }

    String String::replaceLastIgnoreCase(const String& string, char8 oldChar, char8 newChar, uint64 endIndex)
    {
        uint64 index = string.findLastOfIgnoreCase(oldChar, endIndex);
        String newString = String(string);

        if (index != string.mSize)
            newString[index] = newChar;

        return newString;
    }

    String String::replaceLastIgnoreCase(const String& string, const char8* oldString, const char8* newString, uint64 endIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = string.findLastOfIgnoreCase(oldString, endIndex);
        uint64 deltaLen = newLen - oldLen;

        if (index == string.mSize)
            return NONE;

        String changedString = String(string.mSize + deltaLen);

        std::memcpy(changedString.mData, string.mData, index);
        std::memcpy(changedString.mData + index, newString, newLen);
        std::memcpy(changedString.mData + index + newLen, string.mData + index + oldLen, string.mSize - (index + oldLen));

        return changedString;
    }

    String String::replaceLastIgnoreCase(const String& string, const String& oldString, const String& newString, uint64 endIndex)
    {
        uint64 index = string.findLastOfIgnoreCase(oldString, endIndex);
        uint64 deltaLen = newString.mSize - oldString.mSize;

        if (index == string.mSize)
            return NONE;

        String changedString = String(string.mSize + deltaLen);

        std::memcpy(changedString.mData, string.mData, index);
        std::memcpy(changedString.mData + index, newString.mData, newString.mSize);
        std::memcpy(changedString.mData + index + newString.mSize, string.mData + index + oldString.mSize, string.mSize - (index + oldString.mSize));

        return changedString;
    }

    String String::title(const String& string)
    {
        if (!string.isEmpty())
        {
            String newString = string;
            newString.mData[0] = _toUpperCase(string.mData[0]);
            return newString;
        }

        return NONE;
    }

    String String::untitle(const String& string)
    {
        if (!string.isEmpty())
        {
            String newString = string;
            newString.mData[0] = _toLowerCase(string.mData[0]);
            return newString;
        }

        return NONE;
    }

    String String::append(const String& string, char8 c)
    {
        String newString = String(string.mSize + 1);

        std::memcpy(newString.mData, string.mData, string.mSize);

        newString.mData[newString.mSize - 1] = c;

        return newString;
    }

    String String::append(const String& string, const char8* appendString)
    {
        uint64 appendStringLen = std::strlen(appendString);
        String newString = String(string.mSize + appendStringLen);

        std::memcpy(newString.mData, string.mData, string.mSize);
        std::memcpy(newString.mData + string.mSize, appendString, appendStringLen);

        return newString;
    }

    String String::append(const String& string, const String& appendString)
    {
        String newString = String(string.mSize + appendString.mSize);

        std::memcpy(newString.mData, string.mData, string.mSize);
        std::memcpy(newString.mData + string.mSize, appendString.mData, appendString.mSize);

        return newString;
    }

    String String::insert(const String& string, uint64 index, char8 c)
    {
        String newString = String(string.mSize + 1);

        std::memcpy(newString.mData, string.mData, index);
        std::memcpy(newString.mData + index + 1, string.mData + index, string.mSize - index);

        newString.mData[index] = c;

        return newString;
    }

    String String::insert(const String& string, uint64 index, const char8* insertString)
    {
        uint64 insertStringLen = std::strlen(insertString);
        String newString = String(string.mSize + insertStringLen);

        std::memcpy(newString.mData, string.mData, index);
        std::memcpy(newString.mData + index, insertString, insertStringLen);
        std::memcpy(newString.mData + index + insertStringLen, string.mData + index, string.mSize - index);

        return newString;
    }

    String String::insert(const String& string, uint64 index, const String& insertString)
    {
        String newString = String(string.mSize + insertString.mSize);

        std::memcpy(newString.mData, string.mData, index);
        std::memcpy(newString.mData + index, insertString.mData, insertString.mSize);
        std::memcpy(newString.mData + index + insertString.mSize, string.mData + index, string.mSize - index);

        return newString;
    }

    String String::findNextWord(const String& string, uint64 beginIndex)
    {
        uint64 firstIndex = string.findFirstNotOfWhiteSpace(beginIndex);
        return String::subString(string, firstIndex, string.findFirstOfWhiteSpace(firstIndex + 1));
    }

    uint64 String::_replaceFirst(char8 oldChar, char8 newChar, uint64 startIndex)
    {
        uint64 index = findFirstOf(oldChar, startIndex);

        if (index != mSize)
            mData[index] = newChar;

        return index + 1 < mSize ? index + 1 : mSize;
    }

    uint64 String::_replaceFirst(const char8* oldString, const char8* newString, uint64 startIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = findFirstOf(oldString, startIndex);

        if (index == mSize)
            return mSize;

        if (uint64 deltaLen = newLen - oldLen;
            deltaLen != uint64(0))
        {
            char8* newData = new char8[mSize + deltaLen + uint64(1)];

            std::memcpy(newData, mData, index);
            std::memcpy(newData + index, newString, newLen);
            std::memcpy(newData + index + newLen, mData + index + oldLen, mSize - (index + oldLen));

            delete[] mData;
            mData = newData;
            mSize += deltaLen;

            mData[mSize] = '\0';
        }
        else
        {
            std::memcpy(mData + index, newString, newLen);
        }

        auto i = index + newLen;
        return i < mSize ? i : mSize;
    }

    uint64 String::_replaceFirst(const String& oldString, const String& newString, uint64 startIndex)
    {
        uint64 index = findFirstOf(oldString, startIndex);

        if (index == mSize)
            return mSize;

        if (uint64 deltaLen = newString.mSize - oldString.mSize;
            deltaLen != uint64(0))
        {
            char8* newData = new char8[mSize + deltaLen + uint64(1)];

            std::memcpy(newData, mData, index);
            std::memcpy(newData + index, newString.mData, newString.mSize);
            std::memcpy(newData + index + newString.mSize, mData + index + oldString.mSize, mSize - (index + oldString.mSize));

            delete[] mData;
            mData = newData;
            mSize += deltaLen;

            mData[mSize] = '\0';
        }
        else
        {
            std::memcpy(mData + index, newString.mData, newString.mSize);
        }

        auto i = index + newString.mSize;
        return i < mSize ? i : mSize;
    }

    uint64 String::_replaceFirstIgnoreCase(char8 oldChar, char8 newChar, uint64 startIndex)
    {
        uint64 index = findFirstOfIgnoreCase(oldChar, startIndex);

        if (index != mSize)
            mData[index] = newChar;

        return index + 1 < mSize ? index + 1 : mSize;
    }

    uint64 String::_replaceFirstIgnoreCase(const char8* oldString, const char8* newString, uint64 startIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = findFirstOfIgnoreCase(oldString, startIndex);

        if (index == mSize)
            return mSize;

        if (uint64 deltaLen = newLen - oldLen;
            deltaLen != uint64(0))
        {
            char8* newData = new char8[mSize + deltaLen + uint64(1)];

            std::memcpy(newData, mData, index);
            std::memcpy(newData + index, newString, newLen);
            std::memcpy(newData + index + newLen, mData + index + oldLen, mSize - (index + oldLen));

            delete[] mData;
            mData = newData;
            mSize += deltaLen;

            mData[mSize] = '\0';
        }
        else
        {
            std::memcpy(mData + index, newString, newLen);
        }

        auto i = index + newLen;
        return i < mSize ? i : mSize;
    }

    uint64 String::_replaceFirstIgnoreCase(const String& oldString, const String& newString, uint64 startIndex)
    {
        uint64 index = findFirstOfIgnoreCase(oldString, startIndex);

        if (index == mSize)
            return mSize;

        if (uint64 deltaLen = newString.mSize - oldString.mSize;
            deltaLen != uint64(0))
        {
            char8* newData = new char8[mSize + deltaLen + uint64(1)];

            std::memcpy(newData, mData, index);
            std::memcpy(newData + index, newString.mData, newString.mSize);
            std::memcpy(newData + index + newString.mSize, mData + index + oldString.mSize, mSize - (index + oldString.mSize));

            delete[] mData;
            mData = newData;
            mSize += deltaLen;

            mData[mSize] = '\0';
        }
        else
        {
            std::memcpy(mData + index, newString.mData, newString.mSize);
        }

        auto i = index + newString.mSize;
        return i < mSize ? i : mSize;
    }

    String String::repeat(const String& string, uint64 amount)
    {
        String result = String(string.getSize() * amount);

        for (uint64 i = 0; i < amount; i++)
            std::memcpy(result.getData() + i * string.getSize(), string.getData(), string.getSize());

        return result;
    }

    String String::valueOf(bool8 value)
    {
        return value ? String("true", 4) : String("false", 5);
    }

    String String::valueOf(char8 value, int32 base)
    {
        return _integerToString<char8>(value, base);
    }

    String String::valueOf(uchar8 value, int32 base)
    {
        return _integerToString<uchar8>(value, base);
    }

    String String::valueOf(short value, int32 base)
    {
        return _integerToString<short>(value, base);
    }

    String String::valueOf(uint16 value, int32 base)
    {
        return _integerToString<uint16>(value, base);
    }

    String String::valueOf(int32 value, int32 base)
    {
        return _integerToString<int>(value, base);
    }

    String String::valueOf(uint32 value, int32 base)
    {
        return _integerToString<uint32>(value, base);
    }

    String String::valueOf(int64 value, int32 base)
    {
        return _integerToString<int64>(value, base);
    }

    String String::valueOf(uint64 value, int32 base)
    {
        return _integerToString<uint64>(value, base);
    }

    String String::valueOf(float32 value, bool8 scientific)
    {
        return _floatToString<float32>(value, scientific);
    }

    String String::valueOf(float64 value, bool8 scientific)
    {
        return _floatToString<float64>(value, scientific);
    }

    template<typename T>
    T String::stringTo(const String& string)
    {
        static_assert(false);
    }

    template<>
    bool8 String::stringTo<bool8>(const String& string)
    {
        return string == "true" || string == '1';
    }

    template<>
    char8 String::stringTo<char8>(const String& string)
    {
        return static_cast<char8>(std::atoi(string.getData()));
    }

    template<>
    uchar8 String::stringTo<uchar8>(const String& string)
    {
        return static_cast<uchar8>(std::atoi(string.getData()));
    }

    template<>
    int16 String::stringTo<int16>(const String& string)
    {
        return static_cast<short>(std::atoi(string.getData()));
    }

    template<>
    uint16 String::stringTo<uint16>(const String& string)
    {
        return static_cast<uint16>(std::atoi(string.getData()));
    }

    template<>
    int32 String::stringTo<int32>(const String& string)
    {
        return std::atoi(string.getData());
    }

    template<>
    uint32 String::stringTo<uint32>(const String& string)
    {
        return static_cast<uint32>(std::atol(string.getData()));
    }

    template<>
    int64 String::stringTo<int64>(const String& string)
    {
        return std::atol(string.getData());
    }

    template<>
    uint64 String::stringTo<uint64>(const String& string)
    {
        return static_cast<uint64>(std::atoll(string.getData()));
    }

    template<>
    float32 String::stringTo<float32>(const String& string)
    {
        return static_cast<float32>(std::atof(string.getData()));
    }

    template<>
    float64 String::stringTo<float64>(const String& string)
    {
        return std::atof(string.getData());
    }

    std::ostream& operator<<(std::ostream& stream, const String& string)
    {
        stream << string.getData();
        return stream;
    }

    bool8 operator<<(String& string, std::istream& file)
    {
        uint64 posBefore = file.tellg();

        file.seekg(0, file.end);
        uint64 fileSize = file.tellg();

        if (fileSize == -1)
            return false;

        file.seekg(0, file.beg);
        string.setSize(fileSize);
        file.read(string.getData(), fileSize);

        file.seekg(posBefore, file.beg);

        return true;
    }

    bool8 operator<<(String& string, std::istream&& file)
    {
        uint64 posBefore = file.tellg();

        file.seekg(0, file.end);
        uint64 fileSize = file.tellg();

        if (fileSize == -1)
            return false;

        file.seekg(0, file.beg);
        string.setSize(fileSize);
        file.read(string.getData(), fileSize);

        file.seekg(posBefore, file.beg);

        return true;
    }

    String operator+(const char8* string1, const String& string2)
    {
        uint64 len = std::strlen(string1);
        String string = String(len + string2.getSize());

        std::memcpy(string.getData(), string1, len);
        std::memcpy(string.getData() + len, string2.getData(), string2.getSize());

        return string;
    }

    String operator+(const String& string1, const char8* string2)
    {
        uint64 len = std::strlen(string2);
        String string = String(string1.getSize() + len);

        std::memcpy(string.getData(), string1.getData(), string1.getSize());
        std::memcpy(string.getData() + string1.getSize(), string2, len);

        return string;
    }

    String operator+(const String& string1, const String& string2)
    {
        String string = String(string1.getSize() + string2.getSize());

        std::memcpy(string.getData(), string1.getData(), string1.getSize());
        std::memcpy(string.getData() + string1.getSize(), string2.getData(), string2.getSize());

        return string;
    }

    String operator+(const String& string1, char8 c)
    {
        String string = String(string1.getSize() + 1);

        std::memcpy(string.getData(), string1.getData(), string1.getSize());
        string[string1.getSize()] = c;

        return string;
    }

    String operator+(char8 c, const String& string1)
    {
        String string = String(string1.getSize() + 1);

        string[0] = c;
        std::memcpy(string.getData() + 1, string1.getData(), string1.getSize());

        return string;
    }

} // namespace otto
