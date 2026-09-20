#pragma once

#include <cstdint>
#include <cstring>
#include <random>
#include <system_error>
#include <vector>
#include <winsock2.h>
#include <iphlpapi.h>
#include "IPEndpoint.hpp"
#include "../Task.hpp"
#include "NetResult.hpp"
#include "Socket.hpp"
#include "common/ByteStream.hpp"
#include "common/Endian.hpp"
#include "common/Expected.hpp"
#include "common/ExpectedHelpers.h"
#include "common/coroutine/IoContext.hpp"

#pragma comment(lib, "iphlpapi.lib")

namespace sapphire::coro::net {

    namespace detail {

        inline Expected<std::string, std::error_code> decodeDnsName(ByteReader &reader) {
            std::string name;
            int         jumpCount = 0;
            bool        jumped = false;
            size_t      originalOffset = reader.offset();

            constexpr uint8_t labelMask = 0xC0;
            constexpr uint8_t compressionPtrLabel = 0xC0;

            while (true) {
                if (jumpCount > 10) return {unexpected, std::make_error_code(std::errc::too_many_links)};

                X_ASSIGN(uint8_t lenOrPtr, reader.read<uint8_t>());

                if (lenOrPtr == 0) break;

                if ((lenOrPtr & compressionPtrLabel) == compressionPtrLabel) {
                    X_ASSIGN(std::uint8_t nextByte, reader.read<uint8_t>());
                    uint16_t pointer = ((lenOrPtr & ~labelMask) << 8) | nextByte;

                    if (!jumped) {
                        originalOffset = reader.offset();
                        jumped = true;
                    }
                    reader.setOffset(pointer);
                    jumpCount++;
                    continue;
                }
                X_ASSIGN(auto labelView, reader.readSpan(lenOrPtr));
                if (!name.empty()) name += ".";
                name += std::string_view(reinterpret_cast<const char *>(labelView.data()), labelView.size());
            }
            if (jumped) reader.setOffset(originalOffset);
            return name;
        }

        inline Expected<void, std::error_code> encodeDnsName(ByteWriter &writer, std::string_view domain) {
            size_t pos = 0;
            while (pos < domain.length()) {
                size_t dotPos = domain.find('.', pos);
                if (dotPos == std::string_view::npos) dotPos = domain.length();

                const size_t len = dotPos - pos;

                if (len == 0 || len > 63) { return {unexpected, std::make_error_code(std::errc::value_too_large)}; }

                writer.write<uint8_t>(static_cast<uint8_t>(len));
                writer.writeBytes({reinterpret_cast<const uint8_t *>(domain.data() + pos), len});

                pos = dotPos + 1;
            }
            writer.write<uint8_t>(0x00);
            return {};
        }

        inline Expected<std::vector<IPEndpoint>, std::error_code> getSysDnsServers() {
            std::vector<IPEndpoint> dnsServers;

            ULONG flags = GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST;
            ULONG outBufLen = 15000; // 15KB

            auto pAddresses = std::make_unique<uint8_t[]>(outBufLen);

            PIP_ADAPTER_ADDRESSES pCurrAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(pAddresses.get());

            DWORD dwRetVal = GetAdaptersAddresses(AF_UNSPEC, flags, NULL, pCurrAddresses, &outBufLen);
            if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
                pAddresses = std::make_unique<uint8_t[]>(outBufLen);
                pCurrAddresses = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(pAddresses.get());
                dwRetVal = GetAdaptersAddresses(AF_UNSPEC, flags, NULL, pCurrAddresses, &outBufLen);
            }

            if (dwRetVal != NO_ERROR)
                return {
                    unexpected, std::error_code{(int)dwRetVal, std::generic_category()}
                };

            while (pCurrAddresses) {
                if (pCurrAddresses->OperStatus == IfOperStatusUp
                    && pCurrAddresses->IfType != IF_TYPE_SOFTWARE_LOOPBACK) {
                    PIP_ADAPTER_DNS_SERVER_ADDRESS pDnsServer = pCurrAddresses->FirstDnsServerAddress;
                    while (pDnsServer) {
                        sockaddr *addr = pDnsServer->Address.lpSockaddr;
                        dnsServers.emplace_back(IPEndpoint::fromSockAddr((coro::win::sockaddr &)*addr, 53));
                        pDnsServer = pDnsServer->Next;
                    }
                }
                pCurrAddresses = pCurrAddresses->Next;
            }

            if (dnsServers.empty())
                return {
                    unexpected, std::error_code{ERROR_NOT_FOUND, std::generic_category()}
                };
            return dnsServers;
        }

        class DnsSecurity {
        public:
            static uint16_t generateTransactionId() {
                thread_local std::random_device rd;
                return static_cast<uint16_t>(rd() & 0xFFFF);
            }
        };

    } // namespace detail

#pragma pack(push, 1)
    struct DnsHeader {
        endian::ubig16_t id;
        endian::ubig16_t flags;
        endian::ubig16_t qdcount;
        endian::ubig16_t ancount;
        endian::ubig16_t nscount;
        endian::ubig16_t arcount;
    };

