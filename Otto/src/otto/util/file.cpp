#include "file.h"

#include <sstream>

#include <zstd/zstd.h>

namespace otto
{
    const uint32 COMPRESSION_LEVEL = 1;

    File::File(const FilePath& filePath)
        : mFilePath(filePath)
    {
    }

    String File::read() const
    {
        std::ifstream stream = std::ifstream(mFilePath.toString().getData());

        std::stringstream buffer;
        buffer << stream.rdbuf();

        buffer.seekg(0, buffer.end);
        uint64 size = buffer.tellg();

        String content = String(size);

        buffer.seekg(std::ifstream::beg);
        buffer.read(content.getData(), size);

        stream.close();
        return content;
    }

    void File::write(const String& text)
    {
        std::ofstream stream = std::ofstream(mFilePath.toString().getData());

        stream << text.getData();

        stream.close();
    }

    DynamicArray<String> File::readLines() const
    {
        return String::split(read(), '\n');
    }

    BinaryFile::~BinaryFile()
    {
        close();
    }

    void BinaryFile::moveCursor(uint64 offset)
    {
        if (mMode == Mode::READ)
            mInputStream.seekg(offset, std::ios::cur);
    }

    bool BinaryFile::close()
    {
        if (mMode == Mode::WRITE)
        {
            mOutputStream.close();
            mMode = Mode::CLOSED;

            return mOutputStream.good();
        }
        else if (mMode == Mode::READ)
        {
            mInputStream.close();
            mMode = Mode::CLOSED;

            return mInputStream.good();
        }
        else
            return true;
    }

    void BinaryFile::read(uint8* dst, uint64 size)
    {
        if (mMode != Mode::READ)
        {
            if (mMode == Mode::CLOSED)
            {
                mInputStream = std::ifstream(mFilePath.toString().getData(), std::ios::binary | std::ios::in);
                mMode = Mode::READ;
            }
            else
                return;
        }

        mInputStream.read(reinterpret_cast<char*>(dst), size);
    }

    void BinaryFile::readCompressed(uint8* dst, uint64 originalSize)
    {
        if (mMode != Mode::READ)
        {
            if (mMode == Mode::CLOSED)
            {
                mInputStream = std::ifstream(mFilePath.toString().getData(), std::ios::binary | std::ios::in);
                mMode = Mode::READ;
            }
            else
                return;
        }

        uint64 compressedSize = static_cast<uint64>(read<uint32>());
        uint8* compressedData = new uint8[compressedSize];

        read(compressedData, compressedSize);
        ZSTD_decompress(dst, originalSize, compressedData, compressedSize);

        delete[] compressedData;
    }

    void BinaryFile::write(uint8* data, uint64 size)
    {
        if (mMode != Mode::WRITE)
        {
            if (mMode == Mode::CLOSED)
            {
                mOutputStream = std::ofstream(mFilePath.toString().getData(), std::ios::binary | std::ios::out);
                mMode = Mode::WRITE;
            }
            else
                return;
        }

        mOutputStream.write(reinterpret_cast<char*>(data), size);
    }

    void BinaryFile::writeCompressed(uint8* data, uint64 size)
    {
        uint64 maxCompressedSize = ZSTD_compressBound(size);
        uint8* compressedData = new uint8[maxCompressedSize];

        uint64 compressedSize = ZSTD_compress(compressedData, maxCompressedSize, data, size, COMPRESSION_LEVEL);   

        write<uint32>(static_cast<uint32>(compressedSize));
        write(compressedData, compressedSize);

        delete[] compressedData;
    }

} // namespace otto
