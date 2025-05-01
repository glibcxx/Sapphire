#pragma once

#include <Windows.h>

#include <format>

#include "GameLogger.hpp"

namespace Logger {
    template <typename... Args>
    inline void Debug(std::format_string<Args...> fmt, Args &&...args) {
        GameLogger::getInstance().addLog(GameLogger::Type::Debug, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void Info(std::format_string<Args...> fmt, Args &&...args) {
        GameLogger::getInstance().addLog(GameLogger::Type::Info, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void Error(std::format_string<Args...> fmt, Args &&...args) {
        GameLogger::getInstance().addLog(GameLogger::Type::Error, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void Warn(std::format_string<Args...> fmt, Args &&...args) {
        GameLogger::getInstance().addLog(GameLogger::Type::Warn, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void InfoBox(std::wformat_string<Args...> fmt, Args &&...args) {
        MessageBox(nullptr, std::format(fmt, std::forward<Args>(args)...).data(), L"Info", MB_ICONINFORMATION);
    }

    template <typename... Args>
    inline void ErrorBox(std::wformat_string<Args...> fmt, Args &&...args) {
        MessageBox(nullptr, std::format(fmt, std::forward<Args>(args)...).data(), L"Error", MB_ICONERROR);
    }

    template <typename... Args>
    inline void WarnBox(std::wformat_string<Args...> fmt, Args &&...args) {
        MessageBox(nullptr, std::format(fmt, std::forward<Args>(args)...).data(), L"Warning", MB_ICONWARNING);
    }

} // namespace Logger
