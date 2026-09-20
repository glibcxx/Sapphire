#pragma once

#include <guiddef.h>
#include "../IoOperation.hpp"
#include "SocketHandle.hpp"
#include "common/sys/MiniWindows.h"

namespace sapphire::coro::net {

    struct [[nodiscard]] NetOperation : coro::IoOperation {
        bool await_ready() const noexcept { return false; }

        // 若结果为需要挂起(WSA_IO_PENDING)，返回true，否则返回false。自动设置 mErrorCode
        bool handleIoResult(bool ok) noexcept {
            if (ok == 1) return false;
            return getAndCheckErrorCode();
        }

        // 若结果为需要挂起(WSA_IO_PENDING)，返回true，否则返回false。自动设置 mErrorCode
        bool handleIoResult(int result) noexcept {
            if (result != -1) return false;
            return getAndCheckErrorCode();
        }

        bool getAndCheckErrorCode() noexcept {
            int err = WSAGetLastError();
            if (err == -1) return true;
            mErrorCode = err;
            return false;
        }

        // 如有错误，自动设置 mErrorCode，返回nullptr
        template <typename Func>
            requires std::is_pointer_v<Func> && std::is_function_v<std::remove_pointer_t<Func>>
        Func getExFuncPtr(win::SocketHandle &handle, GUID guid) noexcept {
            Func              lpfnEx = NULL;
            sys::win::dword_t bytes = 0;
            int               result = WSAIoctl(
                handle.get(),
                ((0x80000000 | 0x40000000) | (0x08000000) | (6)), // SIO_GET_EXTENSION_FUNCTION_POINTER
                &guid,
                sizeof(guid),
                &lpfnEx,
                sizeof(lpfnEx),
                &bytes,
                NULL,
                NULL
            );
            if (result == -1) {
                mErrorCode = WSAGetLastError();
                return nullptr;
            }
            return lpfnEx;
        }
    };

} // namespace sapphire::net