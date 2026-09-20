#pragma once

#include <coroutine>
#include <winerror.h>

#include "common/sys/MiniWindows.h"
#include "detail/WinDefs.h"

namespace sapphire::coro {

    struct IoOperation : public win::overlapped {
        std::coroutine_handle<> mContinuation = nullptr;

        sys::win::dword_t mBytesTransferred = 0;
        sys::win::dword_t mErrorCode = 0;

        constexpr IoOperation() noexcept {
            Internal = 0;
            InternalHigh = 0;
            Offset = 0;
            OffsetHigh = 0;
            hEvent = 0;
        }
    };

} // namespace sapphire::coro