#pragma once

#include "SDK/api/src-client/common/client/renderer/BaseActorRenderContext.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"
#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-deps/Core/Utility/optional_ref.h"
#include "SDK/api/src-deps/MinecraftRenderer/renderer/TexturePtr.h"
#include "SDK/api/src-deps/MinecraftRenderer/resources/TextureResourceService.h"
#include "SDK/api/src-client/common/client/renderer/game/TerrainMaterialVariationManager.h"
#include "SDK/api/src-deps/Renderer/Camera.h"
#include "SDK/api/src/common/world/actor/player/PlayerListener.h"
#include "SDK/api/src/common/world/level/LevelListener.h"
#include "SDK/api/src/common/world/level/SubChunkPos.h"
#include "SDK/api/src-client/common/client/particlesystem/particle/ParticleRenderData.h"
#include "common/Memory.hpp"

class BaseActorRenderContext;
class ScreenContext;
class IClientInstance;
class RenderChunkInstanced;
class TerrainLayer;
class Level;
class LevelRenderer;
class DrawsByRenderController;
class Dimension;
class LevelRendererCameraProxy;
class WeatherRenderer;
struct ViewRenderObject;
struct LevelRendererCommandListInit;
struct LevelRenderPreRenderUpdateParameters;
struct ChunkRenderObjectCollection;
template <typename>
class GridArea;
namespace mce {
    class Mesh;
}

// size: 24
class SortedMeshDrawList {
public:
    std::vector<DrawsByRenderController> mDrawsByRenderController; // off+0
};

// size: 28
struct FogDistanceSetting {
    enum class DistanceType : int {
        FIXED = 0,
        RENDER = 1,
    };
    mce::Color   mColor; // off+0
    float        mStart; // off+16
    float        mEnd;   // off+20
    DistanceType mType;  // off+24
};

// size: 16
struct FogVolumetricDensitySetting {
    float mMaxDensity;        // off+0
    float mMaxDensityHeight;  // off+4
    float mZeroDensityHeight; // off+8
    bool  mUniform;           // off+12
};

// size: 32
struct FogVolumetricCoefficientSetting {
    mce::Color mScattering; // off+0
    mce::Color mAbsorption; // off+16
};

// size: 3208 (1.21.2), 3232 (1.21.50), 3224 (1.21.60)
class LevelRendererCamera {
public:
    // size: 24
    struct RenderChunkPosBounds {
        SubChunkPos min; // off+0
        SubChunkPos max; // off+12
    };

    size_t mChunkQueueSize;               // off+8
    size_t mTerrainChunkQueueSize[3][18]; // off+16

