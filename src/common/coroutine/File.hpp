#pragma once

#include <filesystem>
#include "FileBase.hpp"
#include "TypeTraits.hpp"

namespace sapphire::coro {

    template <FileAccessMode Access>
    class File : public FileBase {
    public:
        File(win32::Handle handle) noexcept : FileBase(std::move(handle)) {}

        static Expected<File, std::error_code> open(
            IoContext                   &ctx,
            const std::filesystem::path &path,
            FileCreationDisposition      creationDisposition = hasFileMode(Access, FileAccessMode::Write)
                                                                 ? FileCreationDisposition::OpenOrCreate
                                                                 : FileCreationDisposition::OpenExisting,
            FileShareMode                shareMode = hasFileMode(Access, FileAccessMode::Write) ? FileShareMode::None
                                                                                                : FileShareMode::Read,
            FileBufferingMode            bufferingMode = FileBufferingMode::Default
        ) {
            return FileBase::open(ctx, path, Access, creationDisposition, shareMode, bufferingMode);
        }

        auto read(uint64_t offset, void *buffer, std::size_t size) noexcept
            requires(hasFileMode(Access, FileAccessMode::Read))
        {
            return ReadOperation{mHandle.get(), offset, buffer, size};
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, false>
        auto read(uint64_t offset, T &&buffer) noexcept {
            return read(offset, std::ranges::data(buffer), std::ranges::size(buffer));
        }

        auto read(uint64_t offset, void *buffer, std::size_t size, std::stop_token tk) noexcept
            requires(hasFileMode(Access, FileAccessMode::Read))
        {
            return coro::CancelableOperation<ReadOperation>{std::move(tk), mHandle.get(), offset, buffer, size};
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, false>
        auto read(uint64_t offset, T &&buffer, std::stop_token tk) noexcept {
            return read(offset, std::ranges::data(buffer), std::ranges::size(buffer), std::move(tk));
        }

        auto write(uint64_t offset, const void *buffer, std::size_t size) noexcept
            requires(hasFileMode(Access, FileAccessMode::Write))
        {
            return WriteOperation{mHandle.get(), offset, const_cast<void *>(buffer), size};
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto write(uint64_t offset, T &&buffer) noexcept {
            return write(offset, std::ranges::data(buffer), std::ranges::size(buffer));
        }

        auto write(uint64_t offset, const void *buffer, std::size_t size, std::stop_token tk) noexcept
            requires(hasFileMode(Access, FileAccessMode::Write))
        {
            return coro::CancelableOperation<WriteOperation>{std::move(tk), mHandle.get(), offset, buffer, size};
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto write(uint64_t offset, T &&buffer, std::stop_token tk) noexcept {
            return write(offset, std::ranges::data(buffer), std::ranges::size(buffer), std::move(tk));
        }

        auto append(const void *buffer, std::size_t size) noexcept
            requires(hasFileMode(Access, FileAccessMode::Write))
        {
            return WriteOperation{mHandle.get(), 0xFFFFFFFF'FFFFFFFF, const_cast<void *>(buffer), size};
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto append(T &&buffer) noexcept {
            return append(std::ranges::data(buffer), std::ranges::size(buffer));
        }

        auto append(const void *buffer, std::size_t size, std::stop_token tk) noexcept
            requires(hasFileMode(Access, FileAccessMode::Write))
        {
            return coro::CancelableOperation<WriteOperation>{
                std::move(tk), 0xFFFFFFFF'FFFFFFFF, mHandle.get(), const_cast<void *>(buffer), size
            };
        }
        template <typename T>
            requires isConvertibleToCharSpan<T, true>
        auto append(T &&buffer, std::stop_token tk) noexcept {
            return append(std::ranges::data(buffer), std::ranges::size(buffer), std::move(tk));
        }
    };

    using ReadOnlyFile = File<FileAccessMode::Read>;
    using WriteOnlyFile = File<FileAccessMode::Write>;
    using ReadWriteFile = File<FileAccessMode::ReadWrite>;

} // namespace sapphire::coro