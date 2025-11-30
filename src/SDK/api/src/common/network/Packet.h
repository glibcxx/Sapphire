#pragma once

#include "SDK/core/SymbolResolver.h"
#include "SDK/api/src-deps/raknet/raknet/PacketPriority.h"
#include "SDK/api/src/common/platform/Result.h"
#include "NetworkPeer.h"
#include "Compressibility.h"
#include "NetEventCallback.h"

class IPacketHandlerDispatcher;
class BinaryStream;
class ReadOnlyBinaryStream;
class NetworkIdentifier;

enum class MinecraftPacketIds : int {
    // ...
};

// size: 4
class PacketHeader {
public:
    static constexpr uint32_t NUM_CHANNEL_BITS = 1;
    static constexpr uint32_t NUM_CHANNELS = 2;
    static constexpr uint32_t NUM_BITS_FOR_SUBID = 2;

    // size: 4
    union /*`anonymous'*/ {
        uint32_t mRaw;
        // size: 4
        struct /*`anonymous'*/ {
            uint32_t mPacketId    : 10;
            uint32_t mSenderSubId : NUM_BITS_FOR_SUBID;
            uint32_t mClientSubId : NUM_BITS_FOR_SUBID;
        } mBits;
    } mData; // off+0
};

// size: 48
class Packet {
public:
    PacketPriority                        mPriority;         // off+8
    NetworkPeer::Reliability              mReliability;      // off+12
    uint8_t                               mClientSubId;      // off+16
    bool                                  mIsHandled;        // off+17
    std::chrono::steady_clock::time_point mReceiveTimepoint; // off+24
    const IPacketHandlerDispatcher       *mHandler;          // off+32
    Compressibility                       mCompressible;     // off+40

    // vtb+0
    virtual ~Packet() = default;

    // vtb+1
    virtual MinecraftPacketIds getId() const;

    // vtb+2
    virtual std::string getName() const = 0;

    // vtb+3
    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x4C\x8B\xDC\x4D\x89\x43\x00\x53\x48\x81\xEC")
    SDK_API virtual Bedrock::Result<void> checkSize(size_t packetSize, bool receiverIsServer) const;

    // vtb+4
    virtual void write(BinaryStream &stream) const = 0;

    // vtb+5
    virtual Bedrock::Result<void> read(ReadOnlyBinaryStream &stream);

    // vtb+6
    virtual bool disallowBatching() const;

    // vtb+7
    virtual bool isValid() const;

    // vtb+8
    virtual Bedrock::Result<void> _read(ReadOnlyBinaryStream &stream) = 0;

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "disp:7,call", "\x48\x8D\x55\x00\x49\x8B\x0F\xE8\x00\x00\x00\x00\x48\x8B\xD0")
    SDK_API Bedrock::Result<void> readNoHeader(ReadOnlyBinaryStream &bitstream, const SubClientId &subid);
};
static_assert(sizeof(Packet) == 48);

class IPacketHandlerDispatcher {
public:
    // vtb+0
    virtual ~IPacketHandlerDispatcher() = default;

    // vtb+1
    virtual void handle(const NetworkIdentifier &source, class NetEventCallback &callback, std::shared_ptr<Packet> &packet) const = 0;
};

template <typename T, bool SHARED>
class PacketHandlerDispatcherInstance : public IPacketHandlerDispatcher {
public:
    // vtb+1
    virtual void handle(const NetworkIdentifier &source, class NetEventCallback &callback, std::shared_ptr<Packet> &packet) const;
};

// size: 1
class MinecraftPackets {
public:
    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x48\x83\xBD\x00\x00\x00\x00\x00\x0F\x84\x00\x00\x00\x00\xFF\x15")
    SDK_API static std::shared_ptr<Packet> createPacket(MinecraftPacketIds id);
};