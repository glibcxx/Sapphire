#pragma once

#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/entity/gamerefs_entity/EntityContext.h"
#include "SDK/api/src/common/gamerefs/OwnerPtr.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class Level;
class ServerNetworkHandler;
class NetEventCallback;
class LoopbackPacketSender;

// size: 112 (1.21.50)
class GameSession {
public:
    std::aligned_storage_t<16, 8>         mNetwork;                    // off+0
    OwnerPtr<EntityContext>               mLevelEntity;                // off+16
    Bedrock::NonOwnerPointer<Level>       mLevel;                      // off+48
    std::unique_ptr<ServerNetworkHandler> mServerNetworkHandler;       // off+72
    std::unique_ptr<NetEventCallback>     mLegacyClientNetworkHandler; // off+80
    std::unique_ptr<NetEventCallback>     mClientNetworkHandler;       // off+88
    LoopbackPacketSender                 &mLoopbackPacketSender;       // off+96
    SubClientId                           mClientSubId;                // off+104
};
#if MC_VERSION == v1_21_50
static_assert(sizeof(GameSession) == 112);
#endif