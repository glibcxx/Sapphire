#pragma once

#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src/common/world/level/BlockPos.h"
#include "SDK/api/src/common/world/actor/ActorTerrainInterlockData.h"

enum BlockActorRendererId;

#if MC_VERSION >= v1_21_60
namespace Bedrock::Safety {
    class RedactableString {
        ::std::string                  mUnredactedString;
        ::std::optional<::std::string> mRedactedString;
    };
} // namespace Bedrock::Safety
#endif

enum class BlockActorType : int {
    Undefined = 0x0,
    Furnace = 0x1,
    Chest = 0x2,
    NetherReactor = 0x3,
    Sign = 0x4,
    MobSpawner = 0x5,
    Skull = 0x6,
    FlowerPot = 0x7,
    BrewingStand = 0x8,
    EnchantingTable = 0x9,
    DaylightDetector = 0xA,
    Music = 0xB,
    Comparator = 0xC,
    Dispenser = 0xD,
    Dropper = 0xE,
    Hopper = 0xF,
    Cauldron = 0x10,
    ItemFrame = 0x11,
    PistonArm = 0x12,
    MovingBlock = 0x13,
    Chalkboard = 0x14,
    Beacon = 0x15,
    EndPortal = 0x16,
    EnderChest = 0x17,
    EndGateway = 0x18,
    ShulkerBox = 0x19,
    CommandBlock = 0x1A,
    Bed = 0x1B,
    Banner = 0x1C,
    StructureBlock = 0x20,
    Jukebox = 0x21,
    ChemistryTable = 0x22,
    Conduit = 0x23,
    JigsawBlock = 0x24,
    Lectern = 0x25,
    BlastFurnace = 0x26,
    Smoker = 0x27,
    Bell = 0x28,
    Campfire = 0x29,
    BarrelBlock = 0x2A,
    Beehive = 0x2B,
    Lodestone = 0x2C,
    SculkSensor = 0x2D,
    SporeBlossom = 0x2E,
    GlowItemFrame = 0x2F,
    SculkCatalyst = 0x30,
    SculkShrieker = 0x31,
    HangingSign = 0x32,
    ChiseledBookshelf = 0x33,
    BrushableBlock = 0x34,
    DecoratedPot = 0x35,
    CalibratedSculkSensor = 0x36,
    Count = 0x37,
};

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
