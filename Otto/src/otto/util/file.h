#pragma once

#include <fstream>

#include "otto/base.h"
#include "otto/util/pair.h"
#include "otto/util/string.h"
#include "otto/util/dynamic_array.h"
#include "otto/util/platform/file_path.h"

namespace otto
{
    class File
    {
    public:
        File(const FilePath& filePath);

        String read() const;

        DynamicArray<String> readLines() const;

        void write(const String& text);

    private:
        FilePath mFilePath;
    };

    class BinaryFile
    {
    public:
        enum class Mode
        {
            CLOSED,
            READ,
            WRITE,
        };

        BinaryFile(const FilePath& filePath)
            : mFilePath(filePath)
        {
        }

        ~BinaryFile();

        void setMode(Mode mode);
        Mode getMode() const { return mMode; }

        void moveCursor(uint64 offset);

        bool close();

        void read(uint8* dst, uint64 size);
        void readCompressed(uint8* dst, uint64 size);

        void write(const uint8* data, uint64 size);
        void writeCompressed(const uint8* data, uint64 originalSize);

        template<typename T>
        T read()
        {
            T t;
            read(reinterpret_cast<uint8*>(&t), sizeof(T));

            return t;
        }
            
        template<typename T>
        T readCompressed()
        {
            T t;
            readCompressed(reinterpret_cast<uint8*>(&t), sizeof(T));

            return t;
        }

        template<typename T>
        void write(const T& t)
        {
            write(reinterpret_cast<const uint8*>(&t), sizeof(T));
        }

        template<typename T>
        void write(const T* t, uint64 size)
        {
            write(reinterpret_cast<const uint8*>(t), size);
        }

        template<typename T>
        void writeCompressed(const T& t)
        {
            writeCompressed(reinterpret_cast<uint8*>(&t), sizeof(T));
        }

        template<typename T>
        void writeCompressed(const T* t, uint64 size)
        {
            writeCompressed(reinterpret_cast<uint8*>(t), size);
        }

    private:
        Mode mMode = Mode::CLOSED;
        FilePath mFilePath;

        std::ifstream mInputStream;
        std::ofstream mOutputStream;
    };

} // namespace otto
