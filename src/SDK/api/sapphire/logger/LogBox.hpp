#pragma once

#include <string>
#include <Windows.h>
#include <format>

namespace Logger {

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