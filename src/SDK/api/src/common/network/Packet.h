#pragma once

#include "SDK/core/ApiManager.h"
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
    virtual Bedrock::Result<void> checkSize(size_t packetSize, bool receiverIsServer) const;

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
    virtual void handle(const NetworkIdentifier &source, class NetEventCallback &callback, std::shared_ptr<Packet> &packet) const {
        callback.handle(source, static_cast<T &>(*packet));
    }
};