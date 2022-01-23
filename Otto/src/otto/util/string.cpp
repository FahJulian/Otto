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
        String _floatToString(T value, bool scientific)
        {
            auto result = std::to_chars(STRING_CAST_BUFFER.begin(), STRING_CAST_BUFFER.end(),
                value, scientific ? std::chars_format::scientific : std::chars_format::fixed, 2);

            return String(STRING_CAST_BUFFER.begin(), result.ptr - STRING_CAST_BUFFER.begin());
        }

        template<typename T>
        String _integerToString(T value, int base)
        {
            auto result = std::to_chars(STRING_CAST_BUFFER.begin(), STRING_CAST_BUFFER.end(), value, base);
            return String(STRING_CAST_BUFFER.begin(), result.ptr - STRING_CAST_BUFFER.begin());
        }

        char _toLowerCase(char c)
        {
            return (c >= 'A' && c <= 'Z') ? c - 'A' + 'a' : c;
        }

        char _toUpperCase(char c)
        {
            return (c >= 'a' && c <= 'z') ? c - 'a' + 'A' : c;
        }

        bool _equalsIgnoreCase(const char* c1, const char* c2, uint64 len)
        {
            for (uint64 i = 0; i < len; i++)
            {
                if (_toLowerCase(c1[i]) != _toLowerCase(c2[i]))
                    return false;
            }

            return true;
        }

        bool _isWhitespace(char c)
        {
            return c <= ' ';
        }

    } // namespace

    constexpr String::String(char c)
        : mSize(uint64(1)), mData(new char[2] { c, '\0' })
    {
    }

    constexpr String::String() noexcept
        : mSize(0), mData(new char('\0'))
    {
    }

    String::String(const char* data)
        : mSize(std::strlen(data)), mData(new char[mSize + 1])
    {
        std::memcpy(mData, data, mSize);
        mData[mSize] = '\0';
    }

    String::String(const char* data, uint64 size)
        : mSize(size), mData(new char[mSize + 1])
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
        : mSize(other.mSize), mData(new char[mSize + 1])
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

    String& String::operator=(char c)
    {
        if (mSize != 1)
        {
            delete[] mData;

            mData = new char[1]{ c };
            mSize = 1;
        }
        else
        {
            *mData = c;
        }

        return *this;
    }

    String& String::operator=(const char* other)
    {
        uint64 len = std::strlen(other);

        if (mSize != len)
        {
            delete[] mData;

            mData = new char[len + 1];
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
            mData = new char[mSize + 1];

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

    String& String::append(char c)
    {
        char* newData = new char[mSize + 2];
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
        char* newData = new char[mSize + string.mSize + 1];
        std::memcpy(newData, mData, mSize);
        std::memcpy(newData + mSize, string.mData, string.mSize);

        mSize += string.mSize;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;

        return *this;
    }

    String& String::append(const char* string)
    {
        uint64 len = std::strlen(string);

        char* newData = new char[mSize + len + 1];
        std::memcpy(newData, mData, mSize);
        std::memcpy(newData + mSize, string, len);

        mSize += len;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;

        return *this;
    }

    String& String::insert(uint64 index, char c)
    {
        OTTO_ASSERT(index <= mSize, "Index is out of bounds");

        char* newData = new char[mSize + 2];
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

        char* newData = new char[mSize + string.mSize + 1];
        std::memcpy(newData, mData, index);
        std::memcpy(newData + index, string.mData, string.mSize);
        std::memcpy(newData + index + string.mSize, mData + index, mSize - index);

        mSize += string.mSize;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;

        return *this;
    }

    String& String::insert(uint64 index, const char* string)
    {
        OTTO_ASSERT(index <= mSize, "Index is out of bounds");

        uint64 len = std::strlen(string);

        char* newData = new char[mSize + len + 1];
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
        for (char* c = mData; c < mData + mSize; c++)
            *c = _toLowerCase(*c);

        return *this;
    }

    String& String::toUpperCase()
    {
        for (char* c = mData; c < mData + mSize; c++)
            *c = _toUpperCase(*c);

        return *this;
    }

    String& String::toSubString(uint64 beginIndex)
    {
        OTTO_ASSERT(beginIndex <= mSize, "beginIndex must be less than size");

        if (beginIndex != 0)
        {
            mSize = mSize - beginIndex;

            char* newData = new char[mSize + 1];

            std::memcpy(newData, mData, mSize);
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

            char* newData = new char[mSize + 1];

            std::memcpy(newData, mData + beginIndex, mSize);
            newData[mSize] = '\0';

            delete[] mData;
            mData = newData;
        }

        return *this;
    }

    void String::setSize(uint64 size)
    {
        char* newData = new char[size + 1];
        std::memcpy(newData, mData, mSize);

        mSize = size;
        newData[mSize] = '\0';

        delete[] mData;
        mData = newData;
    }

    bool String::equals(char c) const
    {
        return mSize == 1 && *mData == c;
    }

    bool String::equals(const String& other) const
    {
        return std::strcmp(mData, other.mData) == 0;
    }

    bool String::equals(const char* other) const
    {
        return std::strcmp(mData, other) == 0;
    }

    bool String::equalsIgnoreCase(char c) const
    {
        return mSize == 1 && _toLowerCase(*mData) == _toLowerCase(c);
    }

    bool String::equalsIgnoreCase(const String& other) const
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

    bool String::equalsIgnoreCase(const char* other) const
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

    bool String::startsWith(char c) const
    {
        if (isEmpty())
            return false;

        return mData[0] == c;
    }

    bool String::startsWith(const char* string) const
    {
        uint64 len = std::strlen(string);

        if (len > mSize)
            return false;

        return std::strncmp(mData, string, len) == 0;
    }

    bool String::startsWith(const String& string) const
    {
        if (string.mSize > mSize)
            return false;

        return std::strncmp(mData, string.mData, string.mSize) == 0;
    }

    bool String::startsWithIgnoreCase(char c) const
    {
        if (isEmpty())
            return false;

        return _toLowerCase(mData[0]) == _toLowerCase(c);
    }

    bool String::startsWithIgnoreCase(const char* string) const
    {
        uint64 len = std::strlen(string);

        if (len > mSize)
            return false;

        return _equalsIgnoreCase(mData, string, len);
    }

    bool String::startsWithIgnoreCase(const String& string) const
    {
        if (string.mSize > mSize)
            return false;

        return _equalsIgnoreCase(mData, string.mData, string.mSize);
    }

    bool String::endsWith(char c) const
    {
        if (isEmpty())
            return false;

        return mData[mSize - 1] == c;
    }

    bool String::endsWith(const char* string) const
    {
        uint64 len = std::strlen(string);

        if (len > mSize)
            return false;

        return std::strncmp(mData + mSize - len, string, len) == 0;
    }

    bool String::endsWith(const String& string) const
    {
        if (string.mSize > mSize)
            return false;

        return std::strncmp(mData + mSize - string.mSize, string.mData, string.mSize) == 0;
    }

    bool String::endsWithIgnoreCase(char c) const
    {
        if (isEmpty())
            return false;

        return _toLowerCase(mData[mSize - 1]) == _toLowerCase(c);
    }

    bool String::endsWithIgnoreCase(const char* string) const
    {
        uint64 len = std::strlen(string);

        if (len > mSize)
            return false;

        return _equalsIgnoreCase(mData + mSize - len, string, len);
    }

    bool String::endsWithIgnoreCase(const String& string) const
    {
        if (string.mSize > mSize)
            return false;

        return _equalsIgnoreCase(mData + mSize - string.mSize, string.mData, string.mSize);
    }

    bool String::contains(char c) const
    {
        return findFirstOf(c) != mSize;
    }

    bool String::contains(const String& string) const
    {
        return findFirstOf(string) != mSize;
    }

    bool String::contains(const char* string) const
    {
        return findFirstOf(string) != mSize;
    }

    bool String::containsIgnoreCase(char c) const
    {
        return findFirstOfIgnoreCase(c) != mSize;
    }

    bool String::containsIgnoreCase(const String& string) const
    {
        return findFirstOfIgnoreCase(string) != mSize;
    }

    bool String::containsIgnoreCase(const char* string) const
    {
        return findFirstOfIgnoreCase(string) != mSize;
    }

    String& String::replace(uint64 beginIndex, uint64 endIndex, char c)
    {
        OTTO_ASSERT(endIndex >= beginIndex, "endIndex must be greater than or equal to beginIndex");
        OTTO_ASSERT(endIndex <= mSize, "endIndex must be less than or equal to string Size")

        uint64 deltaLen = static_cast<uint64>(1) - (endIndex - beginIndex);

        if (deltaLen != 0)
        {
            char* newData = new char[mSize + 1 + deltaLen];
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

    String& String::replace(uint64 beginIndex, uint64 endIndex, const char* string)
    {
        OTTO_ASSERT(endIndex >= beginIndex, "endIndex must be greater than or equal to beginIndex");
        OTTO_ASSERT(endIndex <= mSize, "endIndex must be less than or equal to string Size")

        uint64 len = std::strlen(string);

        uint64 deltaLen = len - (endIndex - beginIndex);

        if (deltaLen != static_cast<uint64>(0))
        {
            char* newData = new char[mSize + 1 + deltaLen];
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
            char* newData = new char[mSize + 1 + deltaLen];
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

    String& String::replaceAll(char oldChar, char newChar, uint64 startIndex)
    {
        while ((startIndex = _replaceFirst(oldChar, newChar, startIndex)) != mSize)
            ;

        return *this;
    }

    String& String::replaceAll(const char* oldString, const char* newString, uint64 startIndex)
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

    String& String::replaceAllIgnoreCase(char oldChar, char newChar, uint64 startIndex)
    {
        while ((startIndex = _replaceFirstIgnoreCase(oldChar, newChar, startIndex)) != mSize)
            ;

        return *this;
    }

    String& String::replaceAllIgnoreCase(const char* oldString, const char* newString, uint64 startIndex)
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

    String& String::replaceFirst(char oldChar, char newChar, uint64 startIndex)
    {
        _replaceFirst(oldChar, newChar, startIndex);
        return *this;
    }

    String& String::replaceFirst(const char* oldString, const char* newString, uint64 startIndex)
    {
        _replaceFirst(oldString, newString, startIndex);
        return *this;
    }

    String& String::replaceFirst(const String& oldString, const String& newString, uint64 startIndex)
    {
        _replaceFirst(oldString, newString, startIndex);
        return *this;
    }

    String& String::replaceFirstIgnoreCase(char oldChar, char newChar, uint64 startIndex)
    {
        _replaceFirstIgnoreCase(oldChar, newChar, startIndex);
        return *this;
    }

    String& String::replaceFirstIgnoreCase(const char* oldString, const char* newString, uint64 startIndex)
    {
        _replaceFirstIgnoreCase(oldString, newString, startIndex);
        return *this;
    }

    String& String::replaceFirstIgnoreCase(const String& oldString, const String& newString, uint64 startIndex)
    {
        _replaceFirstIgnoreCase(oldString, newString, startIndex);
        return *this;
    }

    String& String::replaceLast(char oldChar, char newChar, uint64 endIndex)
    {
        uint64 index = findLastOf(oldChar, endIndex);

        if (index != mSize)
            mData[index] = newChar;

        return *this;
    }

    String& String::replaceLast(const char* oldString, const char* newString, uint64 endIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = findLastOf(oldString, endIndex);

        if (index == mSize)
            return *this;

        if (int deltaLen = static_cast<int>(newLen - oldLen);
            deltaLen != 0)
        {
            char* newData = new char[mSize + deltaLen + 1];

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

        if (int deltaLen = static_cast<int>(newString.mSize - oldString.mSize);
            deltaLen != 0)
        {
            char* newData = new char[mSize + deltaLen + 1];

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

    String& String::replaceLastIgnoreCase(char oldChar, char newChar, uint64 endIndex)
    {
        uint64 index = findLastOfIgnoreCase(oldChar, endIndex);

        if (index != mSize)
            mData[index] = newChar;

        return *this;
    }

    String& String::replaceLastIgnoreCase(const char* oldString, const char* newString, uint64 endIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = findLastOfIgnoreCase(oldString, endIndex);

        if (index == mSize)
            return *this;

        if (int deltaLen = static_cast<int>(newLen - oldLen);
            deltaLen != 0)
        {
            char* newData = new char[mSize + deltaLen + 1];

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

        if (int deltaLen = static_cast<int>(newString.mSize - oldString.mSize);
            deltaLen != 0)
        {
            char* newData = new char[mSize + deltaLen + 1];

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

    uint64 String::findFirstOf(char c, uint64 startIndex) const
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

    uint64 String::findFirstOf(const char* string, uint64 startIndex) const
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

    uint64 String::findFirstOfIgnoreCase(char c, uint64 startIndex) const
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

    uint64 String::findFirstOfIgnoreCase(const char* string, uint64 startIndex) const
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

    uint64 String::findLastOf(char c, uint64 endIndex) const
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

    uint64 String::findLastOf(const char* string, uint64 endIndex) const
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

    uint64 String::findLastOfIgnoreCase(char c, uint64 endIndex) const
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

    uint64 String::findLastOfIgnoreCase(const char* string, uint64 endIndex) const
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

    uint64 String::findFirstNotOf(char c, uint64 startIndex) const
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

    uint64 String::findFirstNotOf(const char* string, uint64 startIndex) const
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

    uint64 String::findFirstNotOfIgnoreCase(char c, uint64 startIndex) const
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

    uint64 String::findFirstNotOfIgnoreCase(const char* string, uint64 startIndex) const
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

    uint64 String::findLastNotOf(char c, uint64 endIndex) const
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

    uint64 String::findLastNotOf(const char* string, uint64 endIndex) const
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

    uint64 String::findLastNotOfIgnoreCase(char c, uint64 endIndex) const
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

    uint64 String::findLastNotOfIgnoreCase(const char* string, uint64 endIndex) const
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


    DynamicArray<String> String::split(const String& string, char c)
    {
        if (string.isEmpty())
            return { };

        DynamicArray<String> parts;

        const char* nextPartStart = string.mData;
        for (const char* i = string.mData; i < string.mData + string.mSize; i++)
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

    DynamicArray<String> String::split(const String& string, const char* splitString)
    {
        uint64 len = std::strlen(splitString);
        if (len > string.mSize)
            return { };

        DynamicArray<String> parts;

        const char* i = string.mData;
        const char* nextPartStart = string.mData;
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

        const char* i = string.mData;
        const char* nextPartStart = string.mData;
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

    DynamicArray<String> String::splitIgnoreCase(const String& string, char c)
    {
        if (string.isEmpty())
            return { };

        DynamicArray<String> parts;

        c = _toLowerCase(c);

        const char* nextPartStart = string.mData;
        for (const char* i = string.mData; i < string.mData + string.mSize; i++)
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

    DynamicArray<String> String::splitIgnoreCase(const String& string, const char* splitString)
    {
        uint64 len = std::strlen(splitString);
        if (len >= string.mSize)
            return { };

        DynamicArray<String> parts;

        const char* i = string.mData;
        const char* nextPartStart = string.mData;
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

        const char* i = string.mData;
        const char* nextPartStart = string.mData;
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

    String String::replace(const String& string, uint64 beginIndex, uint64 endIndex, char c)
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

    String String::replace(const String& string, uint64 beginIndex, uint64 endIndex, const char* replaceString)
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

    String String::replaceAll(const String& string, char oldChar, char newChar, uint64 startIndex)
    {
        String newString = String(string.mSize);

        for (uint64 i = 0; i < string.mSize; i++)
            newString[i] = string[i] == oldChar ? newChar : string[i];

        return newString;
    }

    String String::replaceAll(const String& string, const char* oldString, const char* newString, uint64 startIndex)
    {
        return String(string).replaceAll(oldString, newString, startIndex);
    }

    String String::replaceAll(const String& string, const String& oldString, const String& newString, uint64 startIndex)
    {
        return String(string).replaceAll(oldString, newString, startIndex);
    }

    String String::replaceAllIgnoreCase(const String& string, char oldChar, char newChar, uint64 startIndex)
    {
        String newString = String(string.mSize);

        for (uint64 i = 0; i < string.mSize; i++)
            newString[i] = _toLowerCase(string[i]) == _toLowerCase(oldChar) ? newChar : string[i];

        return newString;
    }

    String String::replaceAllIgnoreCase(const String& string, const char* oldString, const char* newString, uint64 startIndex)
    {
        return String(string).replaceAllIgnoreCase(oldString, newString, startIndex);
    }

    String String::replaceAllIgnoreCase(const String& string, const String& oldString, const String& newString, uint64 startIndex)
    {
        return String(string).replaceAllIgnoreCase(oldString, newString, startIndex);
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

    String String::replaceFirst(const String& string, char oldChar, char newChar, uint64 startIndex)
    {
        uint64 index = string.findFirstOf(oldChar, startIndex);
        String newString = String(string);

        if (index != string.mSize)
            newString[index] = newChar;

        return newString;
    }

    String String::replaceFirst(const String& string, const char* oldString, const char* newString, uint64 startIndex)
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

    String String::replaceFirstIgnoreCase(const String& string, char oldChar, char newChar, uint64 startIndex)
    {
        uint64 index = string.findFirstOfIgnoreCase(oldChar, startIndex);
        String newString = String(string);

        if (index != string.mSize)
            newString[index] = newChar;

        return newString;
    }

    String String::replaceFirstIgnoreCase(const String& string, const char* oldString, const char* newString, uint64 startIndex)
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

    String String::replaceLast(const String& string, char oldChar, char newChar, uint64 endIndex)
    {
        uint64 index = string.findLastOf(oldChar, endIndex);
        String newString = String(string);

        if (index != string.mSize)
            newString[index] = newChar;

        return newString;
    }

    String String::replaceLast(const String& string, const char* oldString, const char* newString, uint64 endIndex)
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

    String String::replaceLastIgnoreCase(const String& string, char oldChar, char newChar, uint64 endIndex)
    {
        uint64 index = string.findLastOfIgnoreCase(oldChar, endIndex);
        String newString = String(string);

        if (index != string.mSize)
            newString[index] = newChar;

        return newString;
    }

    String String::replaceLastIgnoreCase(const String& string, const char* oldString, const char* newString, uint64 endIndex)
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

    String String::append(const String& string, char c)
    {
        String newString = String(string.mSize + 1);

        std::memcpy(newString.mData, string.mData, string.mSize);

        newString.mData[newString.mSize - 1] = c;

        return newString;
    }

    String String::append(const String& string, const char* appendString)
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

    String String::insert(const String& string, uint64 index, char c)
    {
        String newString = String(string.mSize + 1);

        std::memcpy(newString.mData, string.mData, index);
        std::memcpy(newString.mData + index + 1, string.mData + index, string.mSize - index);

        newString.mData[index] = c;

        return newString;
    }

    String String::insert(const String& string, uint64 index, const char* insertString)
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

    uint64 String::_replaceFirst(char oldChar, char newChar, uint64 startIndex)
    {
        uint64 index = findFirstOf(oldChar, startIndex);

        if (index != mSize)
            mData[index] = newChar;

        return index;
    }

    uint64 String::_replaceFirst(const char* oldString, const char* newString, uint64 startIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = findFirstOf(oldString, startIndex);

        if (index == mSize)
            return mSize;

        if (uint64 deltaLen = newLen - oldLen;
            deltaLen != uint64(0))
        {
            char* newData = new char[mSize + deltaLen + uint64(1)];

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

        return index;
    }

    uint64 String::_replaceFirst(const String& oldString, const String& newString, uint64 startIndex)
    {
        uint64 index = findFirstOf(oldString, startIndex);

        if (index == mSize)
            return mSize;

        if (uint64 deltaLen = newString.mSize - oldString.mSize;
            deltaLen != uint64(0))
        {
            char* newData = new char[mSize + deltaLen + uint64(1)];

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

        return index;
    }

    uint64 String::_replaceFirstIgnoreCase(char oldChar, char newChar, uint64 startIndex)
    {
        uint64 index = findFirstOfIgnoreCase(oldChar, startIndex);

        if (index != mSize)
            mData[index] = newChar;

        return index;
    }

    uint64 String::_replaceFirstIgnoreCase(const char* oldString, const char* newString, uint64 startIndex)
    {
        uint64 oldLen = std::strlen(oldString);
        uint64 newLen = std::strlen(newString);

        uint64 index = findFirstOfIgnoreCase(oldString, startIndex);

        if (index == mSize)
            return mSize;

        if (uint64 deltaLen = newLen - oldLen;
            deltaLen != uint64(0))
        {
            char* newData = new char[mSize + deltaLen + uint64(1)];

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

        return index;
    }

    uint64 String::_replaceFirstIgnoreCase(const String& oldString, const String& newString, uint64 startIndex)
    {
        uint64 index = findFirstOfIgnoreCase(oldString, startIndex);

        if (index == mSize)
            return mSize;

        if (uint64 deltaLen = newString.mSize - oldString.mSize;
            deltaLen != uint64(0))
        {
            char* newData = new char[mSize + deltaLen + uint64(1)];

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

        return index;
    }

    String String::valueOf(bool value)
    {
        return value ? String("true", 4) : String("false", 5);
    }

    String String::valueOf(char value, int base)
    {
        return _integerToString<char>(value, base);
    }

    String String::valueOf(unsigned char value, int base)
    {
        return _integerToString<unsigned char>(value, base);
    }

    String String::valueOf(short value, int base)
    {
        return _integerToString<short>(value, base);
    }

    String String::valueOf(unsigned short value, int base)
    {
        return _integerToString<unsigned short>(value, base);
    }

    String String::valueOf(int value, int base)
    {
        return _integerToString<int>(value, base);
    }

    String String::valueOf(unsigned int value, int base)
    {
        return _integerToString<unsigned int>(value, base);
    }

    String String::valueOf(long value, int base)
    {
        return _integerToString<long>(value, base);
    }

    String String::valueOf(unsigned long value, int base)
    {
        return _integerToString<unsigned long>(value, base);
    }

    String String::valueOf(long long value, int base)
    {
        return _integerToString<long long>(value, base);
    }

    String String::valueOf(unsigned long long value, int base)
    {
        return _integerToString<unsigned long long>(value, base);
    }

    String String::valueOf(float value, bool scientific)
    {
        return _floatToString<float>(value, scientific);
    }

    String String::valueOf(double value, bool scientific)
    {
        return _floatToString<double>(value, scientific);
    }

    String String::valueOf(long double value, bool scientific)
    {
        return _floatToString<long double>(value, scientific);
    }

    template<typename T>
    T String::stringTo(const String& string)
    {
        static_assert(false);
    }

    template<>
    bool String::stringTo<bool>(const String& string)
    {
        return string == "true" || string == 1;
    }

    template<>
    char String::stringTo<char>(const String& string)
    {
        return static_cast<char>(std::atoi(string.getData()));
    }

    template<>
    unsigned char String::stringTo<unsigned char>(const String& string)
    {
        return static_cast<unsigned char>(std::atoi(string.getData()));
    }

    template<>
    short String::stringTo<short>(const String& string)
    {
        return static_cast<short>(std::atoi(string.getData()));
    }

    template<>
    unsigned short String::stringTo<unsigned short>(const String& string)
    {
        return static_cast<unsigned short>(std::atoi(string.getData()));
    }

    template<>
    int String::stringTo<int>(const String& string)
    {
        return std::atoi(string.getData());
    }

    template<>
    unsigned int String::stringTo<unsigned int>(const String& string)
    {
        return static_cast<unsigned int>(std::atol(string.getData()));
    }

    template<>
    long String::stringTo<long>(const String& string)
    {
        return std::atol(string.getData());
    }

    template<>
    unsigned long String::stringTo<unsigned long>(const String& string)
    {
        return static_cast<unsigned long>(std::atoll(string.getData()));
    }

    template<>
    long long String::stringTo<long long>(const String& string)
    {
        return std::atoll(string.getData());
    }

    template<>
    unsigned long long String::stringTo<unsigned long long>(const String& string)
    {
        return std::stoull(string.getData());
    }

    template<>
    float String::stringTo<float>(const String& string)
    {
        return static_cast<float>(std::atof(string.getData()));
    }

    template<>
    double String::stringTo<double>(const String& string)
    {
        return std::atof(string.getData());
    }

    template<>
    long double String::stringTo<long double>(const String& string)
    {
        return static_cast<long double>(std::atof(string.getData())); // In MSVC double and long double are identical
    }

    std::ostream& operator<<(std::ostream& stream, const String& string)
    {
        stream << string.getData();
        return stream;
    }

    bool operator<<(String& string, std::istream& file)
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

    bool operator<<(String& string, std::istream&& file)
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

    String operator+(const char* string1, const String& string2)
    {
        uint64 len = std::strlen(string1);
        String string = String(len + string2.getSize());

        std::memcpy(string.getData(), string1, len);
        std::memcpy(string.getData() + len, string2.getData(), string2.getSize());

        return string;
    }

    String operator+(const String& string1, const char* string2)
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

    String operator+(const String& string1, char c)
    {
        String string = String(string1.getSize() + 1);

        std::memcpy(string.getData(), string1.getData(), string1.getSize());
        string[string1.getSize()] = c;

        return string;
    }

    String operator+(char c, const String& string1)
    {
        String string = String(string1.getSize() + 1);

        string[0] = c;
        std::memcpy(string.getData() + 1, string1.getData(), string1.getSize());

        return string;
    }

} // namespace otto
