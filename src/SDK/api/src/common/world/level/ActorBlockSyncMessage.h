#pragma once

#include "SDK/api/src/common/ActorUniqueID.h"

// size: 16
struct ActorBlockSyncMessage {
    enum class MessageId : uint32_t {
        NONE = 0,
        CREATE = 1,
        DESTROY = 2,
    };

    ActorUniqueID mEntityUniqueID; // off+0
    MessageId     mMessage;        // off+8
};
