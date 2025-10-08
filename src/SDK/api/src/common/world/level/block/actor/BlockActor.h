#pragma once

#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src/common/world/level/BlockPos.h"
#include "SDK/api/src/common/world/actor/ActorTerrainInterlockData.h"
#include "BlockActorRendererId.h"

#if MC_VERSION >= v1_21_60
namespace Bedrock::Safety {
    class RedactableString {
        ::std::string                  mUnredactedString;
        ::std::optional<::std::string> mRedactedString;
    };
} // namespace Bedrock::Safety
#endif

#if MC_VERSION == v1_21_60
// size: 232
class BlockActor {
public:
    int                               mTickCount;            // off+8
    const class Block                *mBlock;                // off+16
    float                             mDestroyTimer;         // off+24
    Vec3                              mDestroyDirection;     // off+28
    float                             mDestroyProgress;      // off+40
    BlockPos                          mPosition;             // off+44
    AABB                              mBB;                   // off+56
    const BlockActorType              mType;                 // off+84
    BlockActorRendererId              mRendererId;           // off+88
    Bedrock::Safety::RedactableString mCustomName;           // off+96
    std::string                       mFilteredCustomName;   // off+160
    int                               mRepairCost;           // off+192
    bool                              mClientSideOnly;       // off+196
    bool                              mIsMovable;            // off+167
    bool                              mSaveCustomName;       // off+168
    bool                              mCanRenderCustomName;  // off+169
    const float                       signShadowRadius;      // off+200
    ActorTerrainInterlockData         mTerrainInterlockData; // off+208
    bool                              mChanged;              // off+232

    virtual ~BlockActor() = default;
};
static_assert(offsetof(BlockActor, mChanged) == 232);
#else
// size: 192
class BlockActor {
public:
    int                       mTickCount;            // off+8
    const class Block        *mBlock;                // off+16
    float                     mDestroyTimer;         // off+24
    Vec3                      mDestroyDirection;     // off+28
    float                     mDestroyProgress;      // off+40
    BlockPos                  mPosition;             // off+44
    AABB                      mBB;                   // off+56
    const BlockActorType      mType;                 // off+80
    BlockActorRendererId      mRendererId;           // off+84
    std::string               mCustomName;           // off+88
    std::string               mFilteredCustomName;   // off+120
    int                       mRepairCost;           // off+152
    bool                      mClientSideOnly;       // off+156
    bool                      mIsMovable;            // off+157
    bool                      mSaveCustomName;       // off+158
    bool                      mCanRenderCustomName;  // off+159
    const float               signShadowRadius;      // off+160
    ActorTerrainInterlockData mTerrainInterlockData; // off+168
    bool                      mChanged;              // off+192

    virtual ~BlockActor() = default;
};
static_assert(offsetof(BlockActor, mChanged) == 192);
#endif