    std::vector<gsl::not_null<BlockActor *>> mUnk448;                               // off+
    std::vector<gsl::not_null<BlockActor *>> mEntityRenderQueue;                    // off+472
    std::vector<__int32>                     mUnk496;                               // off+
    __int64                                  mUnk520;                               // off+
    SortedMeshDrawList                       mSortedMeshDrawList;                   // off+528
    std::vector<gsl::not_null<BlockActor *>> mBlockEntityRenderQueue;               // off+552
    std::vector<gsl::not_null<BlockActor *>> mBlockEntityRenderAlphaQueue;          // off+576
    std::vector<gsl::not_null<BlockActor *>> mBlockEntityShadowQueue;               // off+600
    mce::MaterialPtr                         shadowVolumeBack;                      // off+624
    mce::MaterialPtr                         shadowVolumeFront;                     // off+640
    mce::MaterialPtr                         shadowOverlayMat;                      // off+656
    mce::MaterialPtr                         starsMaterial;                         // off+672
    mce::MaterialPtr                         skyPlaneMaterial;                      // off+688
    mce::MaterialPtr                         sunMoonMaterial;                       // off+704
    mce::MaterialPtr                         endSkyMaterial;                        // off+720
    mce::MaterialPtr                         cloudMaterial;                         // off+736
    mce::MaterialPtr                         wireframeMaterial;                     // off+752
    mce::MaterialPtr                         mCubemapMaterial;                      // off+768
    std::vector<mce::TexturePtr>             mCubemapTextures;                      // off+784
    mce::ClientTexture                       mCubemapTexture;                       // off+808
    TerrainMaterialVariationManager          mTerrainMaterialVariationManager;      // off+832
    float                                    mDeltaTime;                            // off+976
    float                                    mLastTime;                             // off+980
    float                                    mWaterLevel;                           // off+984
    glm::tvec2<float>                        mFogControl;                           // off+988
    float                                    mFogBrOriginal;                        // off+996
    float                                    mUnk1000;                              // off+1000
    float                                    mUnk1004;                              // off+1004
    float                                    mFogBr;                                // off+1008
    float                                    mBaseFogEnd;                           // off+1012
    float                                    mBaseFogStart;                         // off+1016
    FogDistanceSetting                       mCurrentDistanceFog;                   // off+1020
    FogDistanceSetting                       mLastTargetDistanceFog;                // off+1048
    FogVolumetricDensitySetting              mCurrentFogDensity;                    // off+1076
    FogVolumetricCoefficientSetting          mAirFogCoefficient;                    // off+1092
    FogVolumetricCoefficientSetting          mWaterFogCoefficient;                  // off+1124
    FogVolumetricCoefficientSetting          mCloudFogCoefficient;                  // off+1156
    bool                                     mFogWasUnderwaterLastCheck;            // off+1188
    bool                                     mFogWasUnderLavaLastCheck;             // off+1189
    bool                                     mFogWasUnderPowderSnowLastCheck;       // off+1190
    bool                                     mBlendFogThisFrame;                    // off+1191
    float                                    mBlindnessLevel;                       // off+1192
    ParticleRenderData                       mParticleRenderData;                   // off+1200
    const float                              mUnk1400;                              // off+1400
    bool                                     mUnk1404;                              // off+1404
    const float                              mRenderDistanceCloudFadeOutMultiplier; // off+1408
    float                                    mFakeHDR;                              // off+1412
    float                                    mAverageBrightness;                    // off+1416
    uint32_t                                 mFrameID;                              // off+1420
    int32_t                                  mViewAreaDistance;                     // off+1424
    float                                    mFarChunksDistance;                    // off+1428
    float                                    mRenderDistance;                       // off+1432
    float                                    mCullEndDistance;                      // off+1436
    const int32_t                            mMaxInflightChunks;                    // off+1440
    std::shared_ptr<GridArea<std::shared_ptr<RenderChunkInstanced>>>
                                                 mViewArea;              // off+1448
    BlockPos                                     mLastFaceSortPos;       // off+1464
    Vec3                                         mLastFaceSortDir;       // off+1476
    Vec3                                         mLastDirtySortPos;      // off+1488
    BlockPos                                     mLastNearbyFaceSortPos; // off+1500
    RenderChunkPosBounds                         mLastFaceSortBounds;    // off+1512
    Vec3                                         mLastChunkResortPos;    // off+1536
    Vec3                                         mCameraPos;             // off+1548
    Vec3                                         mCameraTargetPos;       // off+1560
    __int64                                      mUnk1576[25];
    __int32                                      mUnk1776[51];
    __int16                                      mUnk1980;
    mce::Camera                                  mWorldSpaceCamera;      // off+1984
    std::vector<Actor *>                         mWaterHoleEntityQueue;  // off+2496
    std::unique_ptr<WeatherRenderer>             mWeatherRenderer;       // off+2520
    std::unique_ptr<ChunkRenderObjectCollection> mChunkRenderObjects;    // off+2528
    bool                                         mCameraUnderWater;      // off+2536
    bool                                         mCameraUnderLiquid;     // off+2537
    bool                                         mCameraUnderPowderSnow; // off+2538
    bool                                         mCameraUnderLava;       // off+2539
    bool                                         mCameraInRain;          // off+2540
    bool                                         mShowSky;               // off+2541
    bool                                         mUnk2542;               // off+2542
    Level                                       &mLevel;                 // off+2544
    std::shared_ptr<BlockSource>                 mViewRegion;            // off+2552
    Dimension                                   *mDimension;             // off+2568
    int16_t                                      mCloudHeight;           // off+2576
    LevelRenderer                               &mLevelRenderer;         // off+2584
    __int64                                      mUnk2592[11];
    std::vector<BlockActor *>                    mTempBlockEntityList; // off+2680
    std::unique_ptr<LevelRendererCameraProxy>    mProxy;               // off+2704
    __int64                                      mUnk2712[65];

