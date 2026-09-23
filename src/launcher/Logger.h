#pragma once

#include <format>
#include "common/sys/MiniWindows.h"

extern "C" {
__declspec(dllimport) int __stdcall MessageBoxW(
    sapphire::sys::win::hwnd_t    hWnd,
    sapphire::sys::win::lpcwstr_t lpText,
    sapphire::sys::win::lpcwstr_t lpCaption,
    unsigned int                  uType
);
}

template <typename... Args>
inline void InfoBox(std::wformat_string<Args...> fmt, Args &&...args) {
    MessageBoxW(nullptr, std::format(fmt, std::forward<Args>(args)...).data(), L"Info", 0x00000040L);
}

template <typename... Args>
inline void ErrorBox(std::wformat_string<Args...> fmt, Args &&...args) {
    MessageBoxW(nullptr, std::format(fmt, std::forward<Args>(args)...).data(), L"Error", 0x00000010L);
}

template <typename... Args>
inline void WarnBox(std::wformat_string<Args...> fmt, Args &&...args) {
    MessageBoxW(nullptr, std::format(fmt, std::forward<Args>(args)...).data(), L"Warning", 0x00000030L);
}