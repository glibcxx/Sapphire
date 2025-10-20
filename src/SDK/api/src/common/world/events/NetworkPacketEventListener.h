#pragma once

#include "EventResult.h"
#include "SDK/api/src/common/network/Packet.h"

// 该类型被弃用？

// size: 8
class NetworkPacketEventListener {
public:
    // vtb+0
    virtual ~NetworkPacketEventListener() = default;

    // vtb+1
    virtual EventResult onPacketReceivedFrom(const PacketHeader &header, const Packet &packet);
};