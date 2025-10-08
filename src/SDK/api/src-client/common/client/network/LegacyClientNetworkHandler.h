#pragma once

#include "ClientNetworkHandler.h"
#include "SDK/api/src/common/world/level/LevelListener.h"

class UpdateBlockPacket;
class UpdateBlockSyncedPacket;
class BlockActorDataPacket;

class LegacyClientNetworkHandler : public ClientNetworkHandler, public LevelListener {
public:
    SDK_API /*virtual*/ void handle(const NetworkIdentifier &source, std::shared_ptr<UpdateBlockSyncedPacket> packet) /*override*/;

    SDK_API /*virtual*/ void handle(const NetworkIdentifier &source, std::shared_ptr<UpdateBlockPacket> packet) /*override*/;

    SDK_API /*virtual*/ void handle(const NetworkIdentifier &source, std::shared_ptr<BlockActorDataPacket> packet) /*override*/;
};