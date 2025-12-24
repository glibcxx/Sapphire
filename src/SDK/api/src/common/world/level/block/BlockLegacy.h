#pragma once

#include "BlockRenderLayer.h"
#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/world/level/BlockPos.h"
#include "SDK/api/src/common/world/level/block/actor/BlockActorRendererId.h"
#include "SDK/api/src/common/world/level/block/components/BlockComponentStorage.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src/common/world/phys/HitResult.h"
#include "SDK/api/src-deps/Core/Utility/optional_ref.h"

class Block;
class BlockSource;
class IConstBlockSource;
class GetCollisionShapeInterface;

enum class ShapeType : uint8_t {
    Outline = 0,
    Collision = 1,
    CollisionForCamera = 2,
};

enum class BlockProperty : uint64_t {
    None = 0,
    Stair = 1,
    HalfSlab = 1ull << 1,
    Hopper = 1ull << 2,
    TopSnow = 1ull << 3,
    FenceGate = 1ull << 4,
    Leaves = 1ull << 5,
    ThinConnects2D = 1ull << 6,
    Connects2D = 1ull << 7,
    Carpet = 1ull << 8,
    Button = 1ull << 9,
    Door = 1ull << 10,
    Portal = 1ull << 11,
    CanFall = 1ull << 12,
    Snow = 1ull << 13,
    Trap = 1ull << 14,
    Sign = 1ull << 15,
    Walkable = 1ull << 16,
    PressurePlate = 1ull << 17,
    PistonBlockGrabber = 1ull << 18,
    TopSolidBlocking = 1ull << 19,
    SolidBlocking = 1ull << 20,
    CubeShaped = 1ull << 21,
    Power_NO = 1ull << 22,
    Power_BlockDown = 1ull << 23,
    Immovable = 1ull << 24,
    BreakOnPush = 1ull << 25,
    Piston = 1ull << 26,
    InfiniBurn = 1ull << 27,
    RequiresWorldBuilder = 1ull << 28,
    CausesDamage = 1ull << 29,
    BreaksWhenFallenOnByHeavy = 1ull << 30,
    OnlyPistonPush = 1ull << 31,
    Liquid = 1ull << 32,
    CanBeBuiltOver = 1ull << 33,
    SnowRecoverable = 1ull << 34,
    Scaffolding = 1ull << 35,
    CanSupportCenterHangingBlock = 1ull << 36,
    BreaksWhenHitByArrow = 1ull << 37,
    Unwalkable = 1ull << 38,
    Impenetrable = 1ull << 39,
    Hollow = 1ull << 40,
    OperatorBlock = 1ull << 41,
    SupportedByFlowerPot = 1ull << 42,
    PreventsJumping = 1ull << 43,
    ContainsHoney = 1ull << 44,
    Slime = 1ull << 45,
    SculkReplaceable = 1ull << 46,
    Climbable = 1ull << 47,
    CanHaltWhenClimbing = 1ull << 48,
    CanDamperVibrations = 1ull << 49,
    CanOccludeVibrations = 1ull << 50,
};

// https://github.com/LiteLDev/LeviLamina/blob/565421538b623fef8d5cf2f96450809fb9b94613/src/mc/world/level/block/BlockLegacy.h#L400

class BlockLegacy {
public:
    // size: 176
    struct NameInfo {
        HashedString mRawName;           // off+0
        std::string  mNamespaceName;     // off+48
        HashedString mFullName;          // off+80
        HashedString mPreFlatteningName; // off+128
    };

    std::string           mDescriptionId;                // off+8
    BlockComponentStorage mComponents;                   // off+40
    NameInfo              mNameInfo;                     // off+144
    BlockProperty         mProperties;                   // off+320
    bool                  mFancy;                        // off+328
    BlockRenderLayer      mRenderLayer;                  // off+329
    bool                  mRenderLayerCanRenderAsOpaque; // off+330
    BlockActorType        mBlockEntityType;              // off+332

    // ...

    // vtb+0
    virtual ~BlockLegacy();

    // vtb+1
    virtual std::shared_ptr<BlockActor> newBlockEntity(const BlockPos &pos, const Block &block) const;

    // vtb+2
    virtual const Block *getNextBlockPermutation(const Block &currentBlock) const;

    // vtb+3
    virtual bool hasTag(BlockSource &region, const BlockPos &pos, const Block &block, const std::string &tagName) const;

    // vtb+4
    virtual HitResult clip(
        const Block                                   &block,
        const BlockSource                             &region,
        const BlockPos                                &pos,
        const Vec3                                    &A,
        const Vec3                                    &B,
        ShapeType                                      shapeType,
        optional_ref<const GetCollisionShapeInterface> entity
    ) const;

    // vtb+5
    virtual AABB getCollisionShape(
        const Block                                   &block,
        const IConstBlockSource                       &region,
        const BlockPos                                &pos,
        optional_ref<const GetCollisionShapeInterface> entity
    ) const;

    // vtb+6
    virtual bool getCollisionShapeForCamera(
        AABB                    &outAABB,
        const Block             &block,
        const IConstBlockSource &region,
        const BlockPos          &pos
    ) const;

    // vtb+7
    virtual bool addCollisionShapes(
        const Block                                       &block,
        const IConstBlockSource                           &region,
        const BlockPos                                    &pos,
        const AABB                                        *intersectTestBox,
        ::std::vector<::AABB>                             &inoutBoxes,
        ::optional_ref<::GetCollisionShapeInterface const> entity
    ) const;

    // vtb+8
    virtual void addAABBs(
        const Block             &block,
        const IConstBlockSource &region,
        const BlockPos          &pos,
        const AABB              *intersectTestBox,
        std::vector<AABB>       &inoutBoxes
    ) const;

    // vtb+9
    virtual const AABB &getOutline(
        const Block &block, const IConstBlockSource &region, const BlockPos &pos, AABB &bufferValue
    ) const;

    // vtb+10
    virtual const AABB &getVisualShapeInWorld(
        const Block &block, const IConstBlockSource &region, const BlockPos &pos, AABB &bufferAABB
    ) const;

    // ...
};