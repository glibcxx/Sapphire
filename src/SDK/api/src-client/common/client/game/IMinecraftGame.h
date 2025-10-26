#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Core/Islands/Islands.h"
#include "SDK/api/src/common/IMinecraftApp.h"
#include "IGameServerStartup.h"
#include "IGameServerShutdown.h"
#include "INetworkGameConnector.h"
#include "IClientInstances.h"
#include "IWorldTransfer.h"
#include "ISplitScreenChangedPublisher.h"

enum class LocalServerStartupState : int {
    NotStarted = 0,
    Starting = 1,
    Started = 2,
};

// size: 88 (1.21.2), 96 (1.21.50/1.21.60)
class IMinecraftGame : public /* off+0  */ Bedrock::EnableNonOwnerReferences,
                       public /* off+24 */ Bedrock::AppIsland,
                       public /* off+40 */ IMinecraftApp,
                       public /* off+48 */ IGameServerStartup,
                       public /* off+56 */ IGameServerShutdown,
                       public /* off+64 */ INetworkGameConnector,
                       public /* off+72 */ IClientInstances,
                       public /* off+80 */ IWorldTransfer
#if MC_VERSION >= v1_21_50
    ,
                       public /* off+88 */ ISplitScreenChangedPublisher
#endif
{
public:
    // vtb+0
    virtual ~IMinecraftGame();

    // vtb+1
    virtual void initialize(int argc, char **argv) = 0;

    // vtb+2
    virtual void update() = 0;
};