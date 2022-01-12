#pragma once

#include <ostream>
#include <sstream>

#include "otto/util/string.h"
#include "otto/util/platform/file_path.h"

#ifdef ERROR    // Windows macro -> Conflict with Loglevel ERROR
#undef ERROR
#endif

namespace otto
{
    class Log
    {
    private:
        struct Level
        {
            operator uint8() const { return severity; }

            const uint8 severity;
            const char* const color;
            const char* const name;
        };

        static constexpr const char* const ANSI_RESET = "\033[0m";

    public:
        static constexpr const Level& FATAL = { 5, "\x1B[1m\x1B[41m", "[FATAL] " };
        static constexpr const Level& ERROR = { 4, "\x1B[1m\x1B[31m", "[ERROR] " };
        static constexpr const Level& WARN = { 3, "\x1B[1m\x1B[33m", "[WARN]  " };
        static constexpr const Level& INFO = { 2, "\x1B[1m\x1B[36m", "[INFO]  " };
        static constexpr const Level& DEBUG = { 1, "\x1B[1m\x1B[35m", "[DEBUG] " };
        static constexpr const Level& TRACE = { 0, "\x1B[1m\x1B[37m", "[TRACE] " };

        static constexpr uint8 DISABLED = 0xff;
        static constexpr uint8 ALL = 0;

        static void init(const FilePath& filePath, std::ostream& ostream, uint8 consoleLevel, uint8 fileLevel);
        static void init(Log* instance);

        template<typename Arg, typename... Args>
        static void log(const Level& level, const Arg& arg, const Args&... args)
        {
            if (level.severity >= sInstance->mConsoleLevel)
            {
                *sInstance->mConsoleStream << level.color;
                _pushMessage(*sInstance->mConsoleStream, level, arg, (args)...);
                *sInstance->mConsoleStream << ANSI_RESET;
            }

            if (level.severity >= sInstance->mFileLevel)
                _pushMessage(sInstance->mFileStream, level, arg, (args)...);
        }

        template<typename... Args>
        static inline void fatal(const Args&... args)
        {
            log<Args...>(FATAL, (args)...);
        }

        template<typename... Args>
        static inline void error(const Args&... args)
        {
            log<Args...>(ERROR, (args)...);
        }

        template<typename... Args>
        static inline void warn(const Args&... args)
        {
            log<Args...>(WARN, (args)...);
        }

        template<typename... Args>
        static inline void info(const Args&... args)
        {
            log<Args...>(INFO, (args)...);
        }

        template<typename... Args>
        static inline void debug(const Args&... args)
        {
            log<Args...>(DEBUG, (args)...);
        }

        template<typename... Args>
        static inline void trace(const Args&... args)
        {
            log<Args...>(TRACE, (args)...);
        }

        static Log* getInstance() { return sInstance; }

    private:
        Log()
            : mFilePath(), mConsoleLevel(Log::DISABLED), mFileLevel(Log::DISABLED), mConsoleStream(nullptr), mFileStream()
        {
        }

        ~Log();

        template<typename Arg, typename... Args>
        inline static void _pushMessage(std::ostream& stream, const Level& level, const Arg& arg, const Args&... args)
        {
            stream << level.name;
            stream << arg;
            ((stream << args), ...);
            stream << std::endl;
        }

        FilePath mFilePath;
        uint8 mConsoleLevel;
        uint8 mFileLevel;
        std::ostream* mConsoleStream;
        std::ostringstream mFileStream;

        static Log* sInstance;
    };

} // namespace otto
