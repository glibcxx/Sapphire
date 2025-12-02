#pragma once

#include "SDK/api/src-client/common/client/gui/screens/ScreenContext.h"
#include "SDK/api/src-client/common/client/renderer/debug/DebugRenderer.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src-deps/MinecraftRenderer/renderer/TexturePtr.h"
#include "SDK/api/src/common/world/level/LevelListener.h"
#include "SDK/api/src/common/world/level/Tick.h"
#include "SDK/api/src/common/world/level/dimension/DimensionType.h"
#include "SDK/api/src/common/entity/WeakEntityRef.h"
#include "SDK/api/src/common/AppPlatformListener.h"

class LevelRendererPlayer;
class IClientInstance;
class HolographicPlatform;
class BlockActorRenderDispatcher;
class ParticleSystemEngine;
class MultiPlayerLevel;
class Options;
class WeakEntityRef;
class GameRenderer;
class SoundMapping;
class TextureAtlas;
class SoundPlayerInterface;
class GeometryGroup;
class ActorResourceDefinitionGroup;
class RenderChunkCoordinator;
class BlockTessellator;
class ParticleEngine;
class TaskGroup;
class Level;
class LevelRendererProxy;
class PlayerRenderView;
class LocalPlayer;
class LevelRendererShadowCamera;
class LevelRenderPreRenderUpdateParameters;
namespace mce {
    class TextureGroup;
    class Mesh;
} // namespace mce

// size: 2416 (1.21.2/1.21.60), 2432 (1.21.50)
class LevelRenderer : public LevelListener, public AppPlatformListener {
public:
    ; // off+(1.21.50)
    std::unordered_map<DimensionType, std::unique_ptr<RenderChunkCoordinator>>
                                                   mRenderChunkCoordinators;     // off+24
    std::shared_ptr<mce::TextureGroup>             mTextures;                    // off+88
    std::shared_ptr<mce::Mesh>                     mShadowCylinder;              // off+104
    std::shared_ptr<mce::Mesh>                     mShadowOverlayCube;           // off+120
    std::shared_ptr<mce::Mesh>                     mShadowDisc;                  // off+136
    std::shared_ptr<mce::Mesh>                     mSkyMesh;                     // off+152
    std::shared_ptr<mce::Mesh>                     mStarsMesh;                   // off+168
    std::shared_ptr<mce::Mesh>                     mEndSkyMesh;                  // off+184
    std::shared_ptr<mce::Mesh>                     mSunMesh;                     // off+200
    std::shared_ptr<mce::Mesh>                     mMoonMesh[8];                 // off+216
    std::shared_ptr<mce::Mesh>                     mUnk344;                      // off+344
    mce::TexturePtr                                mAtlasTexture;                // off+360
    mce::TexturePtr                                mBrightnessTex;               // off+392
    mce::TexturePtr                                mSunTex;                      // off+424
    mce::TexturePtr                                mMoonTex;                     // off+456
    mce::TexturePtr                                mEndSkyTex;                   // off+488
    std::vector<mce::TexturePtr>                   mCrackFrames;                 // off+520
    DebugRenderer                                  mDebugRenderer;               // off+544
    Tick                                           mTicks;                       // off+552
    BlockActorRenderDispatcher                    &mBlockEntityRenderDispatcher; // off+560
    std::unique_ptr<BlockTessellator>              mLocalRenderer;               // off+568
    Bedrock::NonOwnerPointer<ParticleEngine>       mParticleEngine;              // off+576
    Bedrock::NonOwnerPointer<ParticleSystemEngine> mParticleSystemEngine;        // off+600
    std::shared_ptr<void>                          mUnk624;                      // off+624
    std::unique_ptr<TaskGroup>                     mTaskGroup;                   // off+640
    std::unique_ptr<TaskGroup>                     mSyncTaskGroup;               // off+648
    GameRenderer                                  &mGameRenderer;                // off+656
    Level                                         &mLevel;                       // off+664
    std::shared_ptr<Options>                       mOptions;                     // off+672
    IClientInstance                               &mClientInstance;              // off+688
    WeakEntityRef                                  mLocalPlayer;                 // off+696
    HolographicPlatform                           &mHolographicPlatform;         // off+720
    std::function<void()>                          mUnk728;                      // off+728
    std::shared_ptr<LevelRendererPlayer>           mLevelRendererPlayer;         // off+792
    std::shared_ptr<LevelRendererShadowCamera>     mUnk808[2];                   // off+808
    std::shared_ptr<LevelRendererShadowCamera>     mUnk840;                      // off+840
#if MC_VERSION == v1_21_2 || MC_VERSION == v1_21_50
    std::shared_ptr<LevelRendererShadowCamera> mUnk856[2]; // off+856
#endif
    __int64                                          mUnk888;           // off+888
    std::aligned_storage_t<1496, 8> /*LevelBuilder*/ mLevelBuilder;     // off+896
    std::unique_ptr<LevelRendererProxy>              mProxy;            // off+2392
    mce::Color                                       mClearBufferColor; // off+2400
    std::weak_ptr<PlayerRenderView>                  mPlayerView;       // off+2416

