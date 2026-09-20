#pragma once

#include <cstdint>
#include <format>
#include <optional>
#include <string>

#include "common/String.hpp"

namespace sapphire::coro::net {

    class IPv4Address {
        using bytes_t = std::uint8_t[4];

    public:
        constexpr IPv4Address() noexcept : mBytes{0, 0, 0, 0} {}

        explicit constexpr IPv4Address(std::uint32_t integer) noexcept :
            mBytes{
                static_cast<std::uint8_t>(integer >> 24),
                static_cast<std::uint8_t>(integer >> 16),
                static_cast<std::uint8_t>(integer >> 8),
                static_cast<std::uint8_t>(integer)
            } {}

        explicit constexpr IPv4Address(const std::uint8_t (&bytes)[4]) noexcept :
            mBytes{bytes[0], bytes[1], bytes[2], bytes[3]} {}

        explicit constexpr IPv4Address(std::uint8_t b0, std::uint8_t b1, std::uint8_t b2, std::uint8_t b3) noexcept :
            mBytes{b0, b1, b2, b3} {}

        constexpr const bytes_t &bytes() const noexcept { return mBytes; }

        constexpr std::uint32_t toInteger() const noexcept {
            return std::uint32_t(mBytes[0]) << 24 | std::uint32_t(mBytes[1]) << 16 | std::uint32_t(mBytes[2]) << 8
                 | std::uint32_t(mBytes[3]);
        }

        constexpr bool isLoopback() const noexcept { return mBytes[0] == 127; }

        constexpr bool isPrivateNetwork() const noexcept {
            return mBytes[0] == 10 || (mBytes[0] == 172 && (mBytes[1] & 0xF0) == 0x10)
                || (mBytes[0] == 192 && mBytes[2] == 168);
        }

        constexpr auto operator<=>(const IPv4Address &other) const = default;

        static constexpr std::optional<IPv4Address> fromString(std::string_view str) noexcept {
            if (str.empty()) return std::nullopt;
            bytes_t           bytes;
            constexpr uint8_t maxVal = 255;
            if (consumeInteger(str, 10, bytes[0]) || bytes[0] > maxVal) return std::nullopt;
            if (!consumeFront(str, '.')) return std::nullopt;
            if (consumeInteger(str, 10, bytes[1]) || bytes[1] > maxVal) return std::nullopt;
            if (!consumeFront(str, '.')) return std::nullopt;
            if (consumeInteger(str, 10, bytes[2]) || bytes[2] > maxVal) return std::nullopt;
            if (!consumeFront(str, '.')) return std::nullopt;
            if (consumeInteger(str, 10, bytes[3]) || bytes[3] > maxVal) return std::nullopt;
            return IPv4Address{bytes};
        }
        static constexpr IPv4Address loopback() noexcept { return IPv4Address{127, 0, 0, 1}; }

        [[nodiscard]] std::string toString() const {
            return std::format("{}.{}.{}.{}", mBytes[0], mBytes[1], mBytes[2], mBytes[3]);
        }

    private:
        alignas(std::uint32_t) bytes_t mBytes;
    };

} // namespace sapphire::net
