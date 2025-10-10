#pragma once

#include "../Packet.h"
#include "NetworkBlockPosition.h"
#include "SDK/api/src-deps/Core/Utility/PrimitiveTypes.h"
#include "SDK/api/src/common/world/level/ActorBlockSyncMessage.h"

// https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/network/packet/UpdateSubChunkBlocksPacket.h#L20

// size: 40
struct UpdateSubChunkNetworkBlockInfo {
    NetworkBlockPosition  mPos;         // off+0
    BlockRuntimeId        mRuntimeId;   // off+12
    byte                  mUpdateFlags; // off+16
    ActorBlockSyncMessage mSyncMessage; // off+24
};

// size: 48
struct UpdateSubChunkBlocksChangedInfo {
    std::vector<UpdateSubChunkNetworkBlockInfo> mStandards; // off+0
    std::vector<UpdateSubChunkNetworkBlockInfo> mExtras;    // off+24
};

// size: 112
class UpdateSubChunkBlocksPacket : public Packet {
public:
    UpdateSubChunkBlocksChangedInfo mBlocksChanged;         // off+48
    NetworkBlockPosition            mSubChunkBlockPosition; // off+96
};