#pragma once

#include <csignal>
#include <iterator>
#include <Windows.h>
#include <dbghelp.h>
#include <atomic>
#include <array>
#include <string_view>
#include <filesystem>
#include <chrono>
#include <algorithm>
#include <type_traits>

namespace sapphire::core {

    template <size_t Capacity>
    class CrashBuffer {
    public:
        constexpr CrashBuffer() = default;

        template <typename... Args>
        void write(std::format_string<Args...> fmt, Args &&...args) {
            auto remaining = Capacity - mOffset;
            if (remaining <= 1) return;

            auto result = std::format_to_n(
                mData.data() + mOffset,
                remaining - 1,
                fmt,
                std::forward<Args>(args)...
            );

            mOffset = result.out - mData.data();
            *result.out = '\0';
        }

        constexpr auto   data() const { return mData.data(); }
        constexpr auto   c_str() const { return mData.data(); }
        constexpr auto   view() const { return std::string_view{mData.data(), mOffset}; }
        constexpr size_t size() const { return mOffset; }
        constexpr void   clear() {
            mOffset = 0;
            mData[0] = '\0';
        }

    private:
        std::array<char, Capacity> mData = {0};
        size_t                     mOffset = 0;
    };

    class CrashLogger {
    public:
        CrashLogger();

        CrashLogger(const CrashLogger &) = delete;
        CrashLogger &operator=(const CrashLogger &) = delete;

    private:
        std::filesystem::path mLogPath;

        static inline std::atomic<bool>      sIsCrashing = false;
        static inline CrashBuffer<16 * 1024> sLogBuffer;
        static inline CrashBuffer<32>        sMessageBuffer;
        static inline std::string            sUserPdbSearchPaths;
        static inline uintptr_t              sImagebase = 0;

        [[noreturn]] static LONG WINAPI sehHandler(EXCEPTION_POINTERS *ep);

        [[noreturn]] static void signalHandler(int sig);

        [[noreturn]] static void pureCallHandler();

        [[noreturn]] static void invalidParamHandler(const wchar_t *, const wchar_t *, const wchar_t *, unsigned int, uintptr_t);

        [[noreturn]] static void terminateHandler();

        [[noreturn]] static void handleCrash(EXCEPTION_POINTERS *ep, std::string_view reason);

        static void walkStack(CONTEXT *ctx);

        void flushToFile();
    };
} // namespace sapphire::core