#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "NetworkIdentifierWithSubId.h"
#include "ShowStoreOfferRedirectType.h"
#include "PacketViolationResponse.h"
#include "common/Memory.hpp"

#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
#    include "IncomingPacketFilterResult.h"
#    include "OutgoingPacketFilterResult.h"
#endif

class Packet;
class Player;
class ResourcePackClientResponsePacket;
class IPacketSecurityController;
class TextPacket;
enum class MinecraftPacketIds : int;
namespace Connection {
    enum class DisconnectFailReason : int;
}

class GameSpecificNetEventCallback {
public:
    // vtb+0
    virtual ~GameSpecificNetEventCallback() = 0;

    // vtb+1
    virtual void handle(const NetworkIdentifier &source, const ResourcePackClientResponsePacket &packet) = 0;
};

// size: 24
class NetEventCallback : public Bedrock::EnableNonOwnerReferences {
public:
    // vtb+0
    virtual ~NetEventCallback() = 0;

    // vtb+1
    virtual void onPlayerReady(Player &player) = 0;

    // vtb+2
    virtual void onConnect(const NetworkIdentifier &hostId) = 0;

    // vtb+3
    virtual void onUnableToConnect(Connection::DisconnectFailReason reason, const std::string &message) = 0;

    // vtb+4
    virtual void onTick() = 0;

#if MC_VERSION == v1_21_60
    virtual void onValidPacketReceived(
        const NetworkIdentifier &id, MinecraftPacketIds packetId, SubClientId, SubClientId
    ) = 0;
#endif

    virtual void onStoreOfferReceive(ShowStoreOfferRedirectType showAllOffers, const std::string &offerID) = 0;

    virtual void onDisconnect(
        const NetworkIdentifier         &id,
        Connection::DisconnectFailReason reason,
        const std::string               &message,
        bool                             skipMessage,
        const std::string               &telemetryOverride
    ) = 0;

#if MC_VERSION == v1_21_2
    virtual bool allowIncomingPacketId(const NetworkIdentifier &id, MinecraftPacketIds packetId) = 0;

#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    virtual IncomingPacketFilterResult allowIncomingPacketId(
        const NetworkIdentifier &id, MinecraftPacketIds packetId
    ) = 0;

    virtual OutgoingPacketFilterResult allowOutgoingPacket(
        const std::vector<NetworkIdentifierWithSubId> &ids, const Packet &packet
    ) = 0;

#endif

    virtual void onWebsocketRequest(
        const std::string &serverAddress, const std::string &payload, std::function<void(void)> errorCallback
    ) = 0;

#if MC_VERSION == v1_21_2 || MC_VERSION == v1_21_50
    virtual void sendPacketViolationDetectedTelemetryData(
        uint64_t,
        std::string,
        PacketViolationResponse,
        MinecraftPacketIds,
        const NetworkIdentifier &,
        uint32_t,
        SubClientId,
        SubClientId,
        uint32_t
    ) = 0;
#elif MC_VERSION == v1_21_60
    virtual void handlePacketViolation(
        const std::shared_ptr<IPacketSecurityController> &,
        const std::error_code &,
        const PacketViolationResponse,
        const MinecraftPacketIds,
        std::string &&,
        const NetworkIdentifier &,
        const SubClientId,
        const SubClientId,
        const uint32_t
    ) = 0;

    virtual void sendPacketViolationWarningPacket(
        const std::error_code &,
        const PacketViolationResponse,
        const MinecraftPacketIds,
        std::string &&,
        const NetworkIdentifier &,
        SubClientId
    ) = 0;
#endif

    virtual void onTransferRequest(
        const NetworkIdentifier &source, const std::string &serverAddress, int32_t serverPort
    ) = 0;

    virtual bool getIsConnectedToApplicationLayer() const = 0;

    virtual GameSpecificNetEventCallback *getGameSpecificNetEventCallback() = 0;

    // ... many handle functions

    /*virtual*/ void handle(const NetworkIdentifier &source, const TextPacket &packet) {
#if MC_VERSION == v1_21_2
        memory::vCall(this, 21, source, packet);
#elif MC_VERSION == v1_21_50
        memory::vCall(this, 210, source, packet);
#endif
    }
};
static_assert(sizeof(NetEventCallback) == 24);