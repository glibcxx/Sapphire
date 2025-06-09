#include "Service.h"

#include "SDK/api/src/common/gamerefs/OwnerPtr.h"
#include "src/SDK/api/src/common/gamerefs/OwnerPtr.h"

namespace {

    static Level          *clientLevel = nullptr;
    static Level          *serverLevel = nullptr;
    static ServerInstance *serverInstance = nullptr;

    void onClientCreatedLevel(void *This, Level **level, void *localPlayer) {
        /*
            search "onClientCreatedLevel"
        */
        using Hook = core::ApiLoader <
#if MC_VERSION == v1_21_2
                     "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xF8\x4C\x8B\xFA\x4C\x8B\xF1\x48\x89\x95"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            "\x48\x89\x5C\x24\x00\x4C\x89\x44\x24\x00\x48\x89\x54\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xF8"_sig,
            &onClientCreatedLevel>;
#endif
              Hook::origin(This, level, localPlayer);
    }

    HOOK_STATIC(
        GetClientLevelHook,
        hook::HookPriority::Normal,
        onClientCreatedLevel,
        void,
        void   *This,
        Level **level,
        void   *localPlayer
    ) {
        clientLevel = *level;
        origin(This, level, localPlayer);
    }

    void ServerInstanceEventCoordinator__sendServerLevelInitialized(
        class ServerInstanceEventCoordinator *This, ServerInstance &serverInstance, Level &level
    ) {
        /*
            search "sendServerLevelInitialized"
        */
        using Hook = core::ApiLoader<
#if MC_VERSION == v1_21_2
            "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x41\x80\xBC\x24"_sig,
#elif MC_VERSION == v1_21_50
            "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x41\x80\xBD\x00\x00\x00\x00\x00\x0F\x84"_sig,
#elif MC_VERSION == v1_21_60
            "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x00\x8B\x85\x00\x00\x00\x00\x48\x8B\x88"_sig,
#endif
            &ServerInstanceEventCoordinator__sendServerLevelInitialized,
            core::deRefCall>;
        return Hook::origin(This, serverInstance, level);
    }

    HOOK_STATIC(
        GetServerLevelHook,
        hook::HookPriority::Normal,
        ServerInstanceEventCoordinator__sendServerLevelInitialized,
        void *,
        void           *This,
        ServerInstance &serverInstance,
        Level          &level
    ) {
        ::serverInstance = &serverInstance;
        serverLevel = &level;
        return origin(This, serverInstance, level);
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

void sapphire::service::init() {
    if (!GetClientLevelHook::hook())
        Logger::Error("[sapphire service] GetClientLevelHook::hook failed!");
    if (!GetServerLevelHook::hook())
        Logger::Error("[sapphire service] GetServerLevelHook::hook failed!");
}

void sapphire::service::uninit() {
    GetClientLevelHook::unhook();
    GetServerLevelHook::unhook();
}
