#pragma once

#include "ClientOrServerNetworkSystemRef.h"
#include "PacketSender.h"

class NetEventCallback;
class Player;

// size: 104
class LoopbackPacketSender : public PacketSender {
    ClientOrServerNetworkSystemRef              mNetwork;           // off+32
    std::vector<NetEventCallback *>             mLoopbackCallbacks; // off+48
    const std::vector<std::unique_ptr<Player>> *mUserList;          // off+72
    std::vector<NetworkIdentifierWithSubId>     mTempUserIds;       // off+80
};