#pragma once

#include <optional>
#include "SDK/api/src-client/common/client/game/IClientInstance.h"
#include "SDK/api/src/common/world/phys/AABB.h"

class IMinecraftGame;
class ScreenContext;
class BlockActorRenderDispatcher;
class ActorRenderDispatcher;
class ActorBlockRenderer;
class ItemInHandRenderer;
class ItemRenderer;
class ParticleSystemEngine;

class SortedMeshDrawList {
    // ...
};

// size: 488
struct HistoricalFrameTimes {
    uint64_t mFrameIndex;                      // off+0
    float    mHistoricalMinimumFrameTimes[30]; // off+8
    float    mHistoricalMaximumFrameTimes[30]; // off+128
    float    mHistoricalAverageFrameTimes[30]; // off+248
    float    mLastFrameTime[30];               // off+368
};

#if MC_VERSION == v1_21_2
// size: 672
class BaseActorRenderContext {
public:
    float mNumEntitiesRenderedThisFrame; // off+8
    float mLastFrameTime;                // off+12

    SortedMeshDrawList                    *mSortedMeshDrawList;          // off+16
    IClientInstance                       &mClientInstance;              // off+24
    IMinecraftGame                        &mMinecraftGame;               // off+32
    ScreenContext                         &mScreenContext;               // off+40
    BlockActorRenderDispatcher            &mBlockEntityRenderDispatcher; // off+48
    std::shared_ptr<ActorRenderDispatcher> mEntityRenderDispatcher;      // off+56
    ActorBlockRenderer                    &mEntityBlockRenderer;         // off+72
    ItemInHandRenderer                    &mItemInHandRenderer;          // off+80
    ItemRenderer                          &mItemRenderer;                // off+88
    ParticleSystemEngine                  *mParticleSystemEngine;        // off+96
    std::optional<int64_t>                 mRenderUniqueIdOverride;      // off+104

    Vec3 mCameraTargetPosition; // off+120
    Vec3 mCameraPosition;       // off+132
    AABB mWorldClipRegion;      // off+144

    float mFrameAlpha;            // off+168
    bool  mIsOnScreen;            // off+172
    bool  mUpdateBonesAndEffects; // off+173
    bool  mUpdateEffects;         // off+174
    bool  mIgnoresLightning;      // off+175
    bool  mUnk176;                // off+176
    bool  munk177;                // off+177

    HistoricalFrameTimes mHistoricalFrameTimes; // off+184

    virtual ~BaseActorRenderContext() {}
};
static_assert(sizeof(BaseActorRenderContext) == 672);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
// size: 712
class BaseActorRenderContext {
public:
    float mNumEntitiesRenderedThisFrame; // off+8
    float mLastFrameTime;                // off+12

    SortedMeshDrawList                    *mSortedMeshDrawList;          // off+16
    IClientInstance                       &mClientInstance;              // off+24
    IMinecraftGame                        &mMinecraftGame;               // off+32
    ScreenContext                         &mScreenContext;               // off+40
    BlockActorRenderDispatcher            &mBlockEntityRenderDispatcher; // off+48
    std::shared_ptr<ActorRenderDispatcher> mEntityRenderDispatcher;      // off+56
    ActorBlockRenderer                    &mEntityBlockRenderer;         // off+72
    ItemInHandRenderer                    &mItemInHandRenderer;          // off+80
    ItemRenderer                          &mItemRenderer;                // off+88
    ParticleSystemEngine                  *mParticleSystemEngine;        // off+96
    std::optional<int64_t>                 mRenderUniqueIdOverride;      // off+104
    uint64_t                               mUnk120[5];                   // off+120

    Vec3 mCameraTargetPosition; // off+160
    Vec3 mCameraPosition;       // off+172
    AABB mWorldClipRegion;      // off+184

    float mFrameAlpha;            // off+208
    bool  mIsOnScreen;            // off+212
    bool  mUpdateBonesAndEffects; // off+213
    bool  mUpdateEffects;         // off+214
    bool  mIgnoresLightning;      // off+215
    bool  mUnk216;                // off+216
    bool  munk217;                // off+217

    HistoricalFrameTimes mHistoricalFrameTimes; // off+224

    virtual ~BaseActorRenderContext() {}
};
static_assert(sizeof(BaseActorRenderContext) == 712);
#endif