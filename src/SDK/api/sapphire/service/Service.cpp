#include "Service.h"

#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src-client/common/client/player/LocalPlayer.h"
#include "SDK/api/src/common/certificates/Certificate.h"
#include "SDK/api/src/common/gamerefs/OwnerPtr.h"
#include "SDK/api/src/common/gamerefs/OwnerPtr.h"
#include "SDK/api/src/common/world/Minecraft.h"
#include "SDK/api/src/common/network/NetEventCallback.h"
#include "SDK/api/src/common/world/events/ServerInstanceEventCoordinator.h"

namespace {

    static Level          *clientLevel = nullptr;
    static Level          *serverLevel = nullptr;
    static ServerInstance *serverInstance = nullptr;
    static LocalPlayer    *localPlayer = nullptr;

    static Bedrock::NonOwnerPointer<Minecraft>        clientMinecraft;
    static Bedrock::NonOwnerPointer<NetEventCallback> legacyClientNetworkHandler;

    HOOK_TYPE(
        GetClientLevelHook,
        ClientInstance,
        sapphire::hook::HookPriority::Normal,
        ClientInstance::onClientCreatedLevel,
        void,
        std::pair<std::unique_ptr<Level>, OwnerPtr<EntityContext>> level,
        OwnerPtr<EntityContext>                                    player
    ) {
        clientLevel = level.first.get();
        this->origin(std::move(level), std::move(player));
    }

    HOOK_TYPE(
        GetServerLevelHook,
        ServerInstanceEventCoordinator,
        sapphire::hook::HookPriority::Normal,
        ServerInstanceEventCoordinator::sendServerLevelInitialized,
        void,
        ServerInstance &instance,
        Level          &level
    ) {
        serverInstance = &instance;
        serverLevel = &level;
        this->origin(instance, level);
    }

    HOOK_TYPE(
        GetLocalPlayerHook,
        LocalPlayer,
        sapphire::hook::HookPriority::Normal,
        LocalPlayer::ctor,
        LocalPlayer *,
        IClientInstance             &client,
        Level                       &level,
        const std::string           &displayName,
        GameType                     playerGameType,
        bool                         isHostingPlayer,
        const NetworkIdentifier     &owner,
        SubClientId                  subid,
        mce::UUID                    uuid,
        const std::string           &playFabId,
        const std::string           &deviceId,
        std::unique_ptr<Certificate> certificate,
        EntityContext               &entityContext
    ) {
        localPlayer = this;
        return this->origin(client, level, displayName, playerGameType, isHostingPlayer, owner, subid, uuid, playFabId, deviceId, std::move(certificate), entityContext);
    }

#if MC_VERSION < v1_21_60
    HOOK_TYPE(
        GetClientInstance,
        ClientInstance,
        sapphire::hook::HookPriority::Normal,
        ClientInstance::ctor,
        ClientInstance *,
        IMinecraftGame                                             &mg,
        IMinecraftApp                                              &app,
        LevelListener                                              &levelListener,
        SubClientId                                                 subid,
        Bedrock::NotNullNonOwnerPtr<IAdvancedGraphicsOptions>       graphicsOptions,
        Bedrock::NotNullNonOwnerPtr<ClientInstanceEventCoordinator> coordinator,
        LatencyGraphDisplay                                        *latencyGraphDisplay,
        Bedrock::NotNullNonOwnerPtr<NetworkSession>                 networkSession
    )
#else
    HOOK_TYPE(
        GetClientInstance,
        ClientInstance,
        sapphire::hook::HookPriority::Normal,
        ClientInstance::ctor,
        ClientInstance *,
        IMinecraftGame                                             &mg,
        IMinecraftApp                                              &app,
        LevelListener                                              &levelListener,
        SubClientId                                                 subid,
        Bedrock::NotNullNonOwnerPtr<IAdvancedGraphicsOptions>       graphicsOptions,
        Bedrock::NotNullNonOwnerPtr<ClientInstanceEventCoordinator> coordinator,
        LatencyGraphDisplay                                        *latencyGraphDisplay,
        Bedrock::NotNullNonOwnerPtr<NetworkSession>                 networkSession,
        __int64                                                     a10
    )
#endif
    {
        sapphire::debug("[service: primaryClientInstance found: {:#X}", (uintptr_t)this);
        ClientInstance::primaryClientInstance = this;
        return this->origin(mg, app, levelListener, subid, graphicsOptions, coordinator, latencyGraphDisplay, networkSession
#if MC_VERSION >= v1_21_60
                            ,
                            a10
#endif
        );
    }

    HOOK_TYPE(
        GetClientMinecraftHook,
        Minecraft,
        sapphire::hook::HookPriority::Normal,
        Minecraft::startClientGame,
        void,
        std::unique_ptr<NetEventCallback> legacyClientNetworkHandler
    ) {
        clientMinecraft = {this->mControlBlock, this};
        ::legacyClientNetworkHandler = {legacyClientNetworkHandler->mControlBlock, legacyClientNetworkHandler.get()};
        this->origin(std::move(legacyClientNetworkHandler));
    }

} // namespace

Level *sapphire::service::getServerLevel() {
    return serverLevel;
}

Level *sapphire::service::getMultiPlayerLevel() {
    return clientLevel;
}

ServerInstance *sapphire::service::getServerInstance() {
    return serverInstance;
}

LocalPlayer *sapphire::service::getLocalPlayer() {
    return localPlayer;
}

Bedrock::NonOwnerPointer<Minecraft> sapphire::service::getClientMinecraft() {
    return clientMinecraft;
}

Bedrock::NonOwnerPointer<NetEventCallback> sapphire::service::getLegacyClientNetworkHandler() {
    return legacyClientNetworkHandler;
}

void sapphire::service::init() {
    if (!GetClientLevelHook::hook())
        sapphire::error("service: GetClientLevelHook::hook failed!");
    if (!GetServerLevelHook::hook())
        sapphire::error("service: GetServerLevelHook::hook failed!");
    if (!GetLocalPlayerHook::hook())
        sapphire::error("service: GetLocalPlayerHook::hook failed!");
    if (!GetClientInstance::hook())
        sapphire::error("service: GetClientInstance::hook failed!");
    if (!GetClientMinecraftHook::hook())
        sapphire::error("service: GetClientMinecraftHook::hook failed!");
}

void sapphire::service::uninit() {
    GetClientMinecraftHook::unhook();
    GetClientInstance::unhook();
    GetLocalPlayerHook::unhook();
    GetServerLevelHook::unhook();
    GetClientLevelHook::unhook();
}