    SDK_API LevelRendererCamera(IClientInstance &clientInstance, Level &level, LevelRenderer &levelRenderer);
#pragma SDK_LINKER_SYM_ALIAS(                                                                    \
    "??0LevelRendererCamera@@QEAA@AEAVIClientInstance@@AEAVLevel@@AEAVLevelRenderer@@@Z",        \
    "?ctor@LevelRendererCamera@@QEAAPEAV1@AEAVIClientInstance@@AEAVLevel@@AEAVLevelRenderer@@@Z" \
)

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x93\x00\x00\x00\x00\x48\x8D\xBB")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8D\x9F\x00\x00\x00\x00\x48\x89\xB7")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8D\xB7\x00\x00\x00\x00\x4C\x89\xBF")
    SDK_API LevelRendererCamera *ctor(IClientInstance &clientInstance, Level &level, LevelRenderer &levelRenderer);

    // vtb+0
    virtual ~LevelRendererCamera();

    /*virtual*/ Vec3 &getCameraPos() {
        /*
            search "rebuildClouds task" and find
            `v7.m128_f32[0] = (float)((float)(v8 + a3) * 0.02) + *(float *)(a1 + {here it is});`
            at the top
        */

#if MC_VERSION == v1_21_2
        return memory::getField<Vec3>(this, 1544);
#elif MC_VERSION == v1_21_50
        return memory::getField<Vec3>(this, 1560);
#elif MC_VERSION == v1_21_60
        return memory::getField<Vec3>(this, 1552);
#endif
    }

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xF3\x0F\x10\x00\x00\x66\x0F\x6E\x8B")
    SDK_API /*virtual*/ void preRenderUpdate(
        ScreenContext                        &screenContext,
        LevelRenderPreRenderUpdateParameters &levelRenderPreRenderUpdateParameters
    );

    SPHR_DECL_API("1.21.2", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xE9\x4C\x89\x4D\x00\x49\x8B\xF0")
    SPHR_DECL_API("1.21.50,1.21.60", "\x40\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x89\x4D\x00\x4D\x8B\xE8\x48\x89\x54\x24")
    SDK_API /*virtual*/ void render(
        BaseActorRenderContext       &baseEntityRenderContext,
        const ViewRenderObject       &renderObj,
        IClientInstance              &ci,
        LevelRendererCommandListInit &levelRendererCommandListInit
    );

    SPHR_DECL_API("1.21.2,1.21.50", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xC8\x48\x85\xC0\x75\x00\x48\xFF\xC3")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xC8\x48\x85\xC0\x75\x00\x48\xFF\xC7")
    SDK_API RenderChunkInstanced *getRenderChunkInstancedAt(const SubChunkPos &rcp) const;

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x49\x8B\xCF\x49\x8D\x47")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x49\x8B\xD7\x49\x8D\x47")
    SDK_API void queueChunk(
        ChunkRenderObjectCollection                        &collection,
        const RenderChunkInstanced                         &renderChunkInstanced,
        float                                               farDistance2,
        float                                               currentTime,
        const TerrainMaterialVariationManager              &terrainVariationMgr,
        optional_ref<const TerrainMaterialVariationManager> fadeVariationMgr
    );

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x57\x48\x83\xEC\x00\x4C\x8B\x94\x24")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x4C\x8B\x9C\x24")
    SDK_API void _addToRenderChunkQueue(
        ChunkRenderObjectCollection           &collection,
        const TerrainMaterialVariationManager &terrainVariationMgr,
        const TerrainLayer                    &layer,
        const RenderChunkInstanced            &renderChunkInstanced,
        size_t                                 chunkIdx,
        const BlockPos                        &chunkPos
    );
};