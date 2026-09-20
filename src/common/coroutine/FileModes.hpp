#pragma once

#include <type_traits>

namespace sapphire::coro {

    namespace detail {

        template <typename T>
            requires std::is_enum_v<T>
        constexpr std::underlying_type_t<T> enumCast(T val) noexcept {
            return static_cast<std::underlying_type_t<T>>(val);
        }

        template <typename T>
            requires(std::is_arithmetic_v<T>)
        constexpr T enumCast(T val) noexcept {
            return val;
        }

    } // namespace detail

    enum class FileAccessMode {
        Read = 1,
        Write = 2,
        ReadWrite = Read | Write,
    };

    enum class FileCreationDisposition {
        OpenExisting,
        CreateAlways,
        CreateNew,
        OpenOrCreate,
        TruncateExisting,
    };

    enum class FileShareMode {
        None = 0,
        Read = 1,
        Write = 2,
        Delete = 4,
        ReadWrite = Read | Write,
    };

    enum class FileBufferingMode {
        Default = 0,
        Sequential = 1,
        RandomAccess = 2,
        Unbuffered = 4,
        WriteThrough = 8,
        Temporary = 16,
    };

    constexpr FileAccessMode operator&(FileAccessMode a, FileAccessMode b) noexcept {
        return static_cast<FileAccessMode>(detail::enumCast(a) & detail::enumCast(b));
    }
    constexpr FileAccessMode operator|(FileAccessMode a, FileAccessMode b) noexcept {
        return static_cast<FileAccessMode>(detail::enumCast(a) | detail::enumCast(b));
    }

    constexpr FileShareMode operator&(FileShareMode a, FileShareMode b) noexcept {
        return static_cast<FileShareMode>(detail::enumCast(a) & detail::enumCast(b));
    }
    constexpr FileShareMode operator|(FileShareMode a, FileShareMode b) noexcept {
        return static_cast<FileShareMode>(detail::enumCast(a) | detail::enumCast(b));
    }

    constexpr FileBufferingMode operator&(FileBufferingMode a, FileBufferingMode b) noexcept {
        return static_cast<FileBufferingMode>(detail::enumCast(a) & detail::enumCast(b));
    }
    constexpr FileBufferingMode operator|(FileBufferingMode a, FileBufferingMode b) noexcept {
        return static_cast<FileBufferingMode>(detail::enumCast(a) | detail::enumCast(b));
    }

} // namespace sapphire::coro