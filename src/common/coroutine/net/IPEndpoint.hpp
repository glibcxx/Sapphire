#pragma once

#include <cstring>
#include <format>
#include "IPv4.hpp"
#include "IPv6.hpp"
#include "../detail/WinDefs.h"

namespace sapphire::coro::net {

    class IPv4Endpoint {
        IPv4Address   mAddress{};
        std::uint16_t mPort = 0;

    public:
        IPv4Endpoint() = default;

        constexpr explicit IPv4Endpoint(IPv4Address address, std::uint16_t port = 0) noexcept :
            mAddress(address), mPort(port) {}

        constexpr const IPv4Address &address() const noexcept { return mAddress; }

        constexpr std::uint16_t port() const noexcept { return mPort; }

        constexpr IPv4Endpoint &setPort(std::uint16_t port) noexcept {
            mPort = port;
            return *this;
        }

        constexpr bool operator==(const IPv4Endpoint &other) const = default;

        constexpr auto operator<=>(const IPv4Endpoint &other) const {
            auto r = mAddress <=> other.mAddress;
            if (r == std::strong_ordering::equal)
                return mPort <=> other.mPort;
            else
                return r;
        }

        std::string toString() const { return mAddress.toString().append(":").append(std::to_string(mPort)); }

        static constexpr std::optional<IPv4Endpoint> fromString(std::string_view string) noexcept {
            auto colonPos = string.find(':');
            if (colonPos == std::string_view::npos) { return std::nullopt; }

            std::optional<IPv4Address> address = IPv4Address::fromString(string.substr(0, colonPos));
            if (!address) return std::nullopt;
            std::uint16_t port;
            string.remove_prefix(colonPos + 1);
            if (consumeInteger(string, 10, port)) return std::nullopt;

            return IPv4Endpoint{*address, port};
        }
    };

    class IPv6Endpoint {
        IPv6Address   mAddress{};
        std::uint16_t mPort = 0;

    public:
        IPv6Endpoint() = default;

        constexpr explicit IPv6Endpoint(IPv6Address address, std::uint16_t port = 0) noexcept :
            mAddress(address), mPort(port) {}

        constexpr const IPv6Address &address() const noexcept { return mAddress; }

        constexpr std::uint16_t port() const noexcept { return mPort; }

        constexpr IPv6Endpoint &setPort(std::uint16_t port) noexcept {
            mPort = port;
            return *this;
        }

        constexpr bool operator==(const IPv6Endpoint &other) const = default;

        constexpr auto operator<=>(const IPv6Endpoint &other) const {
            auto r = mAddress <=> other.mAddress;
            if (r == std::strong_ordering::equal)
                return mPort <=> other.mPort;
            else
                return r;
        }

        std::string toString() const { return std::format("[{}]:{}", mAddress.toString(), mPort); }

        static std::optional<IPv6Endpoint> fromString(std::string_view string) noexcept;
    };

    class IPEndpoint {
        enum class Family { ipv4,
                            ipv6 };

        Family mFamily;

        union {
            IPv4Endpoint mIpv4;
            IPv6Endpoint mIpv6;
        };

    public:
        constexpr IPEndpoint() noexcept : mFamily(Family::ipv4), mIpv4() {}

        constexpr IPEndpoint(IPv4Endpoint endpoint) noexcept : mFamily(Family::ipv4), mIpv4(endpoint) {}
        constexpr IPEndpoint(IPv6Endpoint endpoint) noexcept : mFamily(Family::ipv6), mIpv6(endpoint) {}

        constexpr bool isIpv4() const noexcept { return mFamily == Family::ipv4; }
        constexpr bool isIpv6() const noexcept { return mFamily == Family::ipv6; }

        constexpr const IPv4Endpoint &toIpv4() const noexcept { return mIpv4; }
        constexpr const IPv6Endpoint &toIpv6() const noexcept { return mIpv6; }

        constexpr std::uint16_t port() const noexcept { return isIpv4() ? mIpv4.port() : mIpv6.port(); }

        constexpr IPEndpoint &setPort(std::uint16_t port) noexcept {
            isIpv4() ? (void)mIpv4.setPort(port) : (void)mIpv6.setPort(port);
            return *this;
        }

        constexpr bool operator==(const IPEndpoint &other) const noexcept {
            return isIpv4() ? (other.isIpv4() && mIpv4 == other.mIpv4) : (other.isIpv6() && mIpv6 == other.mIpv6);
        }

        constexpr auto operator<=>(const IPEndpoint &other) const noexcept {
            if (mFamily != other.mFamily) return std::partial_ordering::unordered;
            return isIpv4() ? (mIpv4 <=> other.mIpv4) : (mIpv6 <=> other.mIpv6).operator std::partial_ordering();
        }

        std::string toString() const { return isIpv4() ? mIpv4.toString() : mIpv6.toString(); }

        int toSockAddr(win::sockaddr &out) const noexcept {
            if (isIpv4()) {
                const auto      &ipv4EndPoint = toIpv4();
                win::sockaddr_in ipv4Address{};
                ipv4Address.sin_family = 2; // AF_INET
                std::memcpy(&ipv4Address.sin_addr, ipv4EndPoint.address().bytes(), 4);
                ipv4Address.sin_port = htons(ipv4EndPoint.port());
                std::memset(&ipv4Address.sin_zero, 0, sizeof(ipv4Address.sin_zero));
                std::memcpy(&out, &ipv4Address, sizeof(ipv4Address));
                return sizeof(win::sockaddr_in);
            } else {
                const auto       &ipv6EndPoint = toIpv6();
                win::sockaddr_in6 ipv6Address{};
                ipv6Address.sin6_family = 23; // AF_INET6
                std::memcpy(&ipv6Address.sin6_addr, ipv6EndPoint.address().bytes(), 16);
                ipv6Address.sin6_port = htons(ipv6EndPoint.port());
                ipv6Address.sin6_flowinfo = 0;
                std::memcpy(&out, &ipv6Address, sizeof(ipv6Address));
                return sizeof(win::sockaddr_in6);
            }
        }

        static IPEndpoint fromSockAddr(win::sockaddr &addr, std::uint16_t port = 0) noexcept {
            if (addr.sa_family == 2) {
                win::sockaddr_in ipv4Address;
                std::memcpy(&ipv4Address, &addr, sizeof(ipv4Address));
                std::uint8_t addressBytes[4];
                std::memcpy(addressBytes, &ipv4Address.sin_addr, 4);
                return IPv4Endpoint{IPv4Address{addressBytes}, port == 0 ? ntohs(ipv4Address.sin_port) : port};
            } else {
                win::sockaddr_in6 ipv6Address;
                std::memcpy(&ipv6Address, &addr, sizeof(ipv6Address));
                return IPv6Endpoint{
                    IPv6Address{ipv6Address.sin6_addr.u.Byte}, port == 0 ? ntohs(ipv6Address.sin6_port) : port
                };
            }
        }

        static std::optional<IPEndpoint> fromString(std::string_view string) noexcept {
            if (auto ipv4 = IPv4Endpoint::fromString(string)) return *ipv4;
            if (auto ipv6 = IPv6Endpoint::fromString(string)) return *ipv6;
            return std::nullopt;
        }
    };

} // namespace sapphire::coro::net