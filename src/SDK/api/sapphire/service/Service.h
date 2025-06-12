#pragma once

#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src/common/server/ServerLevel.h"

class ServerInstance;

namespace sapphire::inline service {

    inline ClientInstance *getPrimaryClientInstance() {
        return ClientInstance::primaryClientInstance;
    }

    SDK_API inline Level *getServerLevel();

    SDK_API inline Level *getMultiPlayerLevel();

    SDK_API inline ServerInstance *getServerInstance();

    SDK_API inline LocalPlayer *getLocalPlayer();

    void init();
    void uninit();

} // namespace sapphire::inline service