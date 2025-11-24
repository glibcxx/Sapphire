#include "Logger.h"
#include <fstream>
#include "SDK/api/sapphire/platform/Environment.h"

namespace {

    class FileSink : public sapphire::ILogSink {
        std::ofstream mLogFileStream;

    public:
        FileSink() {
            try {
                std::filesystem::path logDir = sapphire::platform::Environment::getInstance().getSapphireHomePath() / L"logs";
                std::filesystem::create_directories(logDir);
                std::chrono::zoned_time zt{std::chrono::current_zone(), std::chrono::system_clock::now()};
                std::filesystem::path   logFilePath = logDir / std::format(L"Sapphire_{:%Y.%m.%d-%H_%M_%S}.log", zt);
                mLogFileStream.open(logFilePath, std::ios_base::app | std::ios_base::out);
            } catch (const std::exception &) {
            }
        }
        ~FileSink() {
            if (mLogFileStream.is_open()) {
                mLogFileStream.flush();
                mLogFileStream.close();
            }
        }

        void emit(const sapphire::LogEvent &event) override {
            if (mLogFileStream.is_open()) {
                mLogFileStream << event.toString();
                if (event.level == sapphire::LogLevel::Error)
                    mLogFileStream.flush();
            }
        }

        void flush() override {
            if (mLogFileStream.is_open())
                mLogFileStream.flush();
        }
    };

} // namespace

sapphire::LogManager &sapphire::LogManager::getInstance() {
    static LogManager instance;
    return instance;
}

sapphire::Logger &sapphire::Logger::getDefault() {
    static sapphire::Logger instance{"sapphire"};
    return instance;
}

sapphire::LogManager::LogManager() : mSinks{std::make_shared<FileSink>()} {}