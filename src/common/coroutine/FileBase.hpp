#pragma once

#include <filesystem>
#include "Handle.hpp"
#include "FileModes.hpp"
#include "FileOperation.hpp"
#include "IoContext.hpp"
#include "common/sys/MiniWindows.h"
#include "detail/WinDefs.h"

namespace sapphire::coro {

    class FileBase {
    protected:
        win32::Handle mHandle;

        FileBase(win32::Handle handle) noexcept : mHandle(std::move(handle)) {}

        template <typename T>
        static constexpr bool hasFileMode(T modes, T mode) noexcept {
            return (modes & mode) == mode;
        }

        static Expected<win32::Handle, std::error_code> open(
            IoContext                   &ctx,
            const std::filesystem::path &path,
            FileAccessMode               accessMode,
            FileCreationDisposition      creationDisposition,
            FileShareMode                shareMode,
            FileBufferingMode            bufferingMode
        ) {
            sys::win::dword_t dwAccess = 0;
            if (hasFileMode(accessMode, FileAccessMode::Read)) dwAccess |= 0x80000000l;
            if (hasFileMode(accessMode, FileAccessMode::Write)) dwAccess |= 0x40000000l;

            sys::win::dword_t dwShareMode = 0;
            if (hasFileMode(shareMode, FileShareMode::Read)) dwShareMode |= 0x00000001;
            if (hasFileMode(shareMode, FileShareMode::Write)) dwShareMode |= 0x00000002;
            if (hasFileMode(shareMode, FileShareMode::Delete)) dwShareMode |= 0x00000004;

            sys::win::dword_t dwCreationDisposition = 0;
            switch (creationDisposition) {
            case FileCreationDisposition::OpenExisting: dwCreationDisposition = 3; break;
            case FileCreationDisposition::CreateAlways: dwCreationDisposition = 2; break;
            case FileCreationDisposition::CreateNew: dwCreationDisposition = 1; break;
            case FileCreationDisposition::OpenOrCreate: dwCreationDisposition = 4; break;
            case FileCreationDisposition::TruncateExisting: dwCreationDisposition = 5; break;
            default: break;
            }

            sys::win::dword_t dwFlags = 0x40000000;
            if (hasFileMode(bufferingMode, FileBufferingMode::Sequential)) dwFlags |= 0x8000000;
            if (hasFileMode(bufferingMode, FileBufferingMode::Unbuffered)) dwFlags |= 0x20000000;
            if (hasFileMode(bufferingMode, FileBufferingMode::WriteThrough)) dwFlags |= 0x80000000;
            if (hasFileMode(bufferingMode, FileBufferingMode::Temporary)) dwFlags |= 0x00000100;

            win32::Handle h =
                CreateFileW(path.c_str(), dwAccess, dwShareMode, nullptr, dwCreationDisposition, dwFlags, nullptr);

            if (!h.isValid()) return {unexpected, (int)GetLastError(), std::system_category()};
            if (!ctx.attach(h.get())) return {unexpected, GetLastError(), std::system_category()};

            const sys::win::uchar_t flags = 0x1 | 0x2; // FILE_SKIP_COMPLETION_PORT_ON_SUCCESS | FILE_SKIP_SET_EVENT_ON_HANDLE
            if (!SetFileCompletionNotificationModes(h.get(), flags))
                return {unexpected, GetLastError(), std::system_category()};

            return h;
        }

        struct ReadOperation : FileOperation {
            using FileOperation::FileOperation;

            bool await_suspend(std::coroutine_handle<> h) noexcept {
                this->mContinuation = h;
                bool ok = ReadFile(mFileHandle, mBuffer, mBufferSize, &mBytesTransferred, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)));
                return handleIoResult(ok);
            }

            Expected<size_t, std::error_code> await_resume() noexcept {
                if (mErrorCode == ERROR_HANDLE_EOF) return 0;
                if (mErrorCode != ERROR_SUCCESS) {
                    return {unexpected, static_cast<int>(mErrorCode), std::system_category()};
                }
                return mBytesTransferred;
            }
        };

        struct WriteOperation : FileOperation {
            using FileOperation::FileOperation;

            bool await_suspend(std::coroutine_handle<> h) noexcept {
                this->mContinuation = h;
                bool ok = WriteFile(mFileHandle, mBuffer, mBufferSize, &mBytesTransferred, reinterpret_cast<LPOVERLAPPED>(static_cast<coro::win::overlapped *>(this)));
                return handleIoResult(ok);
            }
        };

    public:
        FileBase(const FileBase &) = delete;
        FileBase &operator=(const FileBase &) = delete;

        FileBase(FileBase &&other) = default;
        FileBase &operator=(FileBase &&other) = default;

        void close() noexcept { mHandle.close(); }

        Expected<uint64_t, std::error_code> size() const noexcept {
            win::large_integer size;
            bool               ok = GetFileSizeEx(mHandle.get(), (PLARGE_INTEGER)&size);
            if (!ok) { return {unexpected, static_cast<int>(GetLastError()), std::system_category()}; }
            return size.QuadPart;
        }

        const win32::Handle &getHandle() const noexcept { return mHandle; }
    };

} // namespace sapphire::coro