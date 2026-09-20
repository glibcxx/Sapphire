#pragma once

#include <bit>
#include <concepts>
#include <cstdint>

namespace sapphire {

#ifndef __has_builtin
#    define __has_builtin(x) 0
#endif

    [[nodiscard]] constexpr uint16_t byteswapUint16(const uint16_t val) noexcept {
#if __has_builtin(__builtin_bswap16)
        return __builtin_bswap16(val);
#elif defined(_MSC_VER)
        if (!std::is_constant_evaluated()) return _byteswap_ushort(val);
#endif
        return static_cast<uint16_t>((val << 8) | (val >> 8));
    }

    [[nodiscard]] constexpr uint32_t byteswapUint32(const uint32_t val) noexcept {
#if __has_builtin(__builtin_bswap32)
        return __builtin_bswap32(val);
#elif defined(_MSC_VER)
        if (!std::is_constant_evaluated()) return _byteswap_ulong(val);
#endif
        return (val << 24) | ((val << 8) & 0x00FF'0000) | ((val >> 8) & 0x0000'FF00) | (val >> 24);
    }

    [[nodiscard]] constexpr uint64_t byteswapUint64(const uint64_t val) noexcept {
#if __has_builtin(__builtin_bswap64)
        return __builtin_bswap64(val);
#elif defined(_MSC_VER)
        if (!std::is_constant_evaluated()) return _byteswap_uint64(val);
#endif
        return (val << 56) | ((val << 40) & 0x00FF'0000'0000'0000) | ((val << 24) & 0x0000'FF00'0000'0000)
             | ((val << 8) & 0x0000'00FF'0000'0000) | ((val >> 8) & 0x0000'0000'FF00'0000)
             | ((val >> 24) & 0x0000'0000'00FF'0000) | ((val >> 40) & 0x0000'0000'0000'FF00) | (val >> 56);
    }

    template <std::integral T>
    [[nodiscard]] constexpr T byteswap(const T val) noexcept {
        if constexpr (sizeof(T) == 1) {
            return val;
        } else if constexpr (sizeof(T) == 2) {
            return static_cast<T>(byteswapUint16(static_cast<uint16_t>(val)));
        } else if constexpr (sizeof(T) == 4) {
            return static_cast<T>(byteswapUint32(static_cast<uint32_t>(val)));
        } else if constexpr (sizeof(T) == 8) {
            return static_cast<T>(byteswapUint64(static_cast<uint64_t>(val)));
        } else {
            static_assert(false, "invalid value type");
        }
    }

    template <std::integral T>
    constexpr T hostToNetwork(T value) noexcept {
        if constexpr (std::endian::native == std::endian::big) {
            return value;
        } else {
            return byteswap(value);
        }
    }

    template <std::integral T>
    constexpr T networkToHost(T value) noexcept {
        return hostToNetwork(value);
    }

} // namespace sapphire