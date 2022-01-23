#pragma once

#include <limits>
#include <ostream>

#include "otto/base.h"
#include "otto/util/dynamic_array.h"

#ifdef max
#undef max
#endif

namespace otto
{
    class String
    {
    public:
        constexpr String() noexcept;

        constexpr String(char c);

        String(const char* data);

        explicit String(const char* data, uint64 size);

        explicit String(uint64 size);

        String(const String& other);

        String(String&& other) noexcept;

        ~String();

        String& operator=(char c);
        String& operator=(const char* other);
        String& operator=(const String& other);

        String& operator=(String&& other) noexcept;

        char& operator[](uint64 index) { return get(index); }
        const char& operator[](uint64 index) const { return get(index); }

        void operator+=(char c) { append(c); }
        void operator+=(const String& String) { append(String); }
        void operator+=(const char* String) { append(String); }

        bool operator==(char c) const { return equals(c); }
        bool operator==(const String& other) const { return equals(other); }
        bool operator==(const char* other) const { return equals(other); }

        bool operator!=(char c) const { return !equals(c); }
        bool operator!=(const String& other) const { return !equals(other); }
        bool operator!=(const char* other) const { return !equals(other); }

        bool equals(char c) const;
        bool equals(const String& other) const;
        bool equals(const char* other) const;

        bool equalsIgnoreCase(char c) const;
        bool equalsIgnoreCase(const String& other) const;
        bool equalsIgnoreCase(const char* other) const;

        bool startsWith(char c) const;
        bool startsWith(const char* String) const;
        bool startsWith(const String& String) const;

        bool startsWithIgnoreCase(char c) const;
        bool startsWithIgnoreCase(const char* String) const;
        bool startsWithIgnoreCase(const String& String) const;

        bool endsWith(char c) const;
        bool endsWith(const char* String) const;
        bool endsWith(const String& String) const;

        bool endsWithIgnoreCase(char c) const;
        bool endsWithIgnoreCase(const char* String) const;
        bool endsWithIgnoreCase(const String& String) const;

        bool contains(char c) const;
        bool contains(const String& String) const;
        bool contains(const char* String) const;

        bool containsIgnoreCase(char c) const;
        bool containsIgnoreCase(const String& String) const;
        bool containsIgnoreCase(const char* String) const;

        String& append(char c);
        String& append(const String& String);
        String& append(const char* String);

        String& insert(uint64 index, char c);
        String& insert(uint64 index, const String& String);
        String& insert(uint64 index, const char* String);

        String& trim();

        String& toLowerCase();
        String& toUpperCase();

        String& toSubString(uint64 beginIndex);
        String& toSubString(uint64 beginIndex, uint64 endIndex);

        char* begin() { return mData; }
        const char* begin() const { return mData; }

        char* end() { return mData + mSize; }
        const char* end() const { return mData + mSize; }

        void setSize(uint64 size);

        uint64 getSize() const { return mSize; }

        bool isEmpty() const { return mSize == 0; }

        char& get(uint64 index);
        const char& get(uint64 index) const;

        char* getData() { return mData; }
        const char* getData() const { return mData; }

        String& replace(uint64 beginIndex, uint64 endIndex, char c);
        String& replace(uint64 beginIndex, uint64 endIndex, const char* String);
        String& replace(uint64 beginIndex, uint64 endIndex, const String& String);

        String& replaceAll(char oldChar, char newChar, uint64 startIndex = 0);
        String& replaceAll(const char* oldString, const char* newString, uint64 startIndex = 0);
        String& replaceAll(const String& oldString, const String& newString, uint64 startIndex = 0);

        String& replaceAllIgnoreCase(char oldChar, char newChar, uint64 startIndex = 0);
        String& replaceAllIgnoreCase(const char* oldString, const char* newString, uint64 startIndex = 0);
        String& replaceAllIgnoreCase(const String& oldString, const String& newString, uint64 startIndex = 0);

        String& replaceFirst(char oldChar, char newChar, uint64 startIndex = 0);
        String& replaceFirst(const char* oldString, const char* newString, uint64 startIndex = 0);
        String& replaceFirst(const String& oldString, const String& newString, uint64 startIndex = 0);

