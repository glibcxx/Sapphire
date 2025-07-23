#include "LogFile.h"

#include "SDK/core/Core.h"

namespace Logger {

    LogFile::LogFile() {
        const auto &coreInfo = sapphire::SapphireModuleInfo::get();
        try {
            std::filesystem::path logDir = coreInfo.sapphireHome / L"logs";
            std::filesystem::create_directories(logDir);
            std::filesystem::path logFilePath = logDir / std::format(L"Sapphire_{:%Y.%m.%d-%H_%M_%S}.log", std::chrono::system_clock::now());

            mLogFileStream.open(logFilePath, std::ios_base::app | std::ios_base::out);
        } catch (const std::exception &) {
        }
    }

    LogFile::~LogFile() {
        if (mLogFileStream.is_open()) {
            mLogFileStream.flush();
            mLogFileStream.close();
        }
    }

} // namespace Logger