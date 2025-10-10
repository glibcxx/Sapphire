#pragma once

#include "ClientNetworkHandler.h"
#include "SDK/api/src/common/world/level/LevelListener.h"

class LegacyClientNetworkHandler : public ClientNetworkHandler, public LevelListener {
public:
    SDK_API /*virtual*/ void handle(const NetworkIdentifier &source, std::shared_ptr<class UpdateBlockSyncedPacket> packet) /*override*/;

    SDK_API /*virtual*/ void handle(const NetworkIdentifier &source, std::shared_ptr<class UpdateBlockPacket> packet) /*override*/;

    SDK_API /*virtual*/ void handle(const NetworkIdentifier &source, const class BlockEventPacket &packet) /*override*/;

    SDK_API /*virtual*/ void handle(const NetworkIdentifier &source, std::shared_ptr<class BlockActorDataPacket> packet) /*override*/;

    SDK_API /*virtual*/ void handle(const NetworkIdentifier &source, std::shared_ptr<class LevelChunkPacket> packet) /*override*/;

    SDK_API /*virtual*/ void handle(const NetworkIdentifier &source, const class UpdateSubChunkBlocksPacket &packet) /*override*/;
};