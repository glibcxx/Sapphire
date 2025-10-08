#pragma once

#include "UpdateBlockPacket.h"
#include "SDK/api/src/common/world/level/ActorBlockSyncMessage.h"

// size: 88
class UpdateBlockSyncedPacket : public UpdateBlockPacket {
public:
    ActorBlockSyncMessage mEntityBlockSyncMessage; // off+72
};