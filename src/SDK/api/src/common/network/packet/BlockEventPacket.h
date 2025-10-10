#pragma once

#include "../Packet.h"
#include "NetworkBlockPosition.h"

// size: 72
class BlockEventPacket : public Packet {
    NetworkBlockPosition mPos; // off+48
    int32_t              mB0;  // off+60
    int32_t              mB1;  // off+64
};