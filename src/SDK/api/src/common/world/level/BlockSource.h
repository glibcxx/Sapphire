#pragma once

#include "IConstBlockSource.h"

class Level;
class ChunkSource;
class LevelChunk;
struct ActorBlockSyncMessage;

class BlockSource : public IConstBlockSource, public std::enable_shared_from_this<BlockSource> {
public:
    const std::thread::id mOwnerThreadID;          // off+24
    const bool            mAllowUnpopulatedChunks; // off+28
    const bool            mPublicSource;           // off+29
    bool                  mCheckValidity;          // off+30
    Level                &mLevel;                  // off+32

    SDK_API BlockSource(
        Level       &level,
        Dimension   &dimension,
        ChunkSource &source,
        bool         publicSource,
        bool         allowUnpopulatedChunks,
        bool         allowClientTickingChanges
    );

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x49\x89\x44\x24\x00\xF2\x0F\x10\x45")
    SPHR_CTOR_ALIAS SDK_API BlockSource *ctor(
        Level       &level,
        Dimension   &dimension,
        ChunkSource &source,
        bool         publicSource,
        bool         allowUnpopulatedChunks,
        bool         allowClientTickingChanges
    );

    BlockSource(const BlockSource &) = delete;

    // vtb+0
    SDK_API virtual ~BlockSource() override;

    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x9D\x00\x00\x00\x00\x48\x85\xDB\x0F\x84\x00\x00\x00\x00\x48\x8B\xB5\x00\x00\x00\x00\x48\x3B\xDE\x74\x00\x0F\x1F\x44")
    SPHR_DTOR_ALIAS SDK_API void dtor() noexcept;

    // vtb+1
    virtual const Block &getBlock(const BlockPos &pos, uint32_t layer) const override;

    // vtb+2
    virtual const Block &getBlock(const BlockPos &pos) const override;

    // vtb+3
    virtual const Block &getBlock(int x, int y, int z) const override;

    // vtb+4
    virtual const BlockActor *getBlockEntity(const BlockPos &pos) const override;

    // vtb+5
    virtual const Block &getExtraBlock(const BlockPos &pos) const override;

    // vtb+6
    virtual const Block &getLiquidBlock(const BlockPos &pos) const override;

    // vtb+7
    virtual bool hasBlock(const BlockPos &pos) const override;

    // vtb+8
    virtual bool containsAnyLiquid(const AABB &box) const override;

    // vtb+9
    virtual bool containsMaterial(const AABB &box, MaterialType material) const override;

#if MC_VERSION == v1_21_60
    // vtb+10
    virtual bool isInWall(const Vec3 &pos) const override;
#endif

    // vtb+10
    virtual bool isUnderWater(const Vec3 &pos, const Block &block) const override;

    // vtb+11
    virtual const Material &getMaterial(int x, int y, int z) const override;

    // vtb+12
    virtual const Material &getMaterial(const BlockPos &pos) const override;

    // vtb+13
    virtual bool hasBorderBlock(const BlockPos pos) const override;

    // vtb+14
    virtual bool hasChunksAt(const AABB &bb, bool ignoreClientGenerated) const override;

    // vtb+15
    virtual bool hasChunksAt(const BlockPos &pos, int r, bool ignoreClientGenerated) const override;

    // vtb+16
    virtual bool hasChunksAt(const Bounds &bounds, bool ignoreClientGenerated) const override;

    // vtb+17
    virtual DimensionType getDimensionId() const override;

    // vtb+18
    virtual void fetchAABBs(
        std::vector<AABB> &result, const AABB &intersectTestBox, bool withUnloadedChunks
    ) const override;

    // vtb+19
    virtual void fetchCollisionShapes(
        std::vector<AABB>                             &result,
        const AABB                                    &box,
        bool                                           withUnloadedChunks,
        optional_ref<const GetCollisionShapeInterface> entity,
        std::vector<AABB> *
    ) const override;

    // vtb+20
    virtual void fetchCollisionShapesAndBlocks(
        std::vector<BlockSourceVisitor::CollisionShape> &result,
        const AABB                                      &box,
        bool                                             withUnloadedChunks,
        optional_ref<const GetCollisionShapeInterface>   entity,
        std::vector<AABB> *
    ) const override;

    // vtb+21
    virtual AABB getTallestCollisionShape(
        const AABB                                    &box,
        float                                         *actualSurfaceOffset,
        bool                                           withUnloadedChunks,
        optional_ref<const GetCollisionShapeInterface> entity
    ) const override;

    // vtb+22
    virtual float getBrightness(const BlockPos &pos) const override;

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x45\x8B\xE9")
    SDK_API bool setBlock(
        const BlockPos              &pos,
        const Block                 &block,
        int                          updateFlags,
        std::shared_ptr<BlockActor>  blockEntity,
        const ActorBlockSyncMessage *syncMsg
    );

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "lea", "\x48\x8D\x0D\x00\x00\x00\x00\x48\x8D\x54\x24\x00\x48\x3B\xC1\x48\x8D\x4E")
    SDK_API LevelChunk *getChunk(const ChunkPos &pos) const;
};