        String& replaceFirstIgnoreCase(char oldChar, char newChar, uint64 startIndex = 0);
        String& replaceFirstIgnoreCase(const char* oldString, const char* newString, uint64 startIndex = 0);
        String& replaceFirstIgnoreCase(const String& oldString, const String& newString, uint64 startIndex = 0);

        String& replaceLast(char oldChar, char newChar, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& replaceLast(const char* oldString, const char* newString, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& replaceLast(const String& oldString, const String& newString, uint64 endIndex = std::numeric_limits<uint64>::max());

        String& replaceLastIgnoreCase(char oldChar, char newChar, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& replaceLastIgnoreCase(const char* oldString, const char* newString, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& replaceLastIgnoreCase(const String& oldString, const String& newString, uint64 endIndex = std::numeric_limits<uint64>::max());

        uint64 findFirstOf(char c, uint64 startIndex = 0) const;
        uint64 findFirstOf(const char* string, uint64 startIndex = 0) const;
        uint64 findFirstOf(const String& string, uint64 startIndex = 0) const;

        uint64 findFirstOfIgnoreCase(char c, uint64 startIndex = 0) const;
        uint64 findFirstOfIgnoreCase(const char* string, uint64 startIndex = 0) const;
        uint64 findFirstOfIgnoreCase(const String& string, uint64 startIndex = 0) const;

        uint64 findLastOf(char c, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastOf(const char* string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastOf(const String& string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;

        uint64 findLastOfIgnoreCase(char c, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastOfIgnoreCase(const char* string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastOfIgnoreCase(const String& string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;

        uint64 findFirstNotOf(char c, uint64 startIndex = 0) const;
        uint64 findFirstNotOf(const char* string, uint64 startIndex = 0) const;
        uint64 findFirstNotOf(const String& string, uint64 startIndex = 0) const;

        uint64 findFirstNotOfIgnoreCase(char c, uint64 startIndex = 0) const;
        uint64 findFirstNotOfIgnoreCase(const char* string, uint64 startIndex = 0) const;
        uint64 findFirstNotOfIgnoreCase(const String& string, uint64 startIndex = 0) const;

        uint64 findLastNotOf(char c, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastNotOf(const char* string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastNotOf(const String& string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;

        uint64 findLastNotOfIgnoreCase(char c, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastNotOfIgnoreCase(const char* string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastNotOfIgnoreCase(const String& string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;

        uint64 findFirstOfWhiteSpace(uint64 startIndex = 0) const;
        uint64 findFirstNotOfWhiteSpace(uint64 startIndex = 0) const;
        uint64 findLastOfWhiteSpace(uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastNotOfWhiteSpace(uint64 endIndex = std::numeric_limits<uint64>::max()) const;

        static String trim(const String& String);
        static String lowerCase(const String& String);
        static String upperCase(const String& String);

        static String subString(const String& string, uint64 beginIndex);
        static String subString(const String& string, uint64 beginIndex, uint64 endIndex);

        static DynamicArray<String> split(const String& string, char c);
        static DynamicArray<String> split(const String& string, const char* splitString);
        static DynamicArray<String> split(const String& string, const String& splitString);

        static DynamicArray<String> splitIgnoreCase(const String& string, char c);
        static DynamicArray<String> splitIgnoreCase(const String& string, const char* splitString);
        static DynamicArray<String> splitIgnoreCase(const String& string, const String& splitString);

        static String replace(const String& string, uint64 beginIndex, uint64 endIndex, char c);
        static String replace(const String& string, uint64 beginIndex, uint64 endIndex, const char* replaceString);
        static String replace(const String& string, uint64 beginIndex, uint64 endIndex, const String& replaceString);

        static String replaceAll(const String& string, char oldChar, char newChar, uint64 startIndex = 0);
        static String replaceAll(const String& string, const char* oldString, const char* newString, uint64 startIndex = 0);
        static String replaceAll(const String& string, const String& oldString, const String& newString, uint64 startIndex = 0);

        static String replaceAllIgnoreCase(const String& string, char oldChar, char newChar, uint64 startIndex = 0);
        static String replaceAllIgnoreCase(const String& string, const char* oldString, const char* newString, uint64 startIndex = 0);
        static String replaceAllIgnoreCase(const String& string, const String& oldString, const String& newString, uint64 startIndex = 0);

        static String replaceFirst(const String& string, char oldChar, char newChar, uint64 startIndex = 0);
        static String replaceFirst(const String& string, const char* oldString, const char* newString, uint64 startIndex = 0);
        static String replaceFirst(const String& string, const String& oldString, const String& newString, uint64 startIndex = 0);

        static String replaceFirstIgnoreCase(const String& string, char oldChar, char newChar, uint64 startIndex = 0);
        static String replaceFirstIgnoreCase(const String& string, const char* oldString, const char* newString, uint64 startIndex = 0);
        static String replaceFirstIgnoreCase(const String& string, const String& oldString, const String& newString, uint64 startIndex = 0);

        static String replaceLast(const String& string, char oldChar, char newChar, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String replaceLast(const String& string, const char* oldString, const char* newString, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String replaceLast(const String& string, const String& oldString, const String& newString, uint64 endIndex = std::numeric_limits<uint64>::max());

        static String replaceLastIgnoreCase(const String& string, char oldChar, char newChar, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String replaceLastIgnoreCase(const String& string, const char* oldString, const char* newString, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String replaceLastIgnoreCase(const String& string, const String& oldString, const String& newString, uint64 endIndex = std::numeric_limits<uint64>::max());

        static String findNextWord(const String& string, uint64 beginIndex = 0);

        static String append(const String& string, char c);
        static String append(const String& string, const char* appendString);
        static String append(const String& string, const String& appendString);

        static String insert(const String& string, uint64 index, char c);
        static String insert(const String& string, uint64 index, const char* insertString);
        static String insert(const String& string, uint64 index, const String& insertString);

        static String valueOf(bool b);
        static String valueOf(char c, int base = 10);
        static String valueOf(unsigned char c, int base = 10);
        static String valueOf(short s, int base = 10);
        static String valueOf(unsigned short s, int base = 10);
        static String valueOf(int i, int base = 10);
        static String valueOf(unsigned int i, int base = 10);
        static String valueOf(long l, int base = 10);
        static String valueOf(unsigned long l, int base = 10);
        static String valueOf(long long l, int base = 10);
        static String valueOf(unsigned long long l, int base = 10);
        static String valueOf(float f, bool scientific = false);
        static String valueOf(double f, bool scientific = false);
        static String valueOf(long double d, bool scientific = false);

        static String toString(const String& s) { return s; }

        template<typename T> requires std::is_integral<T>::value
        static String toString(T t)
        {
            return String::valueOf(t);
        }

        template<typename T>
        static String toString(const T& t)
        {
            return t.toString();
        }

        template<typename T>
        static T stringTo(const String& string);

        template<typename T>
        T to() const
        {
            return String::stringTo<T>(*this);
        }

    private:
        uint64 _replaceFirst(char oldChar, char newChar, uint64 startIndex = 0);
        uint64 _replaceFirst(const char* oldString, const char* newString, uint64 startIndex = 0);
        uint64 _replaceFirst(const String& oldString, const String& newString, uint64 startIndex = 0);

        uint64 _replaceFirstIgnoreCase(char oldChar, char newChar, uint64 startIndex = 0);
        uint64 _replaceFirstIgnoreCase(const char* oldString, const char* newString, uint64 startIndex = 0);
        uint64 _replaceFirstIgnoreCase(const String& oldString, const String& newString, uint64 startIndex = 0);

        uint64 mSize;
        char* mData;
    };

    bool operator<<(String& string, std::istream& file);
    bool operator<<(String& string, std::istream&& file);
    std::ostream& operator<<(std::ostream& stream, const String& String);

    String operator+(char c, const String& string1);
    String operator+(const char* string1, const String& string2);
    String operator+(const String& string1, char c);
    String operator+(const String& string1, const char* string2);
    String operator+(const String& string1, const String& string2);

} // namespace otto
