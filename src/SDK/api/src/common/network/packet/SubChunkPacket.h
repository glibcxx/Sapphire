#pragma once

#include "SDK/api/src/common/network/Packet.h"

// https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/network/packet/SubChunkPacket.h#L19

class SubChunkPacket : public Packet {
public:
    enum class SubChunkRequestResult : uint8_t {
        Undefined = 0,
        Success = 1,
        LevelChunkDoesntExist = 2,
        WrongDimension = 3,
        PlayerDoesntExist = 4,
        IndexOutOfBounds = 5,
        SuccessAllAir = 6,
    };
};