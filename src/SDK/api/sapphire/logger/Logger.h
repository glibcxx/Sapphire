#pragma once

#include <chrono>
#include <memory>
#include <mutex>
#include <vector>

namespace sapphire {

    enum class LogLevel {
        Debug,
        Info,
        Warn,
        Error,
        _count,
    };

    constexpr auto logLevelToString(LogLevel level) {
        switch (level) {
        case LogLevel::Debug: return "DEBUG";
        default:
        case LogLevel::Info: return "INFO";
        case LogLevel::Warn: return "WARN";
        case LogLevel::Error: return "ERROR";
        }
    }

    struct LogEvent {
        std::chrono::system_clock::time_point time;
        LogLevel                              level;
        const char                           *moduleName;
        std::string                           message;

        auto toString() const {
            std::chrono::zoned_time zt{std::chrono::current_zone(), time};
            return std::format(
                "{:%Y.%m.%d-%H:%M:%S} - [{}][{}] {}\n",
                zt,
                logLevelToString(level),
                moduleName,
                message
            );
        }
    };

    class ILogSink {
    public:
        virtual ~ILogSink() = default;

        virtual void emit(const LogEvent &event) = 0;

        virtual void flush() {}
    };

    class LogManager {
        LogManager();

    public:
        SPHR_API static LogManager &getInstance();

        void addSink(std::shared_ptr<ILogSink> sink) {
            std::lock_guard<std::mutex> lock(mMutex);
            mSinks.push_back(std::move(sink));
        }

        void removeSink(std::shared_ptr<ILogSink> sink) {
            std::lock_guard<std::mutex> lock(mMutex);
            std::erase_if(mSinks, [&sink](const std::shared_ptr<sapphire::ILogSink> &s) {
                return s == sink;
            });
        }

        void dispatch(LogEvent &&event) {
            std::lock_guard<std::mutex> lock(mMutex);
            for (auto &sink : mSinks) {
                sink->emit(event);
            }
        }

        void flushAll() {
            std::lock_guard<std::mutex> lock(mMutex);
            for (auto &sink : mSinks) sink->flush();
        }

    private:
        std::vector<std::shared_ptr<ILogSink>> mSinks;
        std::mutex                             mMutex;
    };

    class Logger {
        const char *mModuleName;

    public:
        explicit Logger(const char *name) : mModuleName(name ? name : "unnamed") {}

        SPHR_API static Logger &getDefault();

        template <typename... Args>
        void debug(std::format_string<Args...> fmt, Args &&...args) {
            LogEvent event;
            event.level = LogLevel::Debug;
            event.time = std::chrono::system_clock::now();
            event.moduleName = mModuleName;
            std::format_to(std::back_inserter(event.message), fmt, std::forward<Args>(args)...);
            LogManager::getInstance().dispatch(std::move(event));
        }

        template <typename... Args>
        void info(std::format_string<Args...> fmt, Args &&...args) {
            LogEvent event;
            event.level = LogLevel::Info;
            event.time = std::chrono::system_clock::now();
            event.moduleName = mModuleName;
            std::format_to(std::back_inserter(event.message), fmt, std::forward<Args>(args)...);
            LogManager::getInstance().dispatch(std::move(event));
        }

        template <typename... Args>
        void error(std::format_string<Args...> fmt, Args &&...args) {
            LogEvent event;
            event.level = LogLevel::Error;
            event.time = std::chrono::system_clock::now();
            event.moduleName = mModuleName;
            std::format_to(std::back_inserter(event.message), fmt, std::forward<Args>(args)...);
            LogManager::getInstance().dispatch(std::move(event));
        }

        template <typename... Args>
        void warn(std::format_string<Args...> fmt, Args &&...args) {
            LogEvent event;
            event.level = LogLevel::Warn;
            event.time = std::chrono::system_clock::now();
            event.moduleName = mModuleName;
            std::format_to(std::back_inserter(event.message), fmt, std::forward<Args>(args)...);
            LogManager::getInstance().dispatch(std::move(event));
        }
    };

    template <typename... Args>
    void debug(std::format_string<Args...> fmt, Args &&...args) {
        Logger::getDefault().debug(fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void info(std::format_string<Args...> fmt, Args &&...args) {
        Logger::getDefault().info(fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void error(std::format_string<Args...> fmt, Args &&...args) {
        Logger::getDefault().error(fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void warn(std::format_string<Args...> fmt, Args &&...args) {
        Logger::getDefault().warn(fmt, std::forward<Args>(args)...);
    }
    template <typename... Args>
    void alert(std::wformat_string<Args...> fmt, Args &&...args) {
        MessageBox(nullptr, std::format(fmt, std::forward<Args>(args)...).data(), L"Alert", MB_ICONWARNING);
    }

} // namespace sapphire
