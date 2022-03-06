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

        constexpr String(char8 c);

        String(const char8* data);

        explicit String(const char8* data, uint64 size);

        explicit String(uint64 size);

        String(const String& other);

        String(String&& other) noexcept;

        ~String();

        String& operator=(char8 c);
        String& operator=(const char8* other);
        String& operator=(const String& other);

        String& operator=(String&& other) noexcept;

        char& operator[](uint64 index) { return get(index); }
        const char& operator[](uint64 index) const { return get(index); }

        void operator+=(char8 c) { append(c); }
        void operator+=(const String& String) { append(String); }
        void operator+=(const char8* String) { append(String); }

        bool8 operator==(char8 c) const { return equals(c); }
        bool8 operator==(const String& other) const { return equals(other); }
        bool8 operator==(const char8* other) const { return equals(other); }

        bool8 operator!=(char8 c) const { return !equals(c); }
        bool8 operator!=(const String& other) const { return !equals(other); }
        bool8 operator!=(const char8* other) const { return !equals(other); }

        bool8 equals(char8 c) const;
        bool8 equals(const String& other) const;
        bool8 equals(const char8* other) const;

        bool8 equalsIgnoreCase(char8 c) const;
        bool8 equalsIgnoreCase(const String& other) const;
        bool8 equalsIgnoreCase(const char8* other) const;

        bool8 startsWith(char8 c) const;
        bool8 startsWith(const char8* String) const;
        bool8 startsWith(const String& String) const;

        bool8 startsWithIgnoreCase(char8 c) const;
        bool8 startsWithIgnoreCase(const char8* String) const;
        bool8 startsWithIgnoreCase(const String& String) const;

        bool8 endsWith(char8 c) const;
        bool8 endsWith(const char8* String) const;
        bool8 endsWith(const String& String) const;

        bool8 endsWithIgnoreCase(char8 c) const;
        bool8 endsWithIgnoreCase(const char8* String) const;
        bool8 endsWithIgnoreCase(const String& String) const;

        bool8 contains(char8 c) const;
        bool8 contains(const String& String) const;
        bool8 contains(const char8* String) const;

        bool8 containsIgnoreCase(char8 c) const;
        bool8 containsIgnoreCase(const String& String) const;
        bool8 containsIgnoreCase(const char8* String) const;

        String& append(char8 c);
        String& append(const String& String);
        String& append(const char8* String);

        String& insert(uint64 index, char8 c);
        String& insert(uint64 index, const String& String);
        String& insert(uint64 index, const char8* String);

        String& trim();

        String& toLowerCase();
        String& toUpperCase();

        String& toSubString(uint64 beginIndex);
        String& toSubString(uint64 beginIndex, uint64 endIndex);

        char8* begin() { return mData; }
        const char8* begin() const { return mData; }

        char8* end() { return mData + mSize; }
        const char8* end() const { return mData + mSize; }

        void setSize(uint64 size);

        uint64 getSize() const { return mSize; }

        bool8 isEmpty() const { return mSize == 0; }

        char& get(uint64 index);
        const char& get(uint64 index) const;

        char8* getData() { return mData; }
        const char8* getData() const { return mData; }

        String& replace(uint64 beginIndex, uint64 endIndex, char8 c);
        String& replace(uint64 beginIndex, uint64 endIndex, const char8* String);
        String& replace(uint64 beginIndex, uint64 endIndex, const String& String);

        String& replaceAll(char8 oldChar, char8 newChar, uint64 startIndex = 0);
        String& replaceAll(const char8* oldString, const char8* newString, uint64 startIndex = 0);
        String& replaceAll(const String& oldString, const String& newString, uint64 startIndex = 0);

        String& replaceAllIgnoreCase(char8 oldChar, char8 newChar, uint64 startIndex = 0);
        String& replaceAllIgnoreCase(const char8* oldString, const char8* newString, uint64 startIndex = 0);
        String& replaceAllIgnoreCase(const String& oldString, const String& newString, uint64 startIndex = 0);

        String& replaceFirst(char8 oldChar, char8 newChar, uint64 startIndex = 0);
        String& replaceFirst(const char8* oldString, const char8* newString, uint64 startIndex = 0);
        String& replaceFirst(const String& oldString, const String& newString, uint64 startIndex = 0);

        String& replaceFirstIgnoreCase(char8 oldChar, char8 newChar, uint64 startIndex = 0);
        String& replaceFirstIgnoreCase(const char8* oldString, const char8* newString, uint64 startIndex = 0);
        String& replaceFirstIgnoreCase(const String& oldString, const String& newString, uint64 startIndex = 0);

        String& replaceLast(char8 oldChar, char8 newChar, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& replaceLast(const char8* oldString, const char8* newString, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& replaceLast(const String& oldString, const String& newString, uint64 endIndex = std::numeric_limits<uint64>::max());

        String& replaceLastIgnoreCase(char8 oldChar, char8 newChar, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& replaceLastIgnoreCase(const char8* oldString, const char8* newString, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& replaceLastIgnoreCase(const String& oldString, const String& newString, uint64 endIndex = std::numeric_limits<uint64>::max());

        String& removeAll(char8 c, uint64 startIndex = 0);
        String& removeAll(const char8* string, uint64 startIndex = 0);
        String& removeAll(const String& string, uint64 startIndex = 0);

        String& removeAllIgnoreCase(char8 c, uint64 startIndex = 0);
        String& removeAllIgnoreCase(const char8* string, uint64 startIndex = 0);
        String& removeAllIgnoreCase(const String& string, uint64 startIndex = 0);

        String& removeFirst(char8 c, uint64 startIndex = 0);
        String& removeFirst(const char8* string, uint64 startIndex = 0);
        String& removeFirst(const String& string, uint64 startIndex = 0);

        String& removeFirstIgnoreCase(char8 c, uint64 startIndex = 0);
        String& removeFirstIgnoreCase(const char8* string, uint64 startIndex = 0);
        String& removeFirstIgnoreCase(const String& string, uint64 startIndex = 0);

        String& removeLast(char8 c, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& removeLast(const char8* string, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& removeLast(const String& string, uint64 endIndex = std::numeric_limits<uint64>::max());

        String& removeLastIgnoreCase(char8 c, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& removeLastIgnoreCase(const char8* string, uint64 endIndex = std::numeric_limits<uint64>::max());
        String& removeLastIgnoreCase(const String& string, uint64 endIndex = std::numeric_limits<uint64>::max());

        String& title();
        String& untitle();

        uint64 findFirstOf(char8 c, uint64 startIndex = 0) const;
        uint64 findFirstOf(const char8* string, uint64 startIndex = 0) const;
        uint64 findFirstOf(const String& string, uint64 startIndex = 0) const;

        uint64 findFirstOfIgnoreCase(char8 c, uint64 startIndex = 0) const;
        uint64 findFirstOfIgnoreCase(const char8* string, uint64 startIndex = 0) const;
        uint64 findFirstOfIgnoreCase(const String& string, uint64 startIndex = 0) const;

        uint64 findLastOf(char8 c, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastOf(const char8* string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastOf(const String& string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;

        uint64 findLastOfIgnoreCase(char8 c, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastOfIgnoreCase(const char8* string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastOfIgnoreCase(const String& string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;

        uint64 findFirstNotOf(char8 c, uint64 startIndex = 0) const;
        uint64 findFirstNotOf(const char8* string, uint64 startIndex = 0) const;
        uint64 findFirstNotOf(const String& string, uint64 startIndex = 0) const;

        uint64 findFirstNotOfIgnoreCase(char8 c, uint64 startIndex = 0) const;
        uint64 findFirstNotOfIgnoreCase(const char8* string, uint64 startIndex = 0) const;
        uint64 findFirstNotOfIgnoreCase(const String& string, uint64 startIndex = 0) const;

        uint64 findLastNotOf(char8 c, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastNotOf(const char8* string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastNotOf(const String& string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;

        uint64 findLastNotOfIgnoreCase(char8 c, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
        uint64 findLastNotOfIgnoreCase(const char8* string, uint64 endIndex = std::numeric_limits<uint64>::max()) const;
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

        static DynamicArray<String> split(const String& string, char8 c);
        static DynamicArray<String> split(const String& string, const char8* splitString);
        static DynamicArray<String> split(const String& string, const String& splitString);

        static DynamicArray<String> splitIgnoreCase(const String& string, char8 c);
        static DynamicArray<String> splitIgnoreCase(const String& string, const char8* splitString);
        static DynamicArray<String> splitIgnoreCase(const String& string, const String& splitString);

        static String replace(const String& string, uint64 beginIndex, uint64 endIndex, char8 c);
        static String replace(const String& string, uint64 beginIndex, uint64 endIndex, const char8* replaceString);
        static String replace(const String& string, uint64 beginIndex, uint64 endIndex, const String& replaceString);

        static String replaceAll(const String& string, char8 oldChar, char8 newChar, uint64 startIndex = 0);
        static String replaceAll(const String& string, const char8* oldString, const char8* newString, uint64 startIndex = 0);
        static String replaceAll(const String& string, const String& oldString, const String& newString, uint64 startIndex = 0);

        static String replaceAllIgnoreCase(const String& string, char8 oldChar, char8 newChar, uint64 startIndex = 0);
        static String replaceAllIgnoreCase(const String& string, const char8* oldString, const char8* newString, uint64 startIndex = 0);
        static String replaceAllIgnoreCase(const String& string, const String& oldString, const String& newString, uint64 startIndex = 0);

        static String replaceFirst(const String& string, char8 oldChar, char8 newChar, uint64 startIndex = 0);
        static String replaceFirst(const String& string, const char8* oldString, const char8* newString, uint64 startIndex = 0);
        static String replaceFirst(const String& string, const String& oldString, const String& newString, uint64 startIndex = 0);

        static String replaceFirstIgnoreCase(const String& string, char8 oldChar, char8 newChar, uint64 startIndex = 0);
        static String replaceFirstIgnoreCase(const String& string, const char8* oldString, const char8* newString, uint64 startIndex = 0);
        static String replaceFirstIgnoreCase(const String& string, const String& oldString, const String& newString, uint64 startIndex = 0);

        static String replaceLast(const String& string, char8 oldChar, char8 newChar, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String replaceLast(const String& string, const char8* oldString, const char8* newString, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String replaceLast(const String& string, const String& oldString, const String& newString, uint64 endIndex = std::numeric_limits<uint64>::max());

        static String replaceLastIgnoreCase(const String& string, char8 oldChar, char8 newChar, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String replaceLastIgnoreCase(const String& string, const char8* oldString, const char8* newString, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String replaceLastIgnoreCase(const String& string, const String& oldString, const String& newString, uint64 endIndex = std::numeric_limits<uint64>::max());

        static String removeAll(const String& string, char8 c, uint64 startIndex = 0);
        static String removeAll(const String& string, const char8* removeString, uint64 startIndex = 0);
        static String removeAll(const String& string, const String& removeString, uint64 startIndex = 0);

        static String removeAllIgnoreCase(const String& string, char8 c, uint64 startIndex = 0);
        static String removeAllIgnoreCase(const String& string, const char8* removeString, uint64 startIndex = 0);
        static String removeAllIgnoreCase(const String& string, const String& removeString, uint64 startIndex = 0);

        static String removeFirst(const String& string, char8 c, uint64 startIndex = 0);
        static String removeFirst(const String& string, const char8* removeString, uint64 startIndex = 0);
        static String removeFirst(const String& string, const String& removeString, uint64 startIndex = 0);

        static String removeFirstIgnoreCase(const String& string, char8 c, uint64 startIndex = 0);
        static String removeFirstIgnoreCase(const String& string, const char8* removeString, uint64 startIndex = 0);
        static String removeFirstIgnoreCase(const String& string, const String& removeString, uint64 startIndex = 0);

        static String removeLast(const String& string, char8 c, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String removeLast(const String& string, const char8* removeString, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String removeLast(const String& string, const String& removeString, uint64 endIndex = std::numeric_limits<uint64>::max());

        static String removeLastIgnoreCase(const String& string, char8 c, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String removeLastIgnoreCase(const String& string, const char8* removeString, uint64 endIndex = std::numeric_limits<uint64>::max());
        static String removeLastIgnoreCase(const String& string, const String& removeString, uint64 endIndex = std::numeric_limits<uint64>::max());

        static String repeat(const String& string, uint64 amount);

        static String title(const String& string);
        static String untitle(const String& string);

        static String findNextWord(const String& string, uint64 beginIndex = 0);

        static String append(const String& string, char8 c);
        static String append(const String& string, const char8* appendString);
        static String append(const String& string, const String& appendString);

        static String insert(const String& string, uint64 index, char8 c);
        static String insert(const String& string, uint64 index, const char8* insertString);
        static String insert(const String& string, uint64 index, const String& insertString);

        static String valueOf(bool8 b);
        static String valueOf(char8 c, int32 base = 10);
        static String valueOf(uchar8 c, int32 base = 10);
        static String valueOf(int16 s, int32 base = 10);
        static String valueOf(uint16 s, int32 base = 10);
        static String valueOf(int32 i, int32 base = 10);
        static String valueOf(uint32 i, int32 base = 10);
        static String valueOf(int64 l, int32 base = 10);
        static String valueOf(uint64 l, int32 base = 10);
        static String valueOf(float32 f, bool8 scientific = false);
        static String valueOf(float64 f, bool8 scientific = false);

        static String toString(const String& s) { return s; }

        template<typename T> requires isIntegral<T>
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
        uint64 _replaceFirst(char8 oldChar, char8 newChar, uint64 startIndex = 0);
        uint64 _replaceFirst(const char8* oldString, const char8* newString, uint64 startIndex = 0);
        uint64 _replaceFirst(const String& oldString, const String& newString, uint64 startIndex = 0);

        uint64 _replaceFirstIgnoreCase(char8 oldChar, char8 newChar, uint64 startIndex = 0);
        uint64 _replaceFirstIgnoreCase(const char8* oldString, const char8* newString, uint64 startIndex = 0);
        uint64 _replaceFirstIgnoreCase(const String& oldString, const String& newString, uint64 startIndex = 0);

        uint64 mSize;
        char8* mData;
    };

    bool8 operator<<(String& string, std::istream& file);
    bool8 operator<<(String& string, std::istream&& file);

    std::ostream& operator<<(std::ostream& stream, const String& string);

    String operator+(char8 c, const String& string1);
    String operator+(const char8* string1, const String& string2);
    String operator+(const String& string1, char8 c);
    String operator+(const String& string1, const char8* string2);
    String operator+(const String& string1, const String& string2);

    template<typename T>
    concept hasToString = requires (const T & t)
    {
        t.toString();
    };

    template<typename T> requires hasToString<T>
    std::ostream& operator<<(std::ostream& stream, const T& t)
    {
        return (stream << String::toString(t));
    }

} // namespace otto