    SDK_API LevelRenderer(
        IClientInstance                                                 &clientInstance,
        MultiPlayerLevel                                                &level,
        std::shared_ptr<Options>                                         options,
        std::shared_ptr<mce::TextureGroup>                               textures,
        const Bedrock::NotNullNonOwnerPtr<const TextureAtlas>           &terrainTexture,
        WeakEntityRef                                                    localPlayer,
        HolographicPlatform                                             &holographicPlatform,
        GameRenderer                                                    &gameRenderer,
        const Bedrock::NotNullNonOwnerPtr<SoundPlayerInterface>         &soundPlayer,
        const Bedrock::NotNullNonOwnerPtr<GeometryGroup>                &geometryGroup,
        const SoundMapping                                              &sounds,
        const Bedrock::NotNullNonOwnerPtr<ActorResourceDefinitionGroup> &entityResourceDefGroup
    );
#pragma SPHR_LINKER_SYM_ALIAS(                                                                                                                                                                                                                                                                                                                                                                                                                                                                                            \
    "??0LevelRenderer@@QEAA@AEAVIClientInstance@@AEAVMultiPlayerLevel@@V?$shared_ptr@VOptions@@@std@@V?$shared_ptr@VTextureGroup@mce@@@4@AEBV?$not_null@V?$NonOwnerPointer@$$CBVTextureAtlas@@@Bedrock@@@gsl@@VWeakEntityRef@@AEAVHolographicPlatform@@AEAVGameRenderer@@AEBV?$not_null@V?$NonOwnerPointer@VSoundPlayerInterface@@@Bedrock@@@7@AEBV?$not_null@V?$NonOwnerPointer@VGeometryGroup@@@Bedrock@@@7@AEBVSoundMapping@@AEBV?$not_null@V?$NonOwnerPointer@VActorResourceDefinitionGroup@@@Bedrock@@@7@@Z",        \
    "?ctor@LevelRenderer@@QEAAPEAV1@AEAVIClientInstance@@AEAVMultiPlayerLevel@@V?$shared_ptr@VOptions@@@std@@V?$shared_ptr@VTextureGroup@mce@@@5@AEBV?$not_null@V?$NonOwnerPointer@$$CBVTextureAtlas@@@Bedrock@@@gsl@@VWeakEntityRef@@AEAVHolographicPlatform@@AEAVGameRenderer@@AEBV?$not_null@V?$NonOwnerPointer@VSoundPlayerInterface@@@Bedrock@@@8@AEBV?$not_null@V?$NonOwnerPointer@VGeometryGroup@@@Bedrock@@@8@AEBVSoundMapping@@AEBV?$not_null@V?$NonOwnerPointer@VActorResourceDefinitionGroup@@@Bedrock@@@8@@Z" \
)

    SPHR_DECL_API("1.21.2", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xC1\x48\x89\x45\x00\x4D\x8B\xF8")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xC1\x48\x89\x45")
    SDK_API LevelRenderer *ctor(
        IClientInstance                                                 &clientInstance,
        MultiPlayerLevel                                                &level,
        std::shared_ptr<Options>                                         options,
        std::shared_ptr<mce::TextureGroup>                               textures,
        const Bedrock::NotNullNonOwnerPtr<const TextureAtlas>           &terrainTexture,
        WeakEntityRef                                                    localPlayer,
        HolographicPlatform                                             &holographicPlatform,
        GameRenderer                                                    &gameRenderer,
        const Bedrock::NotNullNonOwnerPtr<SoundPlayerInterface>         &soundPlayer,
        const Bedrock::NotNullNonOwnerPtr<GeometryGroup>                &geometryGroup,
        const SoundMapping                                              &sounds,
        const Bedrock::NotNullNonOwnerPtr<ActorResourceDefinitionGroup> &entityResourceDefGroup
    );

    // vtb+0
    SDK_API virtual ~LevelRenderer() noexcept override;
#pragma SPHR_LINKER_SYM_ALIAS("??1LevelRenderer@@UEAA@XZ", "?dtor@LevelRenderer@@QEAAXXZ")

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\xD9\x48\x8D\x05")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xF1\x48\x8D\x05")
    SDK_API void dtor() noexcept;

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x45\x32\xE4\x48\x8B\x07")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xC6\x44\x24\x00\x00\x48\x8B\x86\x00\x00\x00\x00\x48\x85\xC0")
    SDK_API void renderLevel(ScreenContext &screenCtx, const FrameRenderObject &frameRenderObj);

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x4D\x00\x48\x8B\xCE")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xF3\x41\x0F\x10\x4C\x24\x00\x48\x8B\xCE")
    SDK_API void preRenderUpdate(ScreenContext &screenCtx, LevelRenderPreRenderUpdateParameters &params);

    LevelRendererPlayer &getLevelRendererPlayer() const { return *this->mLevelRendererPlayer; }
};