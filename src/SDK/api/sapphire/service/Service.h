#pragma once

#include "pch.h" // IWYU pragma: keep

#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src-client/common/client/player/LocalPlayer.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class ServerInstance;

namespace sapphire::inline service {

    inline ClientInstance *getPrimaryClientInstance() {
        return ClientInstance::primaryClientInstance;
    }

    SPHR_API Level *getServerLevel();

    SPHR_API Level *getMultiPlayerLevel();

    SPHR_API ServerInstance *getServerInstance();

    SPHR_API LocalPlayer *getLocalPlayer();

    SPHR_API Bedrock::NonOwnerPointer<Minecraft> getClientMinecraft();

    SPHR_API Bedrock::NonOwnerPointer<NetEventCallback> getLegacyClientNetworkHandler();

    void init();
    void uninit();

} // namespace sapphire::inline service