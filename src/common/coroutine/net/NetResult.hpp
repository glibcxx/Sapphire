#pragma once

#include <string>
#include <string_view>
#include <system_error>
#include <winerror.h>

#include "common/Expected.hpp"

extern "C" __declspec(dllimport) int __stdcall SetProcessPreferredUILanguages(
    unsigned long dwFlags, const wchar_t *pwszLanguagesBuffer, unsigned long *pulNumLanguages
);

namespace sapphire::coro::net {

    class NetError {
        std::string     mMessage;
        std::error_code mCode;

        void appendCodeMsg() {
            if (!mMessage.empty()) mMessage.append(": ");
            mMessage.append(mCode.message());
        }

        static inline bool SET_EN_MSG = []() {
            SetProcessPreferredUILanguages(0x8, L"en-US\0", nullptr);
            return 0;
        }();

    public:
        NetError(int code = 0) : mCode(code, std::system_category()) { appendCodeMsg(); }
        NetError(std::error_code code) : mCode(code) { appendCodeMsg(); }
        NetError(std::errc code) : mCode(std::make_error_code(code)) { appendCodeMsg(); }
        NetError(int code, std::string message) : mMessage(std::move(message)), mCode(code, std::system_category()) {
            appendCodeMsg();
        }

        bool isEof() const noexcept { return mCode.value() == 0; }
        bool isOpAborted() const noexcept { return mCode.value() == ERROR_OPERATION_ABORTED; }

        explicit operator bool() const noexcept { return !isEof(); }

        std::error_code  getCode() const noexcept { return mCode; }
        std::string_view message() const noexcept { return mMessage; }
    };

    template <typename T>
    using NetResult = Expected<T, NetError>;

} // namespace sapphire::net