#pragma once

#include "SDK/api/src-deps/Core/File/StorageAreaStateListener.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Core/Utility/ServiceRegistrationToken.h"
#include "SDK/api/src/common/AppPlatformListener.h"
#include "SDK/api/src/common/world/Minecraft.h"
#include "SDK/api/src-deps/Core/Threading/SPSCQueue.h"

class LoopbackPacketSender;
class Scheduler;
class EducationOptions;
class LevelStorage;
class ServerInstanceEventCoordinator;
class ServerScriptManager;
class TextFilteringProcessor;
class WorldSessionEndPoint;
class ItemRegistry;
class BlockTypeRegistry;
class CDNConfig;
class ServerTextSettings;
namespace Editor {
    class IEditorManager;
}
#if MC_VERSION >= v1_21_50
namespace Scripting {
    class RegistryManager;
}
namespace cereal {
    struct ReflectionCtx;
}
#endif

// https://github.com/LiteLDev/LeviLamina/blob/565421538b623fef8d5cf2f96450809fb9b94613/src/mc/server/ServerInstance.h#L62

// size: 912 (1.21.2), 968 (1.21.50), 984 (1.21.60)
class ServerInstance
    : public Bedrock::EnableNonOwnerReferences,
      public AppPlatformListener,
      public GameCallbacks,
      public Core::StorageAreaStateListener {
public:
    enum class InstanceState : int {
        Running = 0,
        Suspended = 1,
        WaitingLeaveGame = 2,
        Stopped = 3,
        NotStarted = 4,
    };

    std::chrono::steady_clock::time_point                       mLastSyncTime;         // off+152
    const IMinecraftApp                                        &mApp;                  // off+160
    std::unique_ptr<Minecraft>                                  mMinecraft;            // off+168
    std::unique_ptr<ServerNetworkSystem>                        mNetwork;              // off+176
    std::unique_ptr<LoopbackPacketSender>                       mPacketSender;         // off+184
    std::unique_ptr<Timer>                                      mSimTimer;             // off+192
    std::unique_ptr<Timer>                                      mRealTimer;            // off+200
    std::unique_ptr<Scheduler>                                  mScheduler;            // off+208
    std::unique_ptr<EducationOptions>                           mEducationOptions;     // off+216
    LevelStorage                                               *mStorage;              // off+224
    std::atomic<bool>                                           mInUpdate;             // off+232
    std::atomic<int>                                            mWriteRefCounter;      // off+236
    std::atomic<bool>                                           mThreadShouldJoin;     // off+240
    Bedrock::Threading::Mutex                                   mMutexDestruction;     // off+248
    Bedrock::NotNullNonOwnerPtr<ServerInstanceEventCoordinator> mEventCoordinator;     // off+328
    std::atomic<InstanceState>                                  mInstanceState;        // off+352
    SPSCQueue<std::function<void()>, 512>                       mCommandQueue;         // off+360
    std::thread                                                 mServerInstanceThread; // off+440
    Bedrock::Threading::Mutex                                   mResumeMutex;          // off+456
    std::condition_variable                                     mResumeSignal;         // off+536
    std::unique_ptr<ServerScriptManager>                        mServerScriptManager;  // off+608
#if MC_VERSION >= v1_21_50
    std::unique_ptr<Scripting::RegistryManager> mScriptRegistryManager; // off+616
#endif
    std::function<void(const char *)>               mScriptWatchdogCriticalErrorCallback; // off+642
    std::function<void(const char *, const char *)> mUnrecoverableErrorCallback;          // off+688
    bool                                            mHandledLevelCorruption;              // off+752
    std::unique_ptr<TextFilteringProcessor>         mTextFilteringProcessor;              // off+760
    std::chrono::microseconds                       mWakeupInterval;                      // off+768
    std::string                                     mLevelId;                             // off+776
    std::unique_ptr<WorldSessionEndPoint>           mWorldSessionEndPoint;                // off+808
    std::shared_ptr<Core::FileStorageArea>          mStorageAreaForLevel;                 // off+816
    std::shared_ptr<ItemRegistry>                   mServerItemRegistry;                  // off+832
    std::shared_ptr<BlockTypeRegistry>              mBlockRegistry;                       // off+848
    bool                                            mEnableItemStackNetManager;           // off+864
    bool                                            mbInitialized;                        // off+865
    bool                                            mbFlaggedForEarlyDestruction;         // off+866
    ServiceRegistrationToken<ServerInstance>        mServiceRegistrationToken;            // off+872
    bool                                            mHasScheduledLeaveGame;               // off+880
    bool                                            mCriticalDiskErrorTelemetrySent;      // off+881
    std::unique_ptr<Editor::IEditorManager>         mEditorManager;                       // off+888
    Bedrock::NonOwnerPointer<CDNConfig>             mCDNConfig;                           // off+896
    Bedrock::NonOwnerPointer<ServerTextSettings>    mServerTextSettings;                  // off+920
#if MC_VERSION >= v1_21_50
    Bedrock::NotNullNonOwnerPtr<cereal::ReflectionCtx> mCerealContext; // off+944
#endif

    SDK_API ServerInstance(
        IMinecraftApp &app, const Bedrock::NotNullNonOwnerPtr<ServerInstanceEventCoordinator> &coordinator
    );
#pragma SPHR_LINKER_SYM_ALIAS(                                                                                                                \
    "??0ServerInstance@@QEAA@AEAVIMinecraftApp@@AEBV?$not_null@V?$NonOwnerPointer@VServerInstanceEventCoordinator@@@Bedrock@@@gsl@@@Z",       \
    "?ctor@ServerInstance@@QEAAPEAV1@AEAVIMinecraftApp@@AEBV?$not_null@V?$NonOwnerPointer@VServerInstanceEventCoordinator@@@Bedrock@@@gsl@@@" \
)

    SPHR_DECL_API("1.21.2", "disp:6,call", "\x4D\x8B\xC6\x48\x8B\xCB\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x89\x37\x48\x8B\xC7\x48\x8B\x5C\x24\x00\x48\x8B\x6C\x24\x00\x48\x83\xC4\x00\x41\x5E\x5F\x5E\xC3\xCC\xCC\xCC\xCC\xCC")
    SPHR_DECL_API("1.21.50,1.21.60", "disp:6,call", "\x4C\x8B\xC6\x48\x8B\xCB\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x48\x8D\x8D")
    SDK_API ServerInstance *ctor(
        IMinecraftApp &app, const Bedrock::NotNullNonOwnerPtr<ServerInstanceEventCoordinator> &coordinator
    );
};