#pragma pack(pop)

    struct DnsQuestion {
        std::string mName;
        uint16_t    mType;
        uint16_t    mClass;

        static Expected<DnsQuestion, std::error_code> decode(ByteReader &reader) {
            DnsQuestion q;
            X_ASSIGN(q.mName, detail::decodeDnsName(reader));
            X_ASSIGN(q.mType, reader.read<endian::ubig16_t>());
            X_ASSIGN(q.mClass, reader.read<endian::ubig16_t>());
            return q;
        }

        Expected<void, std::error_code> encode(ByteWriter &writer) const {
            X_CHECK(detail::encodeDnsName(writer, mName));
            endian::ubig16_t type_be = mType;
            endian::ubig16_t class_be = mClass;
            writer.write(type_be);
            writer.write(class_be);
            return {};
        }
    };

    struct DnsResourceRecord {
        std::string              mName;
        uint16_t                 mType;
        uint16_t                 mClass;
        uint32_t                 mTTL;
        uint16_t                 mRDLength;
        std::span<const uint8_t> mRData;

        static Expected<DnsResourceRecord, std::error_code> decode(ByteReader &reader) {
            DnsResourceRecord rr;
            X_ASSIGN(rr.mName, detail::decodeDnsName(reader));
            X_ASSIGN(rr.mType, reader.read<endian::ubig16_t>());
            X_ASSIGN(rr.mClass, reader.read<endian::ubig16_t>());
            X_ASSIGN(rr.mTTL, reader.read<endian::ubig32_t>());
            X_ASSIGN(rr.mRDLength, reader.read<endian::ubig16_t>());
            X_ASSIGN(rr.mRData, reader.readSpan(rr.mRDLength));
            return rr;
        }
    };

    struct DnsPacket {
        DnsHeader                      mHeader;
        std::vector<DnsQuestion>       mQuestions;
        std::vector<DnsResourceRecord> mAnswers;
        std::vector<DnsResourceRecord> mAuthorities;
        std::vector<DnsResourceRecord> mAdditional;

        static Expected<DnsPacket, std::error_code> parse(std::span<const uint8_t> udpPayload) {
            if (udpPayload.size() < sizeof(DnsHeader)) {
                return {unexpected, std::make_error_code(std::errc::result_out_of_range)};
            }

            DnsPacket  msg;
            ByteReader reader{udpPayload};
            std::memcpy(&msg.mHeader, udpPayload.data(), sizeof(DnsHeader));
            reader.skip(sizeof(DnsHeader));
            for (int i = 0; i < msg.mHeader.qdcount; ++i) {
                X_ASSIGN(auto q, DnsQuestion::decode(reader));
                msg.mQuestions.push_back(std::move(q));
            }
            for (int i = 0; i < msg.mHeader.ancount; ++i) {
                X_ASSIGN(auto rr, DnsResourceRecord::decode(reader));
                msg.mAnswers.push_back(std::move(rr));
            }
            return msg;
        }
    };

    inline Expected<void, std::error_code> buildDnsQuery(ByteWriter &writer, uint16_t id, const DnsQuestion &question) {
        std::vector<uint8_t> buffer(sizeof(DnsHeader));
        auto                *hdr = reinterpret_cast<DnsHeader *>(buffer.data());
        hdr->id = id;
        hdr->flags = 0x0100; // 0x0100 = Standard Query, Recursion Desired
        hdr->qdcount = 1;
        hdr->ancount = 0;
        hdr->nscount = 0;
        hdr->arcount = 0;
        writer.writeBytes(buffer);
        X_CHECK(question.encode(writer));
        return {};
    }

    inline coro::Task<NetResult<std::vector<IPEndpoint>>> resolve(coro::IoContext &io, std::string_view domain) {
        CO_ASSIGN(NetResult<Socket> socket, Socket::createUDPv4(io));
        CO_CHECK(socket->bind({}));

        CO_ASSIGN(auto systemDnsList, detail::getSysDnsServers());
        IPEndpoint *dnsIp = nullptr;
        for (auto &&dns : systemDnsList) {
            if (dns.isIpv4()) dnsIp = &dns;
        }

        if (!dnsIp)
            co_return {
                unexpected, ERROR_NOT_FOUND, "No suitable DNS server found."
            };

        uint16_t    transactionId = detail::DnsSecurity::generateTransactionId();
        DnsQuestion question{
            .mName = std::string{domain},
            .mType = 1, // A 记录 (IPv4)
            .mClass = 1 // IN (互联网)
        };
        ByteWriter writer;
        CO_CHECK(buildDnsQuery(writer, transactionId, question));
        CO_CHECK(co_await socket->sendTo(*dnsIp, writer.extract()));

        std::vector<uint8_t> respBuf(512);

        CO_ASSIGN(auto res, co_await socket->recvFrom(respBuf));
        CO_ASSIGN(auto msg, DnsPacket::parse({respBuf.data(), res.mBytesTransferred}));

        if (msg.mHeader.id != transactionId) co_return Unexpected{std::errc::bad_message};

        std::vector<IPEndpoint> ips;
        for (const auto &answer : msg.mAnswers) {
            if (answer.mType == 1 && answer.mRData.size() == 4) {
                ips.emplace_back(
                    IPv4Endpoint{
                        IPv4Address{answer.mRData[0], answer.mRData[1], answer.mRData[2], answer.mRData[3]},
                        0
                }
                );
            }
        }
        co_return ips;
    }

} // namespace sapphire::coro::net