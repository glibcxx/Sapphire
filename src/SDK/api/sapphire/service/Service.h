#pragma once

#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src/common/server/ServerLevel.h"

class ServerInstance;

namespace sapphire::inline service {

    inline ClientInstance *getPrimaryClientInstance() {
        return ClientInstance::primaryClientInstance;
    }

    SDK_API Level *getServerLevel();

    SDK_API Level *getMultiPlayerLevel();

    SDK_API ServerInstance *getServerInstance();

    SDK_API LocalPlayer *getLocalPlayer();

    SDK_API Bedrock::NonOwnerPointer<Minecraft> getClientMinecraft();

    void init();
    void uninit();

} // namespace sapphire::inline service