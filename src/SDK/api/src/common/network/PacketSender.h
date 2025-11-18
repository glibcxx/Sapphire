#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

class Packet;
class NetworkIdentifier;
class UserEntityIdentifierComponent;
struct NetworkIdentifierWithSubId;

// size: 32
class PacketSender : public Bedrock::EnableNonOwnerReferences {
public:
    uint8_t mSenderSubId; // off+24

    // vtb+0
    virtual ~PacketSender() override;

    // vtb+1
    virtual bool isInitialized() const = 0;

    // vtb+2
    virtual void send(Packet &packet) = 0;

    // vtb+3
    virtual void sendToServer(Packet &packet) = 0;

    // vtb+4
    virtual void sendToClient(const NetworkIdentifier &id, const Packet &packet, uint8_t) = 0;

    // vtb+5
    virtual void sendToClient(const UserEntityIdentifierComponent *id, const Packet &packet) = 0;

    // vtb+6
    virtual void sendToClients(const std::vector<NetworkIdentifierWithSubId> &ids, const Packet &packet) = 0;

    // vtb+7
    virtual void sendBroadcast(const NetworkIdentifier &exceptId, uint8_t, const Packet &packet) = 0;

    // vtb+8
    virtual void sendBroadcast(const Packet &packet) = 0;

    // vtb+9
    virtual void flush(const NetworkIdentifier &id, std::function<void()> &&) = 0;
};