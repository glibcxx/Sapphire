#pragma once

#include <cstdint>
#include <system_error>
#include "IoOperation.hpp"
#include "common/Expected.hpp"
#include "common/sys/MiniWindows.h"

namespace sapphire::coro {

    struct [[nodiscard]] FileOperation : coro::IoOperation {
        sys::win::handle_t mFileHandle;
        void              *mBuffer;
        sys::win::dword_t  mBufferSize;

        FileOperation(sys::win::handle_t fileHandle, uint64_t offset, void *buffer, size_t bufferSize) noexcept :
            mFileHandle(fileHandle), mBuffer(buffer), mBufferSize(std::min((sys::win::dword_t)bufferSize, sys::win::dword_t{0xFFFFFFFF})) {
            Offset = static_cast<sys::win::dword_t>(offset);
            OffsetHigh = static_cast<sys::win::dword_t>(offset >> 32);
        }
        static void cancelOperation(FileOperation &self) noexcept { CancelIoEx((sys::win::handle_t)self.mFileHandle, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(&self))); }

        bool await_ready() const noexcept { return false; }

        Expected<size_t, std::error_code> await_resume() noexcept {
            if (mErrorCode != ERROR_SUCCESS) {
                return {unexpected, static_cast<int>(mErrorCode), std::system_category()};
            }
            return mBytesTransferred;
        }

        // 若结果为需要挂起(ERROR_IO_PENDING)，返回false，否则返回true。自动设置 mErrorCode
        bool handleIoResult(bool ok) noexcept {
            if (ok == 1) return false;
            int err = GetLastError();
            if (err == ERROR_IO_PENDING) return true;
            mErrorCode = err;
            return false;
        }
    };

} // namespace sapphire::coro