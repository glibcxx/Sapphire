#pragma once

#include <format>
#include <windows.h>

namespace LogBox {
    template <typename... Args>
    inline void Info(std::wformat_string<Args...> fmt, Args &&...args) {
        MessageBox(nullptr, std::format(fmt, std::forward<Args>(args)...).data(), L"Info", MB_ICONINFORMATION);
    }

    template <typename... Args>
    inline void Error(std::wformat_string<Args...> fmt, Args &&...args) {
        MessageBox(nullptr, std::format(fmt, std::forward<Args>(args)...).data(), L"Error", MB_ICONERROR);
    }

    template <typename... Args>
    inline void Warn(std::wformat_string<Args...> fmt, Args &&...args) {
        MessageBox(nullptr, std::format(fmt, std::forward<Args>(args)...).data(), L"Warning", MB_ICONWARNING);
    }

} // namespace LogBox
