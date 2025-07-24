#pragma once

#include <fstream>
#include <mutex>
#include <Windows.h>
#include <winrt/Windows.Storage.h>
#include "impl/GameLogger.h"
#include "impl/LogFile.h"
#include "LogBox.hpp"

namespace Logger {

    enum class LogLevel {
        Debug,
        Info,
        Warn,
        Error
    };

    constexpr const char *logLevelToString(LogLevel level) {
        switch (level) {
        case LogLevel::Debug: return "[DEBUG]";
        default:
        case LogLevel::Info: return "[INFO] ";
        case LogLevel::Warn: return "[WARN] ";
        case LogLevel::Error: return "[ERROR]";
        }
    }

    class Loggers {
        GameLogger mGameLogger;
        LogFile    mFileLogger;

        Loggers() = default;

        ~Loggers() = default;

    public:
        SDK_API static Loggers &getInstance();

        GameLogger &getGameLogger() {
            return mGameLogger;
        }

        template <typename... Args>
        void log(LogLevel level, std::format_string<Args...> fmt, Args &&...args) {
            std::string formattedMessage = std::format(fmt, std::forward<Args>(args)...);
            std::string logEntry = std::format("{} {}\n", logLevelToString(level), formattedMessage);
            mGameLogger.log(logEntry);
            mFileLogger.log(logEntry, level != LogLevel::Debug);
        }

        void flush() {
            mFileLogger.flush();
        }
    };

    template <typename... Args>
    inline void Debug(std::format_string<Args...> fmt, Args &&...args) {
#ifdef SPHR_DEBUG
        Loggers::getInstance().log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
#endif
    }

    template <typename... Args>
    inline void Info(std::format_string<Args...> fmt, Args &&...args) {
        Loggers::getInstance().log(LogLevel::Info, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void Warn(std::format_string<Args...> fmt, Args &&...args) {
        Loggers::getInstance().log(LogLevel::Warn, fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inline void Error(std::format_string<Args...> fmt, Args &&...args) {
        Loggers::getInstance().log(LogLevel::Error, fmt, std::forward<Args>(args)...);
    }

} // namespace Logger
