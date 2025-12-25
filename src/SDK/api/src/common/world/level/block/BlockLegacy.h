#pragma once

#include "BlockRenderLayer.h"
#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/world/Flip.h"
#include "SDK/api/src/common/world/Direction.h"
#include "SDK/api/src/common/world/level/BlockPos.h"
#include "SDK/api/src/common/world/level/block/actor/BlockActorRendererId.h"
#include "SDK/api/src/common/world/level/block/components/BlockComponentStorage.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src/common/world/phys/HitResult.h"
#include "SDK/api/src/common/resources/BaseGameVersion.h"
#include "SDK/api/src-deps/Core/Utility/optional_ref.h"

class Block;
class BlockSource;
class IConstBlockSource;
class GetCollisionShapeInterface;
class CopperBehavior;
class ItemInstance;
class Container;
class ItemStack;
class BlockItem;
class MobSpawnerData;
class SpawnConditions;
class Random;
class Experiments;
struct BlockAnimateTickData;
struct UpdateEntityAfterFallOnInterface;
struct ActorBlockSyncMessage;
struct ResourceDropsContext;
struct BlockGraphicsModeChangeContext;
enum class FertilizerType : uint8_t;

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

enum class BlockSupportType : int {
    Center = 0,
    Edge = 1,
    Any = 2,
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
        const Block                                   &block,
        const IConstBlockSource                       &region,
        const BlockPos                                &pos,
        const AABB                                    *intersectTestBox,
        std::vector<AABB>                             &inoutBoxes,
        optional_ref<const GetCollisionShapeInterface> entity
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

    // vtb+11
    virtual const AABB &getVisualShape(const Block &block, AABB &bufferAABB) const;

    // vtb+12
    virtual const AABB &getUIShape(const Block &block, AABB &bufferAABB) const;

    // vtb+13
    virtual bool
    getLiquidClipVolume(const Block &block, BlockSource &region, const BlockPos &pos, AABB &includeBox) const;

    // vtb+14
    virtual bool isObstructingChests(BlockSource &region, const BlockPos &pos, const Block &thisBlock) const;

    // vtb+16
    virtual Vec3 randomlyModifyPosition(const BlockPos &pos, int &seed) const;

    // vtb+15
    virtual Vec3 randomlyModifyPosition(const BlockPos &pos) const;

    // vtb+17
    virtual void onProjectileHit(BlockSource &region, const BlockPos &pos, const Actor &projectile) const;

    // vtb+18
    virtual void onLightningHit(BlockSource &region, const BlockPos &pos) const;

    // vtb+19
    virtual bool liquidCanFlowIntoFromDirection(
        FacingID                                              flowIntoFacing,
        const std::function<const Block &(const BlockPos &)> &getBlock,
        const BlockPos                                       &pos
    ) const;

    // vtb+20
    virtual bool hasVariableLighting() const;

    // vtb+21
    virtual bool isStrippable(const Block &srcBlock) const;

    // vtb+22
    virtual const Block &getStrippedBlock(const Block &srcBlock) const;

    // vtb+23
    virtual bool canProvideSupport(const Block &block, FacingID face, BlockSupportType type) const;

    // vtb+24
    virtual bool canProvideMultifaceSupport(const Block &block, FacingID face) const;

    // vtb+25
    virtual bool canConnect(const Block &block, FacingID toOther, const Block &thisBlock) const;

    // vtb+26
    virtual bool isMovingBlock() const;

    // vtb+27
    virtual const CopperBehavior *tryGetCopperBehavior() const;

#if MC_VERSION == v1_21_2
    // vtb+28
    virtual bool canDamperVibrations() const;

    // vtb+29
    virtual bool canOccludeVibrations() const;
#endif

    // vtb+28
    virtual bool isStemBlock() const;

    // vtb+29
    virtual bool isContainerBlock() const;

    // vtb+30
    virtual bool isCraftingBlock() const;

    // vtb+31
#if MC_VERSION == v1_21_2
    virtual bool isWaterBlocking() const;
#else
    virtual bool isLavaBlocking() const;
#endif

    // vtb+32
    virtual bool isFenceBlock() const;

    // vtb+33
    virtual bool isFenceGateBlock() const;

    // vtb+34
    virtual bool isThinFenceBlock() const;

    // vtb+35
    virtual bool isWallBlock() const;

    // vtb+36
    virtual bool isStairBlock() const;

    // vtb+37
    virtual bool isSlabBlock() const;

#if MC_VERSION == v1_21_2
    // vtb+40
    virtual bool isDoubleSlabBlock() const;
#endif

    // vtb+38
    virtual bool isDoorBlock() const;

    // vtb+39
    virtual bool isRailBlock() const;

    // vtb+40
    virtual bool isButtonBlock() const;

    // vtb+41
    virtual bool isLeverBlock() const;

    // vtb+42
    virtual bool isCandleCakeBlock() const;

    // vtb+43
    virtual bool isMultifaceBlock() const;

    // vtb+44
    virtual bool isSignalSource() const;

#if MC_VERSION >= v1_21_50
    // vtb+45
    virtual bool isConsumerComponent() const;
#endif

    // vtb+46
    virtual bool canBeOriginalSurface() const;

    // vtb+47
    virtual bool isSilentWhenJumpingOff() const;

    // vtb+48
    virtual bool isValidAuxValue(int value) const;

    // vtb+49
    virtual bool canFillAtPos(BlockSource &region, const BlockPos &pos, const Block &block) const;

    // vtb+50
    virtual const Block &sanitizeFillBlock(const Block &block) const;

    // vtb+51
    virtual void onFillBlock(BlockSource &region, const BlockPos &pos, const Block &block) const;

    // vtb+52
    virtual int getDirectSignal(BlockSource &region, const BlockPos &pos, int dir) const;

#if MC_VERSION == v1_21_2
    // vyb+55
    virtual bool canBeDestroyedByWaterSpread() const;

    // vyb+56
    virtual bool waterSpreadCausesSpawn() const;

    // vyb+57
    virtual bool canContainLiquid() const;
#endif

    // vtb+53
    virtual std::optional<HashedString> getRequiredMedium() const;

    // vtb+54
    virtual bool shouldConnectToRedstone(BlockSource &region, const BlockPos &pos, Direction::Type direction) const;

    // vtb+55
    virtual void handlePrecipitation(
        BlockSource &region, const BlockPos &pos, float downfallAmount, float temperature
    ) const;

    // vtb+56
    virtual bool canBeUsedInCommands(const BaseGameVersion &baseGameVersion) const;

    // vtb+57
    virtual bool checkIsPathable(Actor &entity, const BlockPos &lastPathPos, const BlockPos &pathPos) const;

    // vtb+58
    virtual bool shouldDispense(BlockSource &region, Container &container) const;

    // vtb+59
    virtual bool dispense(BlockSource &region, Container &container, int slot, const Vec3 &pos, FacingID face) const;

    // vtb+60
    virtual void transformOnFall(BlockSource &region, const BlockPos &pos, Actor *entity, float fallDistance) const;

    // vtb+61
    virtual void onRedstoneUpdate(BlockSource &region, const BlockPos &pos, int strength, bool isFirstTime) const;

    // vtb+62
    virtual void onMove(BlockSource &region, const BlockPos &from, const BlockPos &to) const;

    // vtb+63
    virtual bool detachesOnPistonMove(BlockSource &region, const BlockPos &pos) const;

    // vtb+64
    virtual void movedByPiston(BlockSource &region, const BlockPos &pos) const;

    // vtb+65
    virtual void onStructureBlockPlace(BlockSource &region, const BlockPos &pos) const;

    // vtb+66
    virtual void onStructureNeighborBlockPlace(BlockSource &region, const BlockPos &pos) const;

    // vtb+67
    virtual void setupRedstoneComponent(BlockSource &region, const BlockPos &pos) const;

#if MC_VERSION == v1_21_2
    // vyb+73
    virtual BlockProperty getRedstoneProperty(BlockSource &region, const BlockPos &pos) const;
#endif

    // vtb+68
    virtual void updateEntityAfterFallOn(const BlockPos &pos, UpdateEntityAfterFallOnInterface &entity) const;

    // vtb+69
    virtual bool isBounceBlock() const;

    // vtb+70
    virtual bool isPreservingMediumWhenPlaced(::BlockLegacy const *medium) const;

    // vtb+71
    virtual bool isFilteredOut(BlockRenderLayer) const;

    // vtb+72
    virtual bool canRenderSelectionOverlay(BlockRenderLayer) const;

    // vtb+73
    virtual bool ignoreEntitiesOnPistonMove(const Block &block) const;

    // vtb+74
    virtual bool onFertilized(BlockSource &region, const BlockPos &pos, Actor *actor, FertilizerType fType) const;

    // vtb+75
    virtual bool mayConsumeFertilizer(BlockSource &region) const;

    // vtb+76
    virtual bool canBeFertilized(BlockSource &region, const BlockPos &pos, const Block &aboveBlock) const;

    // vtb+78
    virtual bool mayPick() const;

    // vtb+77
    virtual bool mayPick(BlockSource const &region, const Block &block, bool liquid) const;

    // vtb+80
    virtual bool mayPlace(BlockSource &region, const BlockPos &pos, FacingID face) const;

    // vtb+79
    virtual bool mayPlace(BlockSource &region, const BlockPos &pos) const;

    // vtb+81
    virtual bool mayPlaceOn(BlockSource &region, const BlockPos &pos) const;

    // vtb+82
    virtual bool tryToPlace(
        BlockSource                 &region,
        const BlockPos              &pos,
        const Block                 &block,
        const ActorBlockSyncMessage *syncMsg
    ) const;

    // vtb+83
    virtual bool tryToTill(BlockSource &region, const BlockPos &pos, Actor &entity, ItemStack &item) const;

    // vtb+84
    virtual bool breaksFallingBlocks(const Block &block, const BaseGameVersion version) const;

    // vtb+85
    virtual void
    destroy(BlockSource &region, const BlockPos &pos, const Block &block, Actor *entitySource) const;

    // vtb+86
    virtual bool getIgnoresDestroyPermissions(Actor &entity, const BlockPos &pos) const;

    // vtb+87
    virtual void neighborChanged(BlockSource &region, const BlockPos &pos, const BlockPos &neighborPos) const;

    // vtb+88
    virtual bool getSecondPart(const IConstBlockSource &region, const BlockPos &pos, BlockPos &out) const;

    // vtb+89
    virtual const Block *playerWillDestroy(Player &player, const BlockPos &pos, const Block &block) const;

    // vtb+90
    virtual ItemInstance asItemInstance(const Block &block, const BlockActor *actor) const;

    // vtb+91
    virtual void spawnAfterBreak(
        BlockSource &region, const Block &block, const BlockPos &pos, const ResourceDropsContext &context
    ) const;

    // vtb+92
    virtual const Block &getPlacementBlock(
        const Actor &by, const BlockPos &pos, FacingID face, const Vec3 &clickPos, int itemValue
    ) const;

    // vtb+93
    virtual int calcVariant(BlockSource &region, const BlockPos &pos, const mce::Color &baseColor) const;

    // vtb+94
    virtual bool isAttachedTo(BlockSource &region, const BlockPos &pos, BlockPos &outAttachedTo) const;

    // vtb+95
    virtual bool attack(Player *player, const BlockPos &pos) const;

    // vtb+96
    virtual bool shouldTriggerEntityInside(BlockSource &region, const BlockPos &pos, Actor &entity) const;

    // vtb+98
    virtual bool canBeBuiltOver(BlockSource &region, const BlockPos &pos, const BlockItem &newItem) const;

    // vtb+97
    virtual bool canBeBuiltOver(BlockSource &region, const BlockPos &pos) const;

    // vtb+99
    virtual void triggerEvent(BlockSource &region, const BlockPos &pos, int b0, int b1) const;

    // vtb+100
    virtual void executeEvent(
        BlockSource       &region,
        const BlockPos    &pos,
        const Block       &block,
        std::string const &eventName,
        Actor             &sourceEntity
    ) const;

    // vtb+101
    virtual const MobSpawnerData *getMobToSpawn(const SpawnConditions &conditions, BlockSource &region) const;

    // vtb+102
    virtual bool shouldStopFalling(Actor &entity) const;

    // vtb+103
    virtual bool pushesUpFallingBlocks() const;

    // vtb+104
    virtual bool canHaveExtraData() const;

    // vtb+105
    virtual bool hasComparatorSignal() const;

    // vtb+106
    virtual int getComparatorSignal(BlockSource &region, const BlockPos &pos, const Block &block, FacingID dir) const;

    // vtb+107
    virtual bool canSlide(BlockSource &region, const BlockPos &pos) const;

    // vtb+108
    virtual bool canInstatick() const;

    // vtb+109
    virtual bool canSpawnAt(BlockSource const &region, const BlockPos &pos) const;

    // vtb+110
    virtual void notifySpawnedAt(BlockSource &region, const BlockPos &pos) const;

    // vtb+111
    virtual bool causesFreezeEffect() const;

    // vtb+112
    virtual std::string buildDescriptionId(const Block &) const;

    // vtb+113
    virtual bool isAuxValueRelevantForPicking() const;
#if MC_VERSION == v1_21_2

    // vtb+121
    virtual int getColor(const Block &block) const;

    // vtb+122
    virtual int getColor(BlockSource &region, const BlockPos &pos, const Block &block) const;

    // vtb+123
    virtual int getColorAtPos(BlockSource &region, const BlockPos &pos) const;

    // vtb+124
    virtual int getColorForParticle(BlockSource &region, const BlockPos &pos, const Block &block) const;
#endif

    // vtb+114
    virtual bool isSeasonTinted(const Block &block, BlockSource &region, const BlockPos &p) const;

    // vtb+115
    virtual void onGraphicsModeChanged(const BlockGraphicsModeChangeContext &context);

    // vtb+116
    virtual float getShadeBrightness(const Block &block) const;

    // vtb+117
    virtual int telemetryVariant(BlockSource &region, const BlockPos &pos) const;

    // vtb+118
    virtual int getVariant(const Block &block) const;

    // vtb+119
    virtual bool canSpawnOn(Actor *) const;

    // vtb+120
    virtual const Block &getRenderBlock() const;

    // vtb+121
    virtual FacingID getMappedFace(FacingID face, const Block &block) const;

    // vtb+122
    virtual Flip getFaceFlip(FacingID face, const Block &block) const;

// vtb+123
#if MC_VERSION <= v1_21_50
    virtual void animateTickBedrockLegacy(BlockSource &region, const BlockPos &pos, const Random &random) const;
#elif MC_VERSION == v1_21_60
    virtual void animateTickBedrockLegacy(const BlockAnimateTickData &animateTickData) const;
#endif

// vtb+124
#if MC_VERSION <= v1_21_50
    virtual void animateTick(BlockSource &region, const BlockPos &pos, const Random &random) const;
#elif MC_VERSION == v1_21_60
    virtual void animateTick(const BlockAnimateTickData &animateTickData) const;
#endif

    // vtb+125
    virtual BlockLegacy &init();

    // vtb+126
    virtual Brightness getLightEmission(const Block &block) const;

    // vtb+127
    virtual const Block *tryLegacyUpgrade(DataID) const;

    // vtb+128
    virtual bool dealsContactDamage(const Actor &actor, const Block &block, bool isPathFinding) const;

    // vtb+129
    virtual const Block *tryGetInfested(const Block &block) const;

    // vtb+130
    virtual const Block *tryGetUninfested(const Block &block) const;

    // vtb+131
    virtual void _addHardCodedBlockComponents(const Experiments &experiments);

    // vtb+132
    virtual void onRemove(BlockSource &region, const BlockPos &pos) const;

    // vtb+133
    virtual void onExploded(BlockSource &region, const BlockPos &pos, Actor *entitySource) const;

    // vtb+134
    virtual void onStandOn(::EntityContext &entity, const BlockPos &pos) const;

#if MC_VERSION == v1_21_2
    // vtb+146
    virtual void onPlace(BlockSource &region, const BlockPos &pos) const;
#endif

    // vtb+135
    virtual bool shouldTickOnSetBlock() const;

    // vtb+136
    virtual void tick(BlockSource &region, const BlockPos &pos, const Random &random) const;

    // vtb+137
    virtual void randomTick(BlockSource &region, const BlockPos &pos, const Random &random) const;

    // vtb+138
    virtual bool isInteractiveBlock() const;

    // vtb+140
    virtual bool use(Player &player, const BlockPos &pos, FacingID face, std::optional<Vec3>) const;

    // vtb+139
    virtual bool use(Player &player, const BlockPos &pos, FacingID face) const;

    // vtb+141
    virtual bool allowStateMismatchOnPlacement(const Block &clientTarget, const Block &serverTarget) const;

    // vtb+142
    virtual bool canSurvive(BlockSource &region, const BlockPos &pos) const;

#if MC_VERSION <= v1_21_50
    // vtb+144
    virtual BlockRenderLayer getRenderLayer() const;
#endif

    // vtb+143
    virtual BlockRenderLayer getRenderLayer(const Block &block, BlockSource &region, const BlockPos &pos) const;

    // vtb+145
    virtual int getExtraRenderLayers() const;

    // vtb+146
    virtual Brightness getLight(const Block &block) const;

    // vtb+147
    virtual Brightness getEmissiveBrightness(const Block &block) const;

    // vtb+148
    virtual mce::Color getMapColor(BlockSource &region, const BlockPos &pos, const Block &block) const;

    // vtb+149
    virtual void _onHitByActivatingAttack(BlockSource &region, const BlockPos &pos, Actor *) const;

    // vtb+150
    virtual void entityInside(BlockSource &region, const BlockPos &pos, Actor &entity) const;

#if MC_VERSION == v1_21_2

    // vtb+163
    virtual mce::Color getDustColor(const Block &block) const;

    // vtb+164
    virtual void _iterateCandles(
        const Block &block, const BlockPos &pos, std::function<void(const Vec3 &, int)> callback
    ) const;

    // vtb+165
    virtual const ItemInstance getBaseSeed() const;

    // vtb+166
    virtual void onLand(BlockSource &region, const BlockPos &pos) const;

    // vtb+167
    virtual bool isFreeToFall(BlockSource &region, const BlockPos &pos) const;

    // vtb+168
    virtual void startFalling(BlockSource &region, const BlockPos &pos, const Block &oldBlock, bool creative) const;

    // vtb+169
    virtual int getInputSignal(BlockSource &region, const BlockPos &pos) const;

    // vtb+170
    virtual bool isAlternateInput(const Block &block) const;

    // vtb+171
    virtual int getAlternateSignal(BlockSource &region, const BlockPos &pos) const;

    // vtb+172
    virtual int getOutputSignal(const Block &block) const;

    // vtb+173
    virtual int getTurnOffDelay(const Block &block) const;

    // vtb+174
    virtual int getTurnOnDelay(const Block &block) const;

    // vtb+175
    virtual const Block *getOnBlock(const Block *block) const;

    // vtb+176
    virtual const Block *getOffBlock(const Block *block) const;
#endif
};