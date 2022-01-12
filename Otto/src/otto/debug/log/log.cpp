#include "log.h"

#include <fstream>

#include "otto/base.h"

#include "otto/util/platform/file_utils.h"

namespace otto
{
    Log* Log::sInstance;

    Log::~Log()
    {
        //platform::createDirectoryRecursively(String::subString(mFilePath, 0, mFilePath.replaceAll('/', '\\').findLastOf('\\') + 1));
        platform::createDirectoryRecursively(mFilePath.getParentDirectory());

        std::ofstream file = std::ofstream(mFilePath.toString().getData(), std::ios::out);
        file << mFileStream.str();
        file.close();
    }

    void Log::init(const FilePath& filePath, std::ostream& ostream, uint8 consoleLevel, uint8 fileLevel)
    {
        static Log instance;
        sInstance = &instance;

        sInstance->mFilePath = filePath;
        sInstance->mFileLevel = fileLevel;
        sInstance->mConsoleLevel = consoleLevel;
        sInstance->mFileStream = std::ostringstream();
        sInstance->mConsoleStream = &ostream;
    }

    void Log::init(Log* instance)
    {
        OTTO_ASSERT(instance != nullptr, "Cant initialize Log without another valid Log instance");

        sInstance = instance;
    }

} // namespace otto
