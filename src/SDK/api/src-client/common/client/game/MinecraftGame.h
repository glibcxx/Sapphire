#pragma once

#include "SDK/core/SymbolResolver.h"
#include "IMinecraftGame.h"
#include "SDK/api/src/common/App.h"
#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/world/level/LevelListener.h"
#include "SDK/api/src/common/util/Timer.h"
#include "SDK/api/src/common/resources/ResourcePackListener.h"
#include "SDK/api/src/common/certificates/edu/CredentialsObserver.h"
#include "SDK/api/src/common/locale/I18nObserver.h"
#include "SDK/api/src/common/entity/gamerefs_entity/EntityContext.h"
#include "SDK/api/src/common/world/level/dimension/DimensionType.h"
#include "SDK/api/src-client/common/client/options/OptionsObserver.h"
#include "SDK/api/src-client/common/client/game/IDynamicPackageConsumer.h"
#include "SDK/api/src-client/common/client/gui/FontHandle.h"
#include "SDK/api/src-client/common/client/resources/PackDownloadManager.h"
#include "SDK/api/src-client/common/client/util/ScreenshotRecorder.h"
#include "SDK/api/src-deps/Core/Utility/ServiceRegistrationToken.h"
#include "SDK/api/src-deps/Core/Utility/SerialWorkList.h"
#include "SDK/api/src-deps/Core/Utility/UUID.h"

class ContentLog;
class GuiContentLogEndPoint;
class IClientInstance;
class GlobalResourcesCrashRecovery;
class MinecraftGraphics;
class ExternalServerFile;
class FontRepository;
class EmoticonManager;
class TextToIconMapper;
class UIProfanityContext;
class MinecraftUIMeasureStrategy;
class ResourceLoadManager;
class ClientInstanceEventCoordinator;
class UIDefRepository;
class NetworkSession;
class ClientNetworkSystem;
class LevelDbEnv;
class ActorAnimationControllerGroup;
class RenderControllerGroup;
class AppConfigs;
class FlightingService;
class OfferRepository;
class BrazeSDKManager;
class IntegrityService;
class SunsettingManager;
class LevelLoader;
class HolosceneRenderer;
class SaveTransactionManager;
class HolographicPlatform;
class IDlcValidation;
class RayTracingHardwareOptions;
class ContentTierManager;
class RayTracingOptions;
class GeometryGroup;
class BlockCullingGroup;
class ParticleEffectGroup;
class DeferredLighting;
class GenericEntitlementChangeListener;
class WebSocketCommManager;
class ResetCallbackObject;
class PixelCalc;
class ActorAnimationGroup;
class DateManager;
class IContentAccessibilityProvider;
class PackManifestFactory;
class WorldTemplateManager;
class ActorResourceDefinitionGroup;
class DevConsoleLogger;
class IGameModuleApp;
class SkinRepository;
class ContentCatalogService;
class StoreCatalogRepository;
class PersonaRepository;
class PersonaService;
class PackSourceFactory;
class ResourcePackRepository;
class EcoModeSystem;
class ServerInstanceEventCoordinator;
class ServerInitData;
class TextureHotReloader;
class CubemapBackgroundResources;
class OculusPlatformMessagePump;
class ProfilingManager;
class ServiceDrivenImageRepository;
class CloudFileUploadManager;
class ContentLogFileEndPoint;
class RealmsAPI;
class ActiveDirectoryIdentity;
class MinecraftInputHandler;
class CommandListQueue;
class SeasonsRenderer;
class TextureAtlas;
class GameRenderer;
class UIRepository;
class ThirdPartyServerRepository;
class PlayerMessagingService;
class ServicesManager;
class CDNService;
class TrialManager;
class TreatmentPackDownloadMonitor;
class EDUSystems;
class ServerInstance;
class TaskGroup;
class SoundEngine;
class ContentAcquisition;
class TextToSpeechSystem;
class MusicManager;
class ExternalContentManager;
class LibraryRepository;
class LevelStorageSource;
class LevelListCache;
class NewPlayerSystem;
class ContentManager;
class MarketplaceServicesManager;
class IMinecraftEventing;
class ServiceEntitlementManager;
class GatheringManager;
class SceneStack;
class MainChunkSource;
class CachedScenes;
class WatchdogTimer;
class ChunkPerformanceData;
class ScopedCPUBoost;
template <typename T>
class DeferredTasksManager;
namespace World {
    class System;
}
namespace mce {
    class TextureGroup;
}
namespace Core {
    class FilePathManager;
}
namespace ClientBlobCache {
    class Cache;
}
namespace OreUI {
    class IResourceAllowList;
}
namespace Social {
    class UserManager;
    class MultiplayerServiceManager;
    class System;
    class PresenceManager;
} // namespace Social
namespace ClientBlockPipeline {
    class SchematicsRepository;
}
namespace OreUI {
    class Router;
    class DataProviderManager;
} // namespace OreUI
namespace Realms {
    class RealmsServices;
    class RealmsSystem;
} // namespace Realms
namespace cereal {
    struct ReflectionCtx;
}
namespace Bedrock::Threading {
    class PendingConditionals;
}
namespace mce {
    class FileWatcherNull;
}
namespace mce::framebuilder {
    class FrameBuilder;
}
namespace Bedrock::Http {
    class DispatchQueue;
}
namespace OreUI {
    class System;
}

enum class TestCommandType : int;
enum class TestAssetCommandType : int;

#if MC_VERSION == v1_21_2
// size: 16
class Unk4CB6580 : public Core::Observer<Unk4CB6580, Core::SingleThreadedLock> {
    // vtb+0
    virtual ~Unk4CB6580();

    // vtb+2
    virtual void unk_virtual_2(__int64 a1, __int64 a2);
};
#endif

class AppSystemRegistry {
public:
    std::shared_ptr<void>              mUnk0;
    std::vector<std::shared_ptr<void>> mUnk16;
};

// https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/client/game/IMinecraftGame.h#L157

// size: 5512 (1.21.2), 5424 (1.21.50), 5248 (1.21.60)
class MinecraftGame : public IMinecraftGame,
                      public App,
                      public LevelListener,
                      public ResourcePackListener,
                      public OptionsObserver,
                      public edu::auth::CredentialsObserver,
#if MC_VERSION <= v1_21_50
                      public IDynamicPackageConsumer,
#endif
#if MC_VERSION == v1_21_2
                      public Unk4CB6580,
#endif
                      public I18nObserver {
public:
    using ButtonPressHandler = std::function<void()>;
    using ButtonHandlerMap = std::unordered_map<uint32_t, ButtonPressHandler>;

    enum class DeferredTaskCategory : int {
        OnFrameStart = 0,
        OnFrameEnd = 1,
        OnUserManagerReady = 2,
        OnPrimaryClientReady = 3,
        AfterSwapBuffers = 4,
    };

    enum class SuspendState : int {
        Suspending = 0,
        Suspended = 1,
        Resuming = 2,
        Running = 3,
    };

    // size: 32
    struct InitContext {
        std::shared_ptr<IClientInstance> mIncompletePrimaryClient; // off+0
        std::shared_ptr<ScopedCPUBoost>  mCPUBoost;                // off+16
    };

    std::shared_ptr<void>                         mUnk216;                             // off+216
    std::function<void()>                         mUnk232;                             // off+232
    std::unique_ptr<ContentLog>                   mContentLog;                         // off+296
    ServiceRegistrationToken<ContentLog>          mContentLogServiceRegistrationToken; // off+304
    std::unique_ptr<GuiContentLogEndPoint>        mGuiContentLogEndPoint;              // off+312
    std::unique_ptr<__int64>                      mUnk320;                             // off+320
    __int64                                       mUnk328;                             // off+328
    std::unique_ptr<__int64>                      mUnk336;                             // off+336
    std::unique_ptr<GlobalResourcesCrashRecovery> mGlobalResourcesCrashRecovery;       // off+344
    std::unique_ptr<MinecraftGraphics>            mMinecraftGraphics;                  // off+352
    std::unique_ptr<__int64>                      mUnk360;                             // off+360
    std::unique_ptr<ExternalServerFile>           mExternalServer;                     // off+368
    std::unique_ptr<FontRepository>               mFontRepository;                     // off+376
    std::unique_ptr<EmoticonManager>              mEmoticonManager;                    // off+384
    std::unique_ptr<TextToIconMapper>             mTextToIconMapper;                   // off+392
    std::unique_ptr<UIProfanityContext>           mUIProfanityContext;                 // off+400
    std::unique_ptr<MinecraftUIMeasureStrategy>   mMeasureStrategy;                    // off+408
    bool                                          mMouseGrabbed;                       // off+416
    ButtonHandlerMap                              mButtonDownHandlerMap;               // off+424
    ButtonHandlerMap                              mButtonUpHandlerMap;                 // off+488
    std::set<SubClientId>                         mAvailableClientSubIds;              // off+552

    std::function<void(TestCommandType, const std::vector<std::string> &, int)> mTestExecuteCommandCallback; // off+568
    std::function<void(TestAssetCommandType, const std::vector<std::string> &)> mTestAssetCommandCallback;   // off+632

    std::unique_ptr<RayTracingHardwareOptions>                 mGraphicHardwareOptions;                 // off+696
    std::unique_ptr<ContentTierManager>                        mContentTierManager;                     // off+704
    std::unique_ptr<RayTracingOptions>                         mRayTracingOptions;                      // off+712
    ResourcePackManager                                       *mResourcePackManager;                    // off+720
    std::shared_ptr<GeometryGroup>                             mGeometryGroup;                          // off+728
    std::unique_ptr<BlockCullingGroup>                         mBlockCullingGroup;                      // off+744
    std::unique_ptr<ClientBlockPipeline::SchematicsRepository> mSchematicsRepository;                   // off+752
    std::unique_ptr<ParticleEffectGroup>                       mParticleEffectGroup;                    // off+760
    std::unique_ptr<__int64>                                   mUnk768;                                 // off+768
    std::unique_ptr<DeferredLighting>                          mDeferredLighting;                       // off+776
    std::string                                                mUnk784;                                 // off+784
    std::unique_ptr<__int64>                                   mUnk816;                                 // off+816
    std::unique_ptr<__int64>                                   mUnk824;                                 // off+824
    std::shared_ptr<GenericEntitlementChangeListener>          mEntitlementChangeListener;              // off+832
    std::shared_ptr<GenericEntitlementChangeListener>          mEntitlementChangeGameEventNotification; // off+848
    bool                                                       mHasUnhandledEntitlementsChangeEvent;    // off+864
    std::unique_ptr<WebSocketCommManager>                      mWebSocketCommunicatorManager;           // off+872
    std::set<SubClientId>                                      mSubclientRemovalSet;                    // off+880
    std::atomic<SuspendState>                                  mSuspended;                              // off+900
    bool                                                       mUnk904;                                 // off+904
    int                                                        mUnk908;                                 // off+908
    ResetCallbackObject                                       *mResetCallbackObj;                       // off+912
    std::unique_ptr<PixelCalc>                                 mPixelCalc;                              // off+920
    mce::UUID                                                  mUUID;                                   // off+928
    std::string                                                mProcessRegistrationKey;                 // off+944
    Bedrock::Threading::IAsyncResult<void>::Handle             mCheckLoadRendererAssetsCompleteHandle;  // off+976
    std::atomic<bool>                                          mIsFileSystemSpaceTrackingComplete;      // off+992
    std::atomic<bool>                                          mIsLevelInfoRepopulationComplete;        // off+993
    __int64                                                    mUnk1000[4];                             // off+1000
    __int8                                                     mUnk1032;                                // off+1032
    uint32_t                                                   mUIRenderClientMask;                     // off+1036
    uint32_t                                                   mUIRenderIssuedMask;                     // off+1040
    bool                                                       mTickedLastFrame;                        // off+1044
    bool                                                       mInitialResourcesLoaded;                 // off+1045
    bool                                                       mReadyToRender;                          // off+1046
    bool                                                       mGenerateDocs;                           // off+1047
    bool                                                       mLaunchedFromLegacyVersion;              // off+1048
    bool                                                       mUnk1049;                                // off+1049
    uint32_t                                                   mLeaveGameProgress;                      // off+1052
    PackDownloadManager                                        mPackDownloadManager;                    // off+1056
    std::atomic<bool>                                          mIsInGame;                               // off+1512
    std::atomic<LocalServerStartupState>                       mLocalServerStartupState;                // off+1516
    std::shared_ptr<bool>                                      mDeferHandleInvite;                      // off+1520
    bool                                                       mInitFinished;                           // off+1536
    bool                                                       mHasDestroyedGame;                       // off+1537
    bool                                                       mAppWillTerminate;                       // off+1538
    bool                                                       mFiredInitialEvents;                     // off+1539
    bool                                                       mForceReloadResources;                   // off+1540
    bool                                                       mRunningInTestCloud;                     // off+1541
    bool                                                       mProfilerIsOn;                           // off+1542
    int                                                        mScreenShotCount;                        // off+1544

    char _fill1548[1616 - 1548]; // off+1548

    ScreenshotRecorder                                     mScreenshotRecorder;                            // off+1616
    std::unique_ptr<__int64>                               mUnk1696;                                       // off+1696
    std::weak_ptr<ClientBlobCache::Cache>                  mClientBlobCache;                               // off+1704
    bool                                                   mUnk1720;                                       // off+1720
    bool                                                   mUnk1721;                                       // off+1721
    bool                                                   minitHoloPosComplete;                           // off+1722
    bool                                                   mInitPrimaryUserComplete;                       // off+1723
    __int16                                                mUnk1724;                                       // off+1724
    std::shared_ptr<void>                                  mUnk1728;                                       // off+1728
    AppSystemRegistry                                      mAppSystemRegistry;                             // off+1744
    std::optional<EntityContext>                           mEntityContext;                                 // off+1784
    bool                                                   mUnk1816;                                       // off+1816
    bool                                                   mNewLevel;                                      // off+1817
    std::shared_ptr<void>                                  mUnk1824;                                       // off+1824
    std::unique_ptr<DateManager>                           mDateManager;                                   // off+1840
    ServiceRegistrationToken<DateManager>                  mDateManagerServiceRegistrationToken;           // off+1848
    std::unique_ptr<ResourceLoadManager>                   mResourceLoadManager;                           // off+1856
    ServiceRegistrationToken<ResourceLoadManager>          mResourceLoadManagerServiceRegistrationToken;   // off+1864
    std::unique_ptr<ClientInstanceEventCoordinator>        mClientInstanceEventCoordinator;                // off+1872
    std::unique_ptr<Social::UserManager>                   mUserManager;                                   // off+1880
    std::shared_ptr<mce::TextureGroup>                     mTextures;                                      // off+1888
    std::shared_ptr<Bedrock::Http::DispatchQueue>          mHttpDispatchQueue;                             // off+1904
    ServiceRegistrationToken<Bedrock::Http::DispatchQueue> mHttpDispatchQueueServiceRegistrationToken;     // off+1920
    std::unique_ptr<IMinecraftEventing>                    mEventing;                                      // off+1928
    std::unique_ptr<ServiceEntitlementManager>             mEntitlementManager;                            // off+1936
    std::unique_ptr<Core::FilePathManager>                 mFilePathManager;                               // off+1944
    std::unique_ptr<IContentAccessibilityProvider>         mContentKeyProvider;                            // off+1952
    std::unique_ptr<PackManifestFactory>                   mManifestFactory;                               // off+1960
    std::unique_ptr<WorldTemplateManager>                  mWorldTemplateManager;                          // off+1968
    std::unique_ptr<ActorResourceDefinitionGroup>          mActorResourceDefinitionGroup;                  // off+1976
    std::unique_ptr<DevConsoleLogger>                      mDevConsoleLogger;                              // off+1984
    std::unique_ptr<IGameModuleApp>                        mGameModule;                                    // off+1992
    std::shared_ptr<SkinRepository>                        mSkinRepository;                                // off+2000
    std::unique_ptr<ContentCatalogService>                 mContentCatalogService;                         // off+2016
    ServiceRegistrationToken<ContentCatalogService>        mContentCatalogServiceServiceRegistrationToken; // off+2024
    std::unique_ptr<StoreCatalogRepository>                mStoreCatalogRepository;                        // off+2032
    std::unique_ptr<PersonaRepository>                     mPersonaRepository;                             // off+2040
    std::unique_ptr<PersonaService>                        mPersonaService;                                // off+2048
    std::unique_ptr<PackSourceFactory>                     mPackSourceFactory;                             // off+2056
    std::unique_ptr<ResourcePackRepository>                mResourcePackRepository;                        // off+2064
    std::function<void()>                                  mUnk2072;                                       // off+2072
    std::unique_ptr<SoundEngine>                           mSoundEngine;                                   // off+2136
    std::unique_ptr<ContentAcquisition>                    mContentAcquisition;                            // off+2144
    std::unique_ptr<UIDefRepository>                       mUIDefRepo;                                     // off+2152
    std::unique_ptr<NetworkSession>                        mNetworkSession;                                // off+2160
    std::unique_ptr<ClientNetworkSystem>                   mClientNetworkSystem;                           // off+2168
    std::unique_ptr<LevelDbEnv>                            mLevelDbEnv;                                    // off+2176
    std::shared_ptr<ActorAnimationGroup>                   mActorAnimationGroup;                           // off+2184
    std::shared_ptr<ActorAnimationControllerGroup>         mActorAnimationControllerGroup;                 // off+2200
    std::shared_ptr<RenderControllerGroup>                 mRenderControllerGroup;                         // off+2216
    ServiceRegistrationToken<ServiceEntitlementManager>    mEntitlementManagerServiceRegistrationToken;    // off+2232
    std::unique_ptr<mce::FileWatcherNull>                  mFileWatcher;                                   // off+2240
    ServiceRegistrationToken<mce::FileWatcherNull>         mFileWatcherServiceRegistrationToken;           // off+2248
    std::unique_ptr<AppConfigs>                            mAppConfig;                                     // off+2256
    ServiceRegistrationToken<AppConfigs>                   mAppConfigServiceRegistrationToken;             // off+2264
    std::unique_ptr<__int64 /* 168 */>                     mUnk2272;                                       // off+2272
    ServiceRegistrationToken<void>                         mUnk2280;                                       // off+2280
    std::unique_ptr<mce::framebuilder::FrameBuilder>       mFrameBuilder;                                  // off+2288
    ServiceRegistrationToken<mce::framebuilder::FrameBuilder>
        mFrameBuilderServiceRegistrationToken; // off+2296

    std::unique_ptr<__int64 /* 416 */>                      mUnk2304;                // off+2304
    ServiceRegistrationToken<void>                          mUnk2312;                // off+2312
    std::unique_ptr<__int64 /* 2320 */>                     mUnk2320;                // off+2320
    ServiceRegistrationToken<void>                          mUnk2328;                // off+2328
    std::shared_ptr<FlightingService>                       mFlightingService;       // off+2336
    std::unique_ptr<OreUI::System>                          mOreUISystem;            // off+2352
    std::map<SubClientId, std::shared_ptr<IClientInstance>> mClientInstances;        // off+2360
    std::unique_ptr<__int64>                                mUnk2376;                // off+2376
    std::unique_ptr<__int64>                                mUnk2384;                // off+2384
    Bedrock::PubSub::Subscription                           mUnk2392;                // off+2392
    Bedrock::PubSub::Subscription                           mUnk2408;                // off+2408
    std::unique_ptr<OfferRepository>                        mOfferRepository;        // off+2424
    std::shared_ptr<BrazeSDKManager>                        mBrazeSDKManager;        // off+2432
    std::unique_ptr<IntegrityService>                       mIntegrityService;       // off+2448
    std::unique_ptr<SunsettingManager>                      mSunsettingManager;      // off+2456
    std::unique_ptr<LevelLoader>                            mLevelLoader;            // off+2464
    std::unique_ptr<HolosceneRenderer>                      mHolosceneRenderer;      // off+2472
    std::function<void()>                                   mUnk2480;                // off+2480
    std::unique_ptr<__int64>                                mUnk2544;                // off+2544
    std::unique_ptr<SaveTransactionManager>                 mSaveTransactionManager; // off+2552
    std::unique_ptr<HolographicPlatform>                    mPrimaryClientHoloInput; // off+2560
    std::unique_ptr<IDlcValidation>                         mDlcValidation;          // off+2568
    std::unique_ptr<__int64>                                mUnk2576;                // off+2576
    std::function<void()>                                   mUnk2584;                // off+2584
    char                                                    mUnk2648;                // off+2648

    std::unique_ptr<EcoModeSystem>                                      mEcoModeSystem;                  // off+2656
    std::unique_ptr<ServerInstanceEventCoordinator>                     mServerInstanceEventCoordinator; // off+2664
    std::unique_ptr<ServerInitData>                                     mServerInitData;                 // off+2672
    Bedrock::Threading::IAsyncResult<void>::Handle                      mServerStartHandle;              // off+2680
    std::unique_ptr<__int64>                                            mUnk2696;                        // off+2696
    std::unordered_map<DimensionType, std::unique_ptr<MainChunkSource>> mClientGenChunkSource;           // off+2704

    std::unique_ptr<Timer>                             mSimTimer;                                 // off+2768
    std::unique_ptr<Timer>                             mRealTimer;                                // off+2776
    std::function<void()>                              mUnk2784;                                  // off+2784
    std::function<void()>                              mUnk2848;                                  // off+2848
    std::unique_ptr<TextureHotReloader>                mTextureHotReloader;                       // off+2912
    std::unique_ptr<CubemapBackgroundResources>        mCubemapBackgroundResources;               // off+2920
    std::unique_ptr<__int64>                           mUnk2928;                                  // off+2928
    std::unique_ptr<OculusPlatformMessagePump>         mOculusPlatformMessagePump;                // off+2936
    std::function<void()>                              mUnk2944;                                  // off+2944
    std::function<void()>                              mUnk3008;                                  // off+3008
    std::unique_ptr<ProfilingManager>                  mProfilingManager;                         // off+3072
    ServiceRegistrationToken<ProfilingManager>         mProfilingManagerServiceRegistrationToken; // off+3080
    std::unique_ptr<ServiceDrivenImageRepository>      mServiceDrivenImageRepository;             // off+3088
    std::function<void()>                              mUnk3096;                                  // off+3096
    std::unique_ptr<CloudFileUploadManager>            mCloudFileUploadManager;                   // off+3160
    std::unique_ptr<ContentLogFileEndPoint>            mContentLogFileEndPoint;                   // off+3168
    std::shared_ptr<RealmsAPI>                         mRealms;                                   // off+3176
    std::unique_ptr<ActiveDirectoryIdentity>           mActiveDirectoryIdentity;                  // off+3192
    std::unique_ptr<MinecraftInputHandler>             mInput;                                    // off+3200
    std::function<void()>                              mUnk3208;                                  // off+3208
    std::unique_ptr<CommandListQueue>                  mCommandListQueue;                         // off+3272
    std::unique_ptr<SeasonsRenderer>                   mSeasonsRenderer;                          // off+3280
    std::unique_ptr<TextureAtlas>                      mTextureAtlas;                             // off+3288
    std::unique_ptr<GameRenderer>                      mGameRenderer;                             // off+3312
    std::unique_ptr<UIRepository>                      mUIRepository;                             // off+3304
    std::unique_ptr<__int64>                           mUnk3412;                                  // off+3412
    std::function<void()>                              mUnk3320;                                  // off+3320
    std::unique_ptr<ThirdPartyServerRepository>        mThirdPartyServerRepository;               // off+3384
    std::unique_ptr<PlayerMessagingService>            mPlayerMessagingService;                   // off+3392
    std::unique_ptr<CDNService>                        mCDNService;                               // off+3400
    std::unique_ptr<__int64>                           mUnk3408;                                  // off+3408
    std::shared_ptr<ServicesManager>                   mServicesManager;                          // off+3416
    ServiceRegistrationToken<ServicesManager>          mServicesManagerServiceRegistrationToken;  // off+3432
    std::shared_ptr<MarketplaceServicesManager>        mMarketplaceServicesManager;               // off+3440
    std::unique_ptr<TrialManager>                      mTrialManager;                             // off+3456
    std::unique_ptr<Social::PresenceManager>           mPresenceManager;                          // off+3464
    std::unique_ptr<TreatmentPackDownloadMonitor>      mTreatmentPackMonitor;                     // off+3472
    std::shared_ptr<mce::TextureGroup>                 mStoreCacheTextures;                       // off+3480
    std::unique_ptr<TextToSpeechSystem>                mTTSSystem;                                // off+3496
    std::unique_ptr<MusicManager>                      mMusicManager;                             // off+3504
    std::unique_ptr<ExternalContentManager>            mContentManager;                           // off+3512
    std::unique_ptr<__int64>                           mUnk3520;                                  // off+3520
    std::unique_ptr<LibraryRepository>                 mLibraryRepository;                        // off+3528
    std::unique_ptr<LevelStorageSource>                mLevelStorageSource;                       // off+3536
    std::unique_ptr<LevelListCache>                    mLevelListCache;                           // off+3544
    std::unique_ptr<NewPlayerSystem>                   mNewPlayerSystem;                          // off+3552
    std::shared_ptr<ContentManager>                    mResourceContentManager;                   // off+3560
    std::unique_ptr<World::System>                     mWorldSystem;                              // off+3576
    std::unique_ptr<Social::System>                    mSocialSystem;                             // off+3584
    std::function<void()>                              mUnk3592;                                  // off+3592
    std::function<void()>                              mUnk3656;                                  // off+3656
    FontHandle                                         mFontHandle;                               // off+3720
    FontHandle                                         mRuneFontHandle;                           // off+3800
    FontHandle                                         mUnicodeFontHandle;                        // off+3880
    FontHandle                                         mSmoothFontLatinHandle;                    // off+3960
    FontHandle                                         mUIFontHandle;                             // off+4040
    std::unique_ptr<__int64>                           mUnk4120;                                  // off+4120
    ServiceRegistrationToken<void>                     mUnk4128;                                  // off+4128
    std::unique_ptr<Social::MultiplayerServiceManager> mMultiplayerServiceManager;                // off+4136
    ServiceRegistrationToken<Social::MultiplayerServiceManager>
        mMultiplayerServiceManagerServiceRegistrationToken; // off+4144

    std::unique_ptr<EDUSystems>                                 mEDUSystems;               // off+4152
    std::unique_ptr<ServerInstance>                             mServerInstance;           // off+4160
    std::array<SerialWorkList, 3>                               mSerialWorkList;           // off+4168
    std::unique_ptr<TaskGroup>                                  mStorageAreaTaskGroup;     // off+4624
    std::unique_ptr<TaskGroup>                                  mInitializationTaskGroup;  // off+4632
    std::unique_ptr<TaskGroup>                                  mServerInitTaskGroup;      // off+4640
    std::unique_ptr<TaskGroup>                                  mJoinMultiplayerTaskGroup; // off+4648
    std::shared_ptr<DeferredTasksManager<DeferredTaskCategory>> mDeferredTasks;            // off+4656

    std::unique_ptr<Bedrock::Threading::PendingConditionals> mPendingConditionals; // off+4672
    ServiceRegistrationToken<Bedrock::Threading::PendingConditionals>
        mPendingConditionalsServiceRegistrationToken; // off+4680

    std::unique_ptr<__int64 /* 120 */>             mUnk4688;                               // off+4688
    ServiceRegistrationToken<void>                 mUnk4696;                               // off+4696
    std::unique_ptr<__int64 /* 48 */>              mUnk4704;                               // off+4704
    std::unique_ptr<ChunkPerformanceData>          mChunkPerformanceData;                  // off+4712
    ServiceRegistrationToken<ChunkPerformanceData> mChunkPerformanceDataRegistrationToken; // off+4720
    std::unique_ptr<__int64 /* 392 */>             mUnk4728;                               // off+4728
    ServiceRegistrationToken<void>                 mUnk4736;                               // off+4736
    std::unique_ptr<__int64 /* 240 */>             mUnk4744;                               // off+4722
    ServiceRegistrationToken<void>                 mUnk4752;                               // off+4752
    std::unique_ptr<__int64 /* 560 */>             mUnk4760;                               // off+4760
    ServiceRegistrationToken<void>                 mUnk4768;                               // off+4768
    std::unique_ptr<__int64 /* 8984 */>            mUnk4776;                               // off+4776
    ServiceRegistrationToken<void>                 mUnk4784;                               // off+4784
    std::unique_ptr<__int64 /* 976 */>             mUnk4792;                               // off+4792
    ServiceRegistrationToken<void>                 mUnk4800;                               // off+4800
    std::unique_ptr<__int64 /* 56 */>              mUnk4808;                               // off+4808
    ServiceRegistrationToken<void>                 mUnk4816;                               // off+4816
    std::unique_ptr<__int64 /* 32 */>              mUnk4824;                               // off+4824
    ServiceRegistrationToken<void>                 mUnk4832;                               // off+4832
    std::unique_ptr<__int64 /* ??? */>             mUnk4840;                               // off+4840
    ServiceRegistrationToken<void>                 mUnk4848;                               // off+4848
    std::unique_ptr<__int64 /* ??? */>             mUnk4856;                               // off+4856
    ServiceRegistrationToken<void>                 mUnk4864;                               // off+4864
    std::unique_ptr<__int64 /* ??? */>             mUnk4872;                               // off+4872
    ServiceRegistrationToken<void>                 mUnk4880;                               // off+4880
    std::unique_ptr<__int64 /* 144 */>             mUnk4888;                               // off+4888
    ServiceRegistrationToken<void>                 mUnk4896;                               // off+4896
    std::unique_ptr<__int64 /* 160 */>             mUnk4904;                               // off+4904
    ServiceRegistrationToken<void>                 mUnk4912;                               // off+4912

    struct Unk4920 : public Bedrock::EnableNonOwnerReferences {
        __int64 mUnk24[2]; // off+24
        bool    mUnk40;    // off+40
        int     mUnk44;    // off+44
    };

    Unk4920                                            mUnk4920;                                // off+4920
    ServiceRegistrationToken<Unk4920>                  mUnk4968;                                // off+4968
    std::map<__int64, Bedrock::PubSub::Subscription>   mUnk4976;                                // off+4976
    std::map<__int64, Bedrock::PubSub::Subscription>   mUnk4992;                                // off+4992
    std::vector<Bedrock::PubSub::Subscription>         mPrimaryUserOptionLockSubscriptions;     // off+5008
    std::vector<Bedrock::PubSub::Subscription>         mPrimaryUserOptionObserverSubscriptions; // off+5032
    Bedrock::PubSub::Subscription                      mUnk5056;                                // off+5056
    std::vector<Bedrock::PubSub::Subscription>         mUnk5072;                                // off+5072
    std::vector<Bedrock::PubSub::Subscription>         mUnk5096;                                // off+5096
    Bedrock::PubSub::Subscription                      mUnk5120;                                // off+5120
    std::unique_ptr<__int64>                           mUnk5136;                                // off+5136
    std::unique_ptr<GatheringManager>                  mGatheringManager;                       // off+5144
    std::unique_ptr<__int64>                           mUnk5152;                                // off+5152
    std::unique_ptr<__int64>                           mUnk5160;                                // off+5160
    std::unique_ptr<std::string>                       mExternalFilePath;                       // off+5168
    std::unique_ptr<__int64 /*232*/>                   mUnk5176;                                // off+5176, 14180EAB0
    ServiceRegistrationToken<void>                     mUnk5184;                                // off+5184
    std::unique_ptr<__int64>                           mUnk5192;                                // off+5192
    ServiceRegistrationToken<void>                     mUnk5200;                                // off+5200
    Bedrock::Threading::IAsyncResult<void>::Handle     mLoadEntitlementCacheHandle;             // off+5208
    std::unique_ptr<WatchdogTimer>                     mWatchdogTimer;                          // off+5224
    std::unique_ptr<__int64 /* 56 */>                  mUnk5232;                                // off+5232
    std::unique_ptr<SceneStack>                        mSceneStack;                             // off+5240
    ServiceRegistrationToken<SceneStack>               mSceneStackServiceRegistrationToken;     // off+5248
    std::unique_ptr<__int64>                           mUnk5256;                                // off+5256
    std::unique_ptr<OreUI::Router>                     mUIRouter;                               // off+5264
    std::unique_ptr<OreUI::DataProviderManager>        mDataProviderManager;                    // off+5272
    std::unique_ptr<Realms::RealmsServices>            mRealmsServices;                         // off+5280
    std::unique_ptr<Realms::RealmsSystem>              mRealmsSystem;                           // off+5288
    std::unique_ptr<CachedScenes>                      mCachedScenes;                           // off+5296
    std::function<void()>                              mRouteConfigurationChangeCallback;       // off+5304
    Bedrock::PubSub::Subscription                      mUnk5368;                                // off+5368
    Bedrock::NotNullNonOwnerPtr<cereal::ReflectionCtx> mReflectionCtx;                          // off+5384
    bool                                               mUnk5408;                                // off+5408
    double                                             mGameUpdateDurationInSeconds;            // off+5416

    SDK_API MinecraftGame();
#pragma SPHR_LINKER_SYM_ALIAS("??0MinecraftGame@@QEAA@XZ", "?ctor@MinecraftGame@@QEAAPEAV1@XZ")

    SDK_API MinecraftGame *ctor();

    // vtb+0
    virtual ~MinecraftGame() override;

    // vtb+1
    SDK_API virtual void initialize(int argc, char **argv) override;

    // vtb+2
    SDK_API virtual void update() override;

    // vtb+1 (for IGameServerShutdown)
    SDK_API virtual void requestLeaveGame(bool switchScreen, bool sync) override;

    SDK_API void startFrame();

    SDK_API void endFrame();

    SDK_API void updateGraphics(IClientInstance &client, const Timer &timer);

    SDK_API void updateAudio(IClientInstance &client, const Timer &timer);

    SDK_API void _update();

    SDK_API bool _clientUpdate();

    SDK_API void _InitComplete();

    SDK_API SerialWorkList::WorkResult _initFinish(std::shared_ptr<InitContext> &initContext);
};