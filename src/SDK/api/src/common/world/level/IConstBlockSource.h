#pragma once

#include "SDK/api/src-deps/Core/Utility/optional_ref.h"
#include "SDK/api/src/common/util/Bounds.h"
#include "SDK/api/src/common/world/level/BlockPos.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include "dimension/DimensionType.h"
#include "material/MaterialType.h"

class Block;
class BlockActor;
class Material;
class GetCollisionShapeInterface;
namespace BlockSourceVisitor {
    struct CollisionShape;
};

// https://github.com/LiteLDev/LeviLamina/blob/b48d3758d352ae4567e9a549e05b05234bd2d82f/src/mc/world/level/IConstBlockSource.h#L16
// https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/world/level/IConstBlockSource.h#L24
// https://github.com/LiteLDev/LeviLamina/blob/565421538b623fef8d5cf2f96450809fb9b94613/src/mc/deps/vanilla_components/IConstBlockSource.h#L24

// size: 8
class IConstBlockSource {
public:
    // vtb+0
    virtual ~IConstBlockSource() = default;

    // vtb+1
    virtual const Block &getBlock(const BlockPos &pos, uint32_t layer) const = 0;

    // vtb+2
    virtual const Block &getBlock(const BlockPos &pos) const = 0;

    // vtb+3
    virtual const Block &getBlock(int x, int y, int z) const = 0;

    // vtb+4
    virtual const BlockActor *getBlockEntity(const BlockPos &pos) const = 0;

    // vtb+5
    virtual const Block &getExtraBlock(const BlockPos &pos) const = 0;

    // vtb+6
    virtual const Block &getLiquidBlock(const BlockPos &pos) const = 0;

    // vtb+7
    virtual bool hasBlock(const BlockPos &pos) const = 0;

    // vtb+8
    virtual bool containsAnyLiquid(const AABB &box) const = 0;

    // vtb+9
    virtual bool containsMaterial(const AABB &box, MaterialType material) const = 0;

#if MC_VERSION == v1_21_60
    // vtb+10
    virtual bool isInWall(const Vec3 &pos) const = 0;
#endif

    // vtb+10
    virtual bool isUnderWater(const Vec3 &pos, const Block &block) const = 0;

    // vtb+11
    virtual const Material &getMaterial(int x, int y, int z) const = 0;

    // vtb+12
    virtual const Material &getMaterial(const BlockPos &pos) const = 0;

    // vtb+13
    virtual bool hasBorderBlock(const BlockPos pos) const = 0;

    // vtb+14
    virtual bool hasChunksAt(const AABB &bb, bool ignoreClientGenerated) const = 0;

    // vtb+15
    virtual bool hasChunksAt(const BlockPos &pos, int r, bool ignoreClientGenerated) const = 0;

    // vtb+16
    virtual bool hasChunksAt(const Bounds &bounds, bool ignoreClientGenerated) const = 0;

    // vtb+17
    virtual DimensionType getDimensionId() const = 0;

    // vtb+18
    virtual void fetchAABBs(std::vector<AABB> &result, const AABB &intersectTestBox, bool withUnloadedChunks) const = 0;

    // vtb+19
    virtual void fetchCollisionShapes(
        std::vector<AABB>                             &result,
        const AABB                                    &box,
        bool                                           withUnloadedChunks,
        optional_ref<const GetCollisionShapeInterface> entity,
        std::vector<AABB> *
    ) const = 0;

    // vtb+20
    virtual void fetchCollisionShapesAndBlocks(
        std::vector<BlockSourceVisitor::CollisionShape> &result,
        const AABB                                      &box,
        bool                                             withUnloadedChunks,
        optional_ref<const GetCollisionShapeInterface>   entity,
        std::vector<AABB> *
    ) const = 0;

    // vtb+21
    virtual AABB getTallestCollisionShape(
        const AABB                                    &box,
        float                                         *actualSurfaceOffset,
        bool                                           withUnloadedChunks,
        optional_ref<const GetCollisionShapeInterface> entity
    ) const = 0;

    // vtb+22
    virtual float getBrightness(const BlockPos &pos) const = 0;
};
