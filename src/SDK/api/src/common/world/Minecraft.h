#pragma once

#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/gamerefs/OwnerPtr.h"
#include "SDK/api/src/common/entity/gamerefs_entity/IEntityRegistryOwner.h"
#include "SDK/api/src/common/network/ClientOrServerNetworkSystemRef.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class IMinecraftEventing;
class ResourcePackManager;
class StructureManager;
class GameModuleServer;
class AllowList;
class PermissionsFile;
class PrivateKeyManager;
class ServerMetrics;
class MinecraftCommands;
class GameSession;
class GameTestLevelListener;
class MinecraftGameTest;
class Timer;
class PacketSender;
class IMinecraftApp;
class NetEventCallback;
class IContentTierManager;
namespace Core {
    class FilePathManager;
}

// size: 8
class GameCallbacks {
public:
    // vtb+0
    virtual ~GameCallbacks();

    // vtb+1
    virtual void onLevelCorrupt() = 0;

    // vtb+2
    virtual void onCriticalScriptError(const char *, const char *) = 0;

    // vtb+3
    virtual void onGameModeChanged() = 0;

    // vtb+4
    virtual void onBeforeSimTick();

    // vtb+5
    virtual void onTick(int a, int b) = 0;

    // vtb+6
    virtual void onInternetUpdate() = 0;

    // vtb+7
    virtual void onGameSessionReset() = 0;

    // vtb+8
    virtual void onLevelExit() = 0;

    // vtb+9
    virtual void onRequestResourceReload() = 0;

    // vtb+10
    virtual void updateScreens() = 0;
};

// size: 296 (1.21.2), 304 (1.21.50/1.21.60)
class Minecraft : public IEntityRegistryOwner {
public:
    ;                                                                          // off+(1.21.50/1.21.60)
    GameCallbacks                                     &mGameCallbacks;         // off+24
    IMinecraftEventing                                &mEventing;              // off+32
    std::unique_ptr<ResourcePackManager>               mResourceLoader;        // off+40
    std::unique_ptr<StructureManager>                  mStructureManager;      // off+48
    std::shared_ptr<GameModuleServer>                  mGameModuleServer;      // off+56
    AllowList                                         &mAllowList;             // off+72
    PermissionsFile                                   *mPermissionsFile;       // off+80
    std::unique_ptr<PrivateKeyManager>                 mServerKeys;            // off+88
    const std::string                                  mSaveGamePath;          // off+96
    Bedrock::NotNullNonOwnerPtr<Core::FilePathManager> mFilePathManager;       // off+128
    ServerMetrics                                     *mServerMetrics;         // off+152
    bool                                               mCorruptionDetected;    // off+160
    bool                                               mFireOnLevelCorrupt;    // off+161
    double                                             mFrameDuration;         // off+168
    double                                             mLastFrameStart;        // off+176
    std::chrono::seconds                               mMaxPlayerIdleTime;     // off+184
    std::unique_ptr<MinecraftCommands>                 mCommands;              // off+192
    std::unique_ptr<GameSession>                       mGameSession;           // off+200
    std::unique_ptr<GameTestLevelListener>             mGameTestLevelListener; // off+208
    std::unique_ptr<MinecraftGameTest>                 mGameTest;              // off+216
    Timer                                             &mSimTimer;              // off+224
    Timer                                             &mRealTimer;             // off+232
    ClientOrServerNetworkSystemRef                     mNetwork;               // off+240
    PacketSender                                      &mPacketSender;          // off+256
    IMinecraftApp                                     &mApp;                   // off+264
    SubClientId                                        mClientSubId;           // off+272
    OwnerPtr<EntityRegistry>                           mEntityRegistry;        // off+280
    std::unique_ptr<void>                              mLevelSubscribers;      // off+296

    SDK_API Minecraft(
        IMinecraftApp                                                &app,
        GameCallbacks                                                &gameCallbacks,
        AllowList                                                    &allowList,
        PermissionsFile                                              *permissionsFile,
        const Bedrock::NotNullNonOwnerPtr<Core::FilePathManager>     &filePathManager,
        std::chrono::seconds                                          maxPlayerIdleTime,
        IMinecraftEventing                                           &eventing,
        ClientOrServerNetworkSystemRef                                network,
        PacketSender                                                 &packetSender,
        SubClientId                                                   clientSubId,
        Timer                                                        &simTimer,
        Timer                                                        &realTimer,
        const Bedrock::NotNullNonOwnerPtr<const IContentTierManager> &contentTierManager,
        ServerMetrics                                                *serverMetrics
    );
#pragma SDK_LINKER_SYM_ALIAS(                                                                                                                                                                                                                                                                                                                                                                                             \
    "??0Minecraft@@QEAA@AEAVIMinecraftApp@@AEAVGameCallbacks@@AEAVAllowList@@PEAVPermissionsFile@@AEBV?$not_null@V?$NonOwnerPointer@VFilePathManager@Core@@@Bedrock@@@gsl@@V?$duration@_JU?$ratio@$00$00@std@@@chrono@std@@AEAVIMinecraftEventing@@VClientOrServerNetworkSystemRef@@AEAVPacketSender@@EAEAVTimer@@9AEBV?$not_null@V?$NonOwnerPointer@$$CBVIContentTierManager@@@Bedrock@@@6@PEAVServerMetrics@@@Z",        \
    "?ctor@Minecraft@@QEAAPEAV1@AEAVIMinecraftApp@@AEAVGameCallbacks@@AEAVAllowList@@PEAVPermissionsFile@@AEBV?$not_null@V?$NonOwnerPointer@VFilePathManager@Core@@@Bedrock@@@gsl@@V?$duration@_JU?$ratio@$00$00@std@@@chrono@std@@AEAVIMinecraftEventing@@VClientOrServerNetworkSystemRef@@AEAVPacketSender@@EAEAVTimer@@9AEBV?$not_null@V?$NonOwnerPointer@$$CBVIContentTierManager@@@Bedrock@@@7@PEAVServerMetrics@@@Z" \
)

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x4D\x8B\xE1\x49\x8B\xD8\x4C\x8B\xEA")
    SPHR_DECL_API("1.21.50,1.21.60", "disp:7,call", "\x48\x8B\x55\x00\x48\x8B\xCF\xE8\x00\x00\x00\x00\xEB\x00\x48\x8B\xC3")
    SDK_API Minecraft *ctor(
        IMinecraftApp                                                &app,
        GameCallbacks                                                &gameCallbacks,
        AllowList                                                    &allowList,
        PermissionsFile                                              *permissionsFile,
        const Bedrock::NotNullNonOwnerPtr<Core::FilePathManager>     &filePathManager,
        std::chrono::seconds                                          maxPlayerIdleTime,
        IMinecraftEventing                                           &eventing,
        ClientOrServerNetworkSystemRef                                network,
        PacketSender                                                 &packetSender,
        SubClientId                                                   clientSubId,
        Timer                                                        &simTimer,
        Timer                                                        &realTimer,
        const Bedrock::NotNullNonOwnerPtr<const IContentTierManager> &contentTierManager,
        ServerMetrics                                                *serverMetrics
    );

    // vtb+0
    virtual ~Minecraft() override;

    // vtb+3
    virtual void setSimTimePause(bool pause);

    // vtb+4
    virtual void setSimTimeScale(float scale);

    // vtb+5
    virtual bool getSimPaused() const;

    // vtb+6
    virtual bool isOnlineClient() const;

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x4C\x24\x00\x48\x85\xC9\x74\x00\x48\x8B\x01\xBA\x00\x00\x00\x00\x48\x8B\x00\xFF\x15\x00\x00\x00\x00\x90\x48\x8B\x1F")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x8B\x00\x00\x00\x00\x48\x8B\x49\x00\xE8")
    SDK_API void startClientGame(std::unique_ptr<NetEventCallback> legacyClientNetworkHandler);
};