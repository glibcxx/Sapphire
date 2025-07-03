#include "Service.h"

#include "SDK/api/src/common/gamerefs/OwnerPtr.h"
#include "src/SDK/api/src/common/gamerefs/OwnerPtr.h"

class ServerInstanceEventCoordinator;

namespace {

    static Level          *clientLevel = nullptr;
    static Level          *serverLevel = nullptr;
    static ServerInstance *serverInstance = nullptr;
    static LocalPlayer    *localPlayer = nullptr;

    void onClientCreatedLevel(void *This, void *level, void *localPlayer) {
        /*
            search "onClientCreatedLevel"
        */
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xF8\x4C\x8B\xFA\x4C\x8B\xF1\x48\x89\x95"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            "\x48\x89\x5C\x24\x00\x4C\x89\x44\x24\x00\x48\x89\x54\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xF8"_sig,
#endif
            &onClientCreatedLevel>;
        Hook::origin(This, std::move(level), localPlayer);
    }

    HOOK_STATIC(
        GetClientLevelHook,
        sapphire::hook::HookPriority::Normal,
        onClientCreatedLevel,
        void,
        void *This,
        void *level,
        void *localPlayer
    ) {
        clientLevel = *(Level **)level;
        origin(This, std::move(level), localPlayer);
    }

    void ServerInstanceEventCoordinator__sendServerLevelInitialized(
        ServerInstanceEventCoordinator *This, ServerInstance &serverInstance, Level &level
    ) {
        /*
            search "sendServerLevelInitialized"
        */
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x41\x80\xBC\x24"_sig,
#elif MC_VERSION == v1_21_50
            "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x41\x80\xBD\x00\x00\x00\x00\x00\x0F\x84"_sig,
#elif MC_VERSION == v1_21_60
            "\xE8\x00\x00\x00\x00\x90\x48\x8D\x8D\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x00\x8B\x85\x00\x00\x00\x00\x48\x8B\x88"_sig,
#endif
            &ServerInstanceEventCoordinator__sendServerLevelInitialized,
            sapphire::deRefCall>;
        return Hook::origin(This, serverInstance, level);
    }

    HOOK_STATIC(
        GetServerLevelHook,
        sapphire::hook::HookPriority::Normal,
        ServerInstanceEventCoordinator__sendServerLevelInitialized,
        void,
        ServerInstanceEventCoordinator *This,
        ServerInstance                 &serverInstance,
        Level                          &level
    ) {
        ::serverInstance = &serverInstance;
        serverLevel = &level;
        origin(This, serverInstance, level);
    }

    LocalPlayer *LocalPlayer_ctor(
        LocalPlayer *This,
        void        *a2,
        void        *a3,
        void        *a4,
        void        *a5,
        void        *a6,
        void        *a7,
        void        *a8,
        void        *a9,
        void        *a10,
        void        *a11,
        void        *a12,
        void        *a13
    ) {
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            "\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x89\x33\x48\x8B\xC3\x48\x81\xC4\x00\x00\x00\x00\x41\x5F\x41\x5E\x41\x5D"_sig,
#elif MC_VERSION == v1_21_50
            "\xE8\x00\x00\x00\x00\x48\x8B\xF8\x48\x89\x3B\x48\x8B\xC3\x48\x8B\x9C\x24\x00\x00\x00\x00\x48\x81\xC4\x00\x00\x00\x00\x41"_sig,
#elif MC_VERSION == v1_21_60
            "\xE8\x00\x00\x00\x00\x48\x8B\xD8\x33\xFF\xEB\x00\x33\xFF\x8B\xDF\x48\x89\x5D"_sig,
#endif
            &LocalPlayer_ctor,
            sapphire::deRefCall>;
        return Hook::origin(This, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
    }

    HOOK_STATIC(
        GetLocalPlayerHook,
        sapphire::hook::HookPriority::Normal,
        LocalPlayer_ctor,
        LocalPlayer *,
        LocalPlayer *This,
        void        *a2,
        void        *a3,
        void        *a4,
        void        *a5,
        void        *a6,
        void        *a7,
        void        *a8,
        void        *a9,
        void        *a10,
        void        *a11,
        void        *a12,
        void        *a13
    ) {
        localPlayer = This;
        return origin(This, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13);
    }

    static void makeClientInstance(
        ClientInstance *ret, uint64_t a2, uint64_t a3, uint64_t a4, uint64_t a5, uint64_t a6, uint64_t a7, uint64_t a8, uint64_t a9, uint64_t a10
    ) {
        sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x49\x8B\xF9\x49\x8B\xF0\x48\x8B\xEA"_sig,
#elif MC_VERSION == v1_21_50
            "\x48\x89\x5C\x24\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x4C\x8B\xDA\x48\x8B\xD9\x4C\x8B\x94\x24"_sig,
#elif MC_VERSION == v1_21_60
            "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x68\x00\x48\x89\x70\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x49\x8B\xD9"_sig,
#endif
            &makeClientInstance>::origin(ret, a2, a3, a4, a5, a6, a7, a8, a9, a10);
    }

    HOOK_STATIC(
        GetClientInstance,
        sapphire::hook::HookPriority::Normal,
        makeClientInstance,
        void,
        ClientInstance *ret,
        uint64_t        a2,
        uint64_t        a3,
        uint64_t        a4,
        uint64_t        a5,
        uint64_t        a6,
        uint64_t        a7,
        uint64_t        a8,
        uint64_t        a9,
        uint64_t        a10
    ) {
        origin(ret, a2, a3, a4, a5, a6, a7, a8, a9, a10);
        ClientInstance::primaryClientInstance = ret;
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

void sapphire::service::init() {
    if (!GetClientLevelHook::hook())
        Logger::Error("[sapphire service] GetClientLevelHook::hook failed!");
    if (!GetServerLevelHook::hook())
        Logger::Error("[sapphire service] GetServerLevelHook::hook failed!");
    if (!GetLocalPlayerHook::hook())
        Logger::Error("[sapphire service] GetLocalPlayerHook::hook failed!");
    if (!GetClientInstance::hook())
        Logger::Error("[sapphire service] GetClientInstance::hook failed!");
}

void sapphire::service::uninit() {
    GetClientLevelHook::unhook();
    GetServerLevelHook::unhook();
    GetLocalPlayerHook::unhook();
    GetClientInstance::unhook();
}
