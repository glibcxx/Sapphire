#pragma once

#include "SDK/api/src/common/network/packet/SubChunkPacket.h"

class SubChunkRequestManager;

// https://github.com/LiteLDev/LeviLamina/blob/dad11c69b20f644ab7192d45add4e0be1d54d432/src/mc/world/level/chunk/SubChunkRequestSubscriber.h#L13

// size: 72
class SubChunkRequestSubscriber {
public:
    SubChunkRequestManager                                    &mManager;
    std::function<void(SubChunkPacket::SubChunkRequestResult)> mCallback;
};