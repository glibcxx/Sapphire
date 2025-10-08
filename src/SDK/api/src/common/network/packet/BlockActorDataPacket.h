#pragma once

#include "../Packet.h"
#include "NetworkBlockPosition.h"
#include "SDK/api/src/common/nbt/CompoundTag.h"

// size: 88
class BlockActorDataPacket : public Packet {
public:
    static const bool SHARE_WITH_HANDLER = true;

    NetworkBlockPosition mPos;  // off+48
    CompoundTag          mData; // off+64
};