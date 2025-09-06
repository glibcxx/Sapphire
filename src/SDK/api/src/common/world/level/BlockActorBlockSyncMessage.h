#pragma once

#include "SDK/api/src/common/world/level/BlockPos.h"

// size: 16
struct BlockActorBlockSyncMessage {
    enum class MessageId : int {
        NONE = 0,
        CREATE = 1,
        DESTROY = 2,
    };
    BlockPos  mBlockEntityPos; // off+0
    MessageId mMessage;        // off+12
};
