#pragma once

#include <cstdint>
#include <optional>
#include <string>

namespace sapphire::coro::net {

    class IPv6Address {
        using bytes_t = std::uint8_t[16];

    public:
        constexpr IPv6Address() noexcept : mBytes{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}
        explicit constexpr IPv6Address(std::uint64_t subnetPrefix, std::uint64_t interfaceIdentifier) :
            mBytes{
                static_cast<std::uint8_t>(subnetPrefix >> 56),
                static_cast<std::uint8_t>(subnetPrefix >> 48),
                static_cast<std::uint8_t>(subnetPrefix >> 40),
                static_cast<std::uint8_t>(subnetPrefix >> 32),
                static_cast<std::uint8_t>(subnetPrefix >> 24),
                static_cast<std::uint8_t>(subnetPrefix >> 16),
                static_cast<std::uint8_t>(subnetPrefix >> 8),
                static_cast<std::uint8_t>(subnetPrefix),
                static_cast<std::uint8_t>(interfaceIdentifier >> 56),
                static_cast<std::uint8_t>(interfaceIdentifier >> 48),
                static_cast<std::uint8_t>(interfaceIdentifier >> 40),
                static_cast<std::uint8_t>(interfaceIdentifier >> 32),
                static_cast<std::uint8_t>(interfaceIdentifier >> 24),
                static_cast<std::uint8_t>(interfaceIdentifier >> 16),
                static_cast<std::uint8_t>(interfaceIdentifier >> 8),
                static_cast<std::uint8_t>(interfaceIdentifier)
            } {}

        constexpr IPv6Address(
            std::uint16_t part0,
            std::uint16_t part1,
            std::uint16_t part2,
            std::uint16_t part3,
            std::uint16_t part4,
            std::uint16_t part5,
            std::uint16_t part6,
            std::uint16_t part7
        ) noexcept :
            mBytes{
                static_cast<std::uint8_t>(part0 >> 8),
                static_cast<std::uint8_t>(part0),
                static_cast<std::uint8_t>(part1 >> 8),
                static_cast<std::uint8_t>(part1),
                static_cast<std::uint8_t>(part2 >> 8),
                static_cast<std::uint8_t>(part2),
                static_cast<std::uint8_t>(part3 >> 8),
                static_cast<std::uint8_t>(part3),
                static_cast<std::uint8_t>(part4 >> 8),
                static_cast<std::uint8_t>(part4),
                static_cast<std::uint8_t>(part5 >> 8),
                static_cast<std::uint8_t>(part5),
                static_cast<std::uint8_t>(part6 >> 8),
                static_cast<std::uint8_t>(part6),
                static_cast<std::uint8_t>(part7 >> 8),
                static_cast<std::uint8_t>(part7)
            } {}

        explicit constexpr IPv6Address(const std::uint16_t (&parts)[8]) :
            IPv6Address(parts[0], parts[1], parts[2], parts[3], parts[4], parts[5], parts[6], parts[7]) {}

        explicit constexpr IPv6Address(const std::uint8_t (&bytes)[16]) :
            mBytes{
                bytes[0],
                bytes[1],
                bytes[2],
                bytes[3],
                bytes[4],
                bytes[5],
                bytes[6],
                bytes[7],
                bytes[8],
                bytes[9],
                bytes[10],
                bytes[11],
                bytes[12],
                bytes[13],
                bytes[14],
                bytes[15]
            } {}

        constexpr const bytes_t &bytes() const { return mBytes; }

        constexpr std::uint64_t subnetPrefix() const {
            return static_cast<std::uint64_t>(mBytes[0]) << 56 | static_cast<std::uint64_t>(mBytes[1]) << 48
                 | static_cast<std::uint64_t>(mBytes[2]) << 40 | static_cast<std::uint64_t>(mBytes[3]) << 32
                 | static_cast<std::uint64_t>(mBytes[4]) << 24 | static_cast<std::uint64_t>(mBytes[5]) << 16
                 | static_cast<std::uint64_t>(mBytes[6]) << 8 | static_cast<std::uint64_t>(mBytes[7]);
        }

        constexpr std::uint64_t interfaceIdentifier() const {
            return static_cast<std::uint64_t>(mBytes[8]) << 56 | static_cast<std::uint64_t>(mBytes[9]) << 48
                 | static_cast<std::uint64_t>(mBytes[10]) << 40 | static_cast<std::uint64_t>(mBytes[11]) << 32
                 | static_cast<std::uint64_t>(mBytes[12]) << 24 | static_cast<std::uint64_t>(mBytes[13]) << 16
                 | static_cast<std::uint64_t>(mBytes[14]) << 8 | static_cast<std::uint64_t>(mBytes[15]);
        }

        static constexpr IPv6Address unspecified() { return {}; }
        static constexpr IPv6Address loopback() { return {0, 0, 0, 0, 0, 0, 0, 1}; }

        static std::optional<IPv6Address> fromString(std::string_view string) noexcept {
            // TODO
            constexpr std::size_t maxLength = 45;
            if (string.empty() || string.length() > maxLength) { return std::nullopt; }
            return std::nullopt;
        }

        std::string toString() const {
            // TODO
            return {};
        }

        constexpr auto operator<=>(const IPv6Address &other) const = default;

    private:
        alignas(std::uint64_t) bytes_t mBytes;
    };

} // namespace sapphire::net