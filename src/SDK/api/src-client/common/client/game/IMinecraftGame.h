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

// size: 88 (1.21.2), 96 (1.21.50/1.21.60)
class IMinecraftGame : public Bedrock::EnableNonOwnerReferences,
                       public Bedrock::AppIsland,
                       public IMinecraftApp,
                       public IGameServerStartup,
                       public IGameServerShutdown,
                       public INetworkGameConnector,
                       public IClientInstances,
                       public IWorldTransfer
#if MC_VERSION >= v1_21_50
    ,
                       public ISplitScreenChangedPublisher
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