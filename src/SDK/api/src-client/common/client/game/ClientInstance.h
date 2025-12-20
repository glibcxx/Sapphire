#pragma once

#include "IClientInstance.h"
#include "SDK/api/src/common/world/Minecraft.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src/common/entity/WeakEntityRef.h"
#include "SDK/api/src/common/world/actor/bhave/BehaviorStatus.h"
#include "SDK/api/src/common/world/actor/player/PlayerListener.h"
#include "SDK/api/src-client/common/client/gui/oreui/OreUITelemetry.h"
#include "SDK/api/src-client/common/client/gui/screens/models/PlayScreenModel.h"
#include "SDK/api/src-client/common/client/util/ClipboardProxy.h"
#include "SDK/api/src-deps/Renderer/Color.h"
#include "SDK/api/src-deps/Renderer/Camera.h"
#include "SDK/api/src-deps/Renderer/ViewportInfo.h"
#include "SDK/api/src-deps/Core/File/StorageAreaStateListener.h"
#include "SDK/api/src-deps/Core/Utility/pubsub/Subscription.h"
#include "SDK/api/src-deps/MinecraftRenderer/renderer/TexturePtr.h"
#include "common/Memory.hpp"

class ToastManager;
class Certificate;
class LegacyClientNetworkHandler;
class LevelListener;
class IAdvancedGraphicsOptions;
class IMinecraftGame;
class ClientInstanceEventCoordinator;
class ClientNetworkEventCoordinator;
class HitDetectEventListener;
class ClientScriptEventCoordinator;
class KeyboardManager;
class GuiData;
class ClientHitDetectCoordinator;
class ScreenLoadTimeTracker;
class FogDefinitionRegistry;
class FogManager;
class UIEventCoordinator;
class LatencyGraphDisplay;
class GameModuleClient;
class ClientRequirementVerifier;
class BlockTypeRegistry;
class DisconnectionRequestHandler;
class UserAuthentication;
class SceneFactory;
class CachedScenes;
class HitDetectSystem;
class BuildActionIntention;
class LevelRenderer;
class LightTexture;
class LoopbackPacketSender;
class VoiceSystem;
class ClientMoveInputHandler;
class ClientInputHandler;
class SceneStack;
class TextToSpeechClient;
class TTSEventManager;
class TaskGroup;
class BlockTessellator;
class BlockActorRenderDispatcher;
class ActorRenderDispatcher;
class ActorBlockRenderer;
class ItemInHandRenderer;
class ItemRenderer;
class GuidedFlowManager;
class MobEffectsLayout;
class SkinRepositoryClientInterface;
class IGameConnectionListener;
class ContentCatalogService;
class ClientScriptManager;
class PlayerReportHandler;
class NetworkSession;
class GameMode;
class HitResult;
class EducationOptions;
class Level;
struct RenderCameraComponent;
// clang-format off
namespace mce { class Texture; }
namespace persona { class PersonaPieceCollectionModel; }
namespace Editor { class IEditorManager; }
namespace OreUI {
    class SceneProvider;
    class Router;
    class UIBlockThumbnailAtlasManager;
} // namespace OreUI
namespace ApplicationSignal {
    class ClipboardCopy;
    class ClipboardPasteRequest;
} // namespace ApplicationSignal
namespace ui { class ScreenTechStackSelector; }
namespace Social { class User; }
namespace PlayerCapabilities { struct IClientController; }
// clang-format on

enum class ClientPlayMode : int {
    Normal = 0,
    Teaser = 1,
    Screen = 2,
    Viewer = 3,
    Reality = 4,
    Placement = 5,
    LivingRoom = 6,
    ExitLevel = 7,
    ExitLevelLivingRoom = 8,
    NumModes = 9,
};

// https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/client/game/ClientInstance.h#L183

// size: 3208 (1.21.2), 3328 (1.21.50), 3368 (1.21.60)
class ClientInstance : public /* off+0   */ IClientInstance,
                       public /* off+24  */ Core::StorageAreaStateListener,
                       public /* off+128 */ GameCallbacks,
                       public /* off+136 */ PlayerListener,
                       public /* off+144 */ std::enable_shared_from_this<ClientInstance> {
public:
    static_assert(sizeof(IClientInstance) == 24);
    static_assert(sizeof(Core::StorageAreaStateListener) == 128 - 24);
    static_assert(sizeof(PlayerListener) == 136 - 128);

    // size: 40
    struct ClientRenderResources {
        mce::Texture   *mUITexture;       // off+0
        mce::TexturePtr mUICursorTexture; // off+8
    };

    // size: 224
    struct ClientDestroyBlockState {
        using Callback = std::function<void()>;

        Player          &mPlayer;             // off+0
        GameMode        &mGameMode;           // off+8
        bool             mDestroyButtonDown;  // off+16
        const HitResult &mHit;                // off+24
        Callback         mClearInProgressBAI; // off+32
        Callback         mStopDestroying;     // off+96
        Callback         mVibrate;            // off+160
    };

    // size: 32
    struct Unk1248 {
        std::set<uint32_t> mActiveControlOptions;            // off+0
        bool               mOtherConfigsExistInThisCategory; // off+16
        uint32_t           mNumberOfActiveConfigs;           // off+20
        uint32_t           mNumberOfEnabledConfigs;          // off+24
        WYSIWYGState       mWYSIWYGState;                    // off+28
    };

    ;                                                                                // off+(1.21.50)
    ClientInstanceState                                   mClientState;              // off+160
    IMinecraftApp                                        &mApp;                      // off+168
    LevelListener                                        &mLevelListener;            // off+176
    Bedrock::NotNullNonOwnerPtr<IAdvancedGraphicsOptions> mAdvancedGraphicsOptions;  // off+184
    IMinecraftGame                                       &mMinecraftGame;            // off+208
    std::unique_ptr<Minecraft>                            mMinecraft;                // off+216
    bool                                                  mIsFullVanillaPackOnStack; // off+224
    std::unique_ptr<LevelRenderer>                        mLevelRenderer;            // off+232
    std::unique_ptr<LightTexture>                         mLightTexture;             // off+240
    std::unique_ptr<LoopbackPacketSender>                 mPacketSender;             // off+248
    std::unique_ptr<HolographicPlatform>                  mHoloInput;                // off+256
    std::unique_ptr<VoiceSystem>                          mVoiceSystem;              // off+264
    std::unique_ptr<ClientMoveInputHandler>               mClientMoveInputHandler;   // off+272
    std::unique_ptr<ClientInputHandler>                   mClientInputHandler;       // off+280
    std::unique_ptr<KeyboardManager>                      mKeyboardManager;          // off+288
    std::unique_ptr<HitDetectSystem>                      mHitDetectSystem;          // off+296
    std::shared_ptr<UserAuthentication>                   mUserAuthentication;       // off+304
    std::unique_ptr<SceneFactory>                         mSceneFactory;             // off+320
    std::unique_ptr<CachedScenes>                         mCachesScenes;             // off+328
    WeakEntityRef                                         mCameraEntity;             // off+336
    WeakEntityRef                                         mCameraTargetEntity;       // off+360
    WeakEntityRef                                         mLocalPlayer;              // off+384
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    __int64 mUnk408[6]; // off+408
#endif
    std::unique_ptr<BuildActionIntention>       mInProgressBai;                  // off+456
    float                                       mHoloviewerScale;                // off+464
    float                                       mRealityModeFrameFactor;         // off+468
    bool                                        mRealityModeToggleTriggered;     // off+472
    ClientPlayMode                              mPlayMode;                       // off+476
    bool                                        mTickedLastFrame;                // off+480
    std::atomic<bool>                           mOpenControllerDisconnectScreen; // off+481
    std::atomic<bool>                           mHandlingControllerDisconnect;   // off+482
    std::atomic<bool>                           mConnectGamepadScreenActive;     // off+483
    PlayScreenDefaultTab                        mDefaultPlayscreenTab;           // off+484
    int                                         mUnk488;                         // off+488
    char                                        mUnk492;                         // off+492
    bool                                        mIsInUpdate;                     // off+493
    bool                                        mLivingRoomCredits;              // off+494
    std::function<void()>                       mCreditsCallback;                // off+496
    std::chrono::steady_clock::time_point       mNoBlockBreakUntil;              // off+560
    bool                                        mNewDictationString;             // off+568
    std::string                                 mDictation;                      // off+576
    mce::ViewportInfo                           mViewportInfo;                   // off+608
    ClientRenderResources                       mClientRenderResources;          // off+632
    mce::Texture                               *mLevelTexture;                   // off+672
    mce::Camera                                 mCamera;                         // off+680
    ShaderColor                                 mShaderColor;                    // off+1192
    ShaderColor                                 mDarkShaderColor;                // off+1212
    Vec3                                        mLastPointerLocation;            // off+1232
    Unk1248                                     mControlOptions;                 // off+1248
    std::map<int, Vec2>                         mTouchPointerLocations;          // off+1280
    int                                         mActionPointerId;                // off+1296
    std::unique_ptr<SceneStack>                 mSceneStack;                     // off+1304
    std::unique_ptr<OreUI::Router>              mUIRouter;                       // off+1312
    std::unique_ptr<ContentCatalogService>      mServerService;                  // off+1320
    std::shared_ptr<TextToSpeechClient>         mTextToSpeechClient;             // off+1328
    std::shared_ptr<TTSEventManager>            mTTSEventManager;                // off+1344
    std::unique_ptr<TaskGroup>                  mTaskGroup;                      // off+1360
    std::unique_ptr<BlockTessellator>           mBlockTessellator;               // off+1368
    std::unique_ptr<BlockActorRenderDispatcher> mBlockEntityRenderDispatcher;    // off+1376
    std::shared_ptr<ActorRenderDispatcher>      mEntityRenderDispatcher;         // off+1384
    std::unique_ptr<ActorBlockRenderer>         mEntityBlockRenderer;            // off+1400
    std::unique_ptr<ItemInHandRenderer>         mItemInHandRenderer;             // off+1408
    std::unique_ptr<ItemRenderer>               mItemRenderer;                   // off+1416
    std::unique_ptr<GuiData>                    mGuiData;                        // off+1424
    std::unique_ptr<GuidedFlowManager>          mGuidedFlowManager;              // off+1432
    std::unique_ptr<MobEffectsLayout>           mMobEffectsLayout;               // off+1440
    std::shared_ptr<ToastManager>               mToastManager;                   // off+1448
    const SubClientId                           mClientSubId;                    // off+1464
    std::unique_ptr<ClipboardProxy<ApplicationSignal::ClipboardCopy, ApplicationSignal::ClipboardPasteRequest>>
                                                          mClipboardManager;              // off+1472
    std::unique_ptr<SkinRepositoryClientInterface>        mSkinRepositoryClientInterface; // off+1480
    std::shared_ptr<persona::PersonaPieceCollectionModel> mPersonaPieceCollectionModel;   // off+1488
    ClientHMDState                                        mHMDState;                      // off+1504
    std::unique_ptr<FogDefinitionRegistry>                mFogDefinitionRegistry;         // off+2144
    std::unique_ptr<FogManager>                           mFogManager;                    // off+2152
    bool                                                  mIsSceneStackChanging;          // off+2160
    std::unique_ptr<ScreenLoadTimeTracker>                mScreenLoadTimeTracker;         // off+2168
    float                                                 mRemoteServerTimeMs;            // off+2176
    float                                                 mRemoteServerNetworkTimeMs;     // off+2180
    std::shared_ptr<PlayerReportHandler>                  mPlayerReportHandler;           // off+2184
    bool                                                  mHasSwitchedScreen;             // off+2200
    bool                                                  mShouldLeaveGame;               // off+2201
    bool                                                  mPreparingToLeaveGame;          // off+2202
    bool                                                  mIsLeavingGame;                 // off+2203
    bool                                                  mSyncLeaveGame;                 // off+2204
    bool                                                  mDestroyingGame;                // off+2205
    bool                                                  mShuttingDown;                  // off+2206
    std::optional<std::aligned_storage_t<424 - 8, 8> /*Social::GameConnectionInfo*/>
                                                                mGameConnectionInfo;            // off+2208
    char                                                        mUnk2632;                       // off+2632
    char                                                        mUnk2633;                       // off+2633
    char                                                        mUnk2634;                       // off+2634
    int                                                         mUnk2636;                       // off+2636
    char                                                        mUnk2640;                       // off+2640
    std::chrono::steady_clock::time_point                       mServerConnectionTime;          // off+2648
    uint32_t                                                    mServerPingTime;                // off+2656
    std::deque<std::string>                                     mSentMessageHistory;            // off+2664
    std::deque<std::string>                                     mDevConsoleMessageHistory;      // off+2704
    std::shared_ptr<IGameConnectionListener>                    mGameConnectionListener;        // off+2744
    std::function<void(const std::string &, BehaviorStatus)>    mBehaviorCommandStatusCallback; // off+2760
    std::unique_ptr<UIEventCoordinator>                         mUIEventCoordinator;            // off+2824
    std::unique_ptr<ClientHitDetectCoordinator>                 mHitEventCoordinator;           // off+2832
    std::unique_ptr<ClientNetworkEventCoordinator>              mClientNetworkEventCoordinator; // off+2840
    std::unique_ptr<HitDetectEventListener>                     mHitDetectEventListener;        // off+2848
    std::unique_ptr<ClientScriptEventCoordinator>               mClientScriptEventCoordinator;  // off+2856
    std::unique_ptr<EducationOptions>                           mEducationOptions;              // off+2864
    std::shared_ptr<Core::FileStorageArea>                      mActiveFileStorageArea;         // off+2872
    glm::tvec2<float>                                           mPrevCursorPos;                 // off+2888
    glm::tvec2<float>                                           mCurrentCursorPos;              // off+2896
    float                                                       mCursorPosAlpha;                // off+2904
    LatencyGraphDisplay                                        *mLatencyGraphDisplay;           // off+2912
    std::unique_ptr<GameModuleClient>                           mGameModule;                    // off+2920
    Bedrock::NotNullNonOwnerPtr<ClientInstanceEventCoordinator> mEventCoordinator;              // off+2928
    std::unique_ptr<ClientScriptManager>                        mClientScriptManager;           // off+2952
    Bedrock::PubSub::Subscription                               mUnk2960;                       // off+2960
    std::unique_ptr<PlayerCapabilities::IClientController>      mClientCapabilities;            // off+2976
    std::unique_ptr<OreUI::SceneProvider>                       mSceneProvider;                 // off+2984
    std::unique_ptr<ui::ScreenTechStackSelector>                mScreenTechStackSelector;       // off+2992
    OreUI::Telemetry                                            mOreUITelemetry;                // off+3000
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    std::shared_ptr<OreUI::UIBlockThumbnailAtlasManager> mOreUIBlockThumbnailAtlasManager; // off+3024
#endif
    std::shared_ptr<ClientRequirementVerifier>   mClientRequirementVerifier;   // off+3040
    std::shared_ptr<BlockTypeRegistry>           mBlockRegistry;               // off+3056
    std::unique_ptr<DisconnectionRequestHandler> mDisconnectionRequestHandler; // off+3072
    std::unique_ptr<Editor::IEditorManager>      mEditorManager;               // off+3080
#if MC_VERSION == v1_21_2
    std::optional<std::aligned_storage_t<40, 8>> mUnk3008; // off+3008
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    std::optional<std::aligned_storage_t<48, 8>> mUnk3088; // off+3088
#endif
    __int64                                     mUnk3144;        // off+3144
    Bedrock::NotNullNonOwnerPtr<NetworkSession> mNetworkSession; // off+3152
#if MC_VERSION == v1_21_60
    Bedrock::NotNullNonOwnerPtr<void> mUnk3176; // off+3176
#endif
    bool                          mClientUpdateAndRenderThrottlingEnabled;   // off+3176
    int                           mClientUpdateAndRenderThrottlingThreshold; // off+3180
    float                         mClientUpdateAndRenderThrottlingScalar;    // off+3184
    Bedrock::PubSub::Subscription mUnk3192;                                  // off+3192
    std::mutex                    mUserLock;                                 // off+3208
    std::shared_ptr<Social::User> mUser;                                     // off+3288
#if MC_VERSION == v1_21_60
    std::unique_ptr<std::aligned_storage_t<72, 8>> mUnk3328; // off+3328
#endif
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    Bedrock::NotNullNonOwnerPtr<void> mUnk3304; // off+3304
#endif
#if MC_VERSION == v1_21_60
    __int64 mUnk3360; // off+3360
#endif

    SDK_API ClientInstance(
        IMinecraftGame                                             &mg,
        IMinecraftApp                                              &app,
        LevelListener                                              &levelListener,
        SubClientId                                                 subid,
        Bedrock::NotNullNonOwnerPtr<IAdvancedGraphicsOptions>       graphicsOptions,
        Bedrock::NotNullNonOwnerPtr<ClientInstanceEventCoordinator> coordinator,
        LatencyGraphDisplay                                        *latencyGraphDisplay,
        Bedrock::NotNullNonOwnerPtr<NetworkSession>                 networkSession
#if MC_VERSION >= v1_21_60
        ,
        __int64 a10
#endif
    );
    // TODO: SYMBOL ALIAS

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x49\x8B\xF9\x49\x8B\xD8\x4C\x8B\xE2")
    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4D\x8B\xF1\x4D\x8B\xE0\x4C\x8B\xEA")
    SPHR_DECL_API("1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x49\x8B\xF9\x49\x8B\xD8\x4C\x8B\xFA\x48\x89\x54\x24")
    SDK_API ClientInstance *ctor(
        IMinecraftGame                                             &mg,
        IMinecraftApp                                              &app,
        LevelListener                                              &levelListener,
        SubClientId                                                 subid,
        Bedrock::NotNullNonOwnerPtr<IAdvancedGraphicsOptions>       graphicsOptions,
        Bedrock::NotNullNonOwnerPtr<ClientInstanceEventCoordinator> coordinator,
        LatencyGraphDisplay                                        *latencyGraphDisplay,
        Bedrock::NotNullNonOwnerPtr<NetworkSession>                 networkSession
#if MC_VERSION >= v1_21_60
        ,
        __int64 a10
#endif
    );

    // vtb+2
    SPHR_DECL_API("v1_21_2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\x03")
    SPHR_DECL_API("v1_21_50,v1_21_60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x48\x8B\xF9\xE8\x00\x00\x00\x00\x48\x8B\x07")
    SDK_API virtual void onDestroyMinecraftGame() override;

    SPHR_DECL_API("v1_21_2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xF8\x4C\x8B\xFA\x4C\x8B\xF1\x48\x89\x95")
    SPHR_DECL_API("v1_21_50,v1_21_60", "\x48\x89\x5C\x24\x00\x4C\x89\x44\x24\x00\x48\x89\x54\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xF8")
    SDK_API /*virtual*/ void onClientCreatedLevel(
        std::pair<std::unique_ptr<Level>, OwnerPtr<EntityContext>> level, OwnerPtr<EntityContext> player
    ) /*override*/;

    /*virtual*/ bool getMouseGrabbed() const /*override*/ {
        // \x40\x53\x48\x83\xEC\x00\x48\x8B\x01\x48\x8B\xD9\x48\x8B\x80\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x84\xC0\x74\x00\x48\x8B\x8B\x00\x00\x00\x00\x48\x8B\x01\x48\x8B\x80\x00\x00\x00\x00\xFF\x15
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        return memory::vCall<bool>(this, 167);
#elif MC_VERSION == v1_21_2
        return memory::vCall<bool>(this, 162);
#endif
    }

    /*virtual*/ bool isShowingMenu() const /*override*/ {
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        return memory::vCall<bool>(this, 172); // \x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xD9\x45\x33\xFF\x44\x89\x7C\x24\x00\x44\x88\x7D
#elif MC_VERSION == v1_21_2
        return memory::vCall<bool>(this, 167); // \x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xD9\x33\xFF\x89\x7C\x24\x00\x40\x88\x7D
#endif
    }

    /*virtual*/ void grabMouse() const /*override*/ {
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        memory::vCall(this, 340);
#elif MC_VERSION == v1_21_2
        memory::vCall(this, 332);
#endif
    }

    /*virtual*/ void releaseMouse() /*override*/ {
        // \x40\x53\x48\x83\xEC\x00\x48\x8B\x01\x48\x8B\xD9\x48\x8B\x80\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x84\xC0\x74\x00\x48\x8B\x8B\x00\x00\x00\x00\x48\x8B\x01\x48\x8B\x80\x00\x00\x00\x00\x48\x83\xC4\x00\x5B\x48\xFF\x25\x00\x00\x00\x00\x48\x83\xC4\x00\x5B\xC3\x48\x89\x5C\x24
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        memory::vCall(this, 341);
#elif MC_VERSION == v1_21_2
        memory::vCall(this, 333);
#endif
    }

    SPHR_DECL_API("v1_21_2,v1_21_50,v1_21_60", "call", "\xE8\x00\x00\x00\x00\x48\x85\xC0\x74\x00\xF3\x0F\x10\x58\x00\xF3\x0F\x10\x05")
    SDK_API RenderCameraComponent *getRenderCameraComponent() const;

    SPHR_DECL_API("v1_21_2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x0F\x57\xC0")
    SPHR_DECL_API("v1_21_50", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x56\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\xB4\x24\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x0F\x57\xC0")
    SPHR_DECL_API("v1_21_60", "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x70\x00\x48\x89\x78\x00\x55\x41\x56\x41\x57\x48\x8D\x68\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4C\x8B\xF1")
    SDK_API /*virtual*/ bool getRenderPlayerModel() const /*override*/;

    SPHR_DECL_API("v1_21_2", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x10\x48\x89\x38\x48\x89\x54\x24\x00\x48\x8D\x54\x24\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x90\x48\x8B\x4C\x24")
    SPHR_DECL_API("v1_21_50", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x10\x48\x89\x28\x48\x89\x54\x24\x00\x48\x8D\x54\x24\x00\x48\x8B\xCB")
    SPHR_DECL_API("v1_21_60", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x10\x33\xED")
    SDK_API std::unique_ptr<LegacyClientNetworkHandler> _createNetworkHandler(std::unique_ptr<Certificate> cert);

    SPHR_API static ClientInstance *primaryClientInstance;
};