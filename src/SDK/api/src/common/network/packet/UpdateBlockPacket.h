#pragma once

#include "../Packet.h"
#include "NetworkBlockPosition.h"
#include "SDK/api/src-deps/Core/Utility/PrimitiveTypes.h"

// size: 72
class UpdateBlockPacket : public Packet {
public:
    static const bool SHARE_WITH_HANDLER = true;

    NetworkBlockPosition mPos;         // off+48
    uint32_t             mLayer;       // off+60
    byte                 mUpdateFlags; // off+64
    BlockRuntimeId       mRuntimeId;   // off+68
};