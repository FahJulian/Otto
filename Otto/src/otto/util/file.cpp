#include "file.h"

#include <fstream>
#include <sstream>

namespace otto
{
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

} // namespace otto
