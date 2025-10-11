#pragma once

#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src/common/world/level/BlockPos.h"
#include "SDK/api/src/common/world/actor/ActorTerrainInterlockData.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "BlockActorRendererId.h"

class Level;
class CompoundTag;
class DataLoadHelper;
class BlockSource;
class IConstBlockSource;
class PistonBlockActor;
class UIProfanityContext;

#if MC_VERSION >= v1_21_60
namespace Bedrock::Safety {
    class RedactableString {
        std::string                  mUnredactedString;
        std::optional<::std::string> mRedactedString;
    };
} // namespace Bedrock::Safety
#endif

// size: 192 (1.21.2/1.21.50), 232 (1.21.60)
class BlockActor {
public:
#if MC_VERSION == v1_21_2 || MC_VERSION == v1_21_50
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
#elif MC_VERSION == v1_21_60
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
#endif

    // vtb+0
    virtual ~BlockActor();

    // vtb+1
    virtual void load(Level &level, const CompoundTag &tag, DataLoadHelper &dataLoadHelper);

    // vtb+2
    virtual bool save(CompoundTag &tag) const;

    // vtb+3
    virtual bool saveItemInstanceData(CompoundTag &tag);

    // vtb+4
    virtual void saveBlockData(CompoundTag &tag, BlockSource &region) const;

    // vtb+5
    virtual void loadBlockData(const CompoundTag &tag, BlockSource &region, DataLoadHelper &dataLoadHelper);

    // vtb+6
    virtual void onCustomTagLoadDone(BlockSource &region);

    // vtb+7
    virtual void tick(BlockSource &region);

    // vtb+8
    virtual void onChanged(BlockSource &region);

    // vtb+9
    virtual bool isMovable(BlockSource &region);

    // vtb+10
    virtual bool isCustomNameSaved();

    // vtb+11
    virtual void onPlace(BlockSource &region);

    // vtb+12
    virtual void onMove();

    // vtb+13
    virtual void onRemoved(BlockSource &region);

    // vtb+14
    virtual bool isPreserved(BlockSource &region) const;

    // vtb+15
    virtual bool shouldPreserve(BlockSource &region);

    // vtb+16
    virtual void triggerEvent(int b0, int b1);

    // vtb+17
    virtual void clearCache();

    // vtb+18
    virtual void onNeighborChanged(BlockSource &region, const BlockPos &position);

    // vtb+19
    virtual float getShadowRadius(BlockSource &region) const;

    // vtb+20
    virtual bool hasAlphaLayer() const;

    // vtb+21
    virtual BlockActor *getCrackEntity(BlockSource &region, const BlockPos &pos);

    // vtb+22
    virtual AABB getCollisionShape(const IConstBlockSource &region) const;

    // vtb+23
    virtual void getDebugText(std::vector<std::string> &outputInfo, const BlockPos &debugPos);

    // vtb+24
    virtual const std::string &getCustomName() const;

    // vtb+25
    virtual const std::string &getFilteredCustomName(const Bedrock::NotNullNonOwnerPtr<UIProfanityContext> &context);

    // vtb+26
    virtual std::string getName() const;

    // vtb+27
    virtual void setCustomName(const std::string &name);

    // vtb+28
    virtual std::string getImmersiveReaderText(BlockSource &region);

    // vtb+29
    virtual int getRepairCost() const;

    // vtb+31
    virtual PistonBlockActor *getOwningPiston(BlockSource &region);

    // vtb+30
    virtual const PistonBlockActor *getOwningPiston(BlockSource &region) const;

    // ...
};
