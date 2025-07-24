#pragma once

#include <fstream>
#include <string>
#include <filesystem>
#include <chrono>
#include <format>
#include <Windows.h>

namespace Logger {

    class LogFile {
        friend class Loggers;

        std::wofstream mLogFileStream;

        LogFile();

        ~LogFile();

    public:
        void log(std::string_view msg, bool flush) {
            if (mLogFileStream.is_open()) {
                mLogFileStream << msg.data();
                if (flush)
                    mLogFileStream.flush();
            }
        }

        void flush() {
            if (mLogFileStream.is_open())
                mLogFileStream.flush();
        }
    };

} // namespace Logger