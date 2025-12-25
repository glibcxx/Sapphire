#pragma once

#include "ActorDamageSource.h"
#include "ActorTypes.h"
#include "SDK/api/src-deps/Core/Math/Color.h"
#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "SDK/api/src/common/ActorUniqueID.h"
#include "SDK/api/src/common/entity/gamerefs_entity/EntityContext.h"
#include "SDK/api/src/common/world/level/dimension/DimensionType.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src/common/world/item/ItemHelper.h"
#include "SDK/api/src/common/world/item/EquipmentSlot.h"
#include "SDK/api/src/common/world/level/material/MaterialType.h"
#include "SDK/api/src/common/server/commands/CommandPermissionLevel.h"
#include "SDK/api/src/common/network/packet/types/world/actor/ActorEvent.h"
#include "SDK/api/src-deps/Input/InputMode.h"

class AddActorBasePacket;
class IConstBlockSource;
class Player;
class Block;
class ItemStack;
class CompoundTag;
class DataLoadHelper;
class ChangeDimensionPacket;
class MobEffectInstance;
class AnimationComponent;
class ItemStackBase;
class ActorInteraction;
class Options;
struct VariantParameterList;
struct PlayerMovementSettings;
namespace SharedTypes::Legacy {
    enum class LevelSoundEvent : unsigned int;
};
namespace MovementDataExtractionUtility {
    class SnapshotAccessor;
}

enum class ActorFlags : int {
    Unknown = -1,
    ONFIRE = 0,
    SNEAKING = 1,
    RIDING = 2,
    SPRINTING = 3,
    USINGITEM = 4,
    INVISIBLE = 5,
    TEMPTED = 6,
    INLOVE = 7,
    SADDLED = 8,
    POWERED = 9,
    IGNITED = 10,
    BABY = 11,
    CONVERTING = 12,
    CRITICAL = 13,
    CAN_SHOW_NAME = 14,
    ALWAYS_SHOW_NAME = 15,
    NOAI = 16,
    SILENT = 17,
    WALLCLIMBING = 18,
    CANCLIMB = 19,
    CANSWIM = 20,
    CANFLY = 21,
    CANWALK = 22,
    RESTING = 23,
    SITTING = 24,
    ANGRY = 25,
    INTERESTED = 26,
    CHARGED = 27,
    TAMED = 28,
    ORPHANED = 29,
    LEASHED = 30,
    SHEARED = 31,
    GLIDING = 32,
    ELDER = 33,
    MOVING = 34,
    BREATHING = 35,
    CHESTED = 36,
    STACKABLE = 37,
    SHOW_BOTTOM = 38,
    STANDING = 39,
    SHAKING = 40,
    IDLING = 41,
    CASTING = 42,
    CHARGING = 43,
    WASD_CONTROLLED = 44,
    CAN_POWER_JUMP = 45,
    CANDASH = 46,
    LINGERING = 47,
    HAS_COLLISION = 48,
    HAS_GRAVITY = 49,
    FIRE_IMMUNE = 50,
    DANCING = 51,
    ENCHANTED = 52,
    RETURNTRIDENT = 53,
    CONTAINER_IS_PRIVATE = 54,
    IS_TRANSFORMING = 55,
    DAMAGENEARBYMOBS = 56,
    SWIMMING = 57,
    BRIBED = 58,
    IS_PREGNANT = 59,
    LAYING_EGG = 60,
    RIDER_CAN_PICK = 64,
    TRANSITION_SITTING = 65,
    EATING = 66,
    LAYING_DOWN = 67,
    SNEEZING = 65,
    TRUSTING = 66,
    ROLLING = 67,
    SCARED = 68,
    IN_SCAFFOLDING = 69,
    OVER_SCAFFOLDING = 70,
    FALL_THROUGH_SCAFFOLDING = 71,
    BLOCKING = 72,
    TRANSITION_BLOCKING = 73,
    BLOCKED_USING_SHIELD = 74,
    BLOCKED_USING_DAMAGED_SHIELD = 75,
    SLEEPING = 76,
    WANTS_TO_WAKE = 77,
    TRADE_INTEREST = 78,
    DOOR_BREAKER = 79,
    BREAKING_OBSTRUCTION = 80,
    DOOR_OPENER = 81,
    IS_ILLAGER_CAPTAIN = 82,
    STUNNED = 83,
    ROARING = 84,
    DELAYED_ATTACK = 85,
    IS_AVOIDING_MOBS = 86,
    IS_AVOIDING_BLOCK = 87,
    FACING_TARGET_TO_RANGE_ATTACK = 88,
    HIDDEN_WHEN_INVISIBLE = 89,
    IS_IN_UI = 90,
    STALKING = 91,
    EMOTING = 92,
    CELEBRATING = 93,
    ADMIRING = 94,
    CELEBRATING_SPECIAL = 95,
    OUT_OF_CONTROL = 96,
    RAM_ATTACK = 97,
    PLAYING_DEAD = 98,
    IN_ASCENDABLE_BLOCK = 99,
    OVER_DESCENDABLE_BLOCK = 100,
    CROAKING = 101,
    EATMOB = 102,
    JUMP_GOAL_JUMP = 103,
    EMERGING = 104,
    SNIFFING = 105,
    DIGGING = 106,
    SONICBOOM = 107,
    HAS_DASHCOOLDOWN = 108,
    PUSH_TOWARDS_CLOSEST_SPACE = 109,
    DEPRECATED1 = 110,
    DEPRECATED2 = 111,
    DEPRECATED3 = 112,
    SEARCHING = 113,
    CRAWLING = 114,
    TIMER_FLAG1 = 115,
    TIMER_FLAG2 = 116,
    TIMER_FLAG3 = 117,
    BODY_ROTATION_BLOCKED = 118,
    RENDERS_WHEN_INVISIBLE = 119,
    Count,
};

enum class HorseInventoryEnum : int {
    INV_SLOT_SADDLE = 0,
    INV_SLOT_ARMOR = 1,
    INV_BASE_COUNT = 2,
};

enum class ArmorMaterialType : int {
    None = -1,
    DefaultArmor = 0,
    EnchantedArmor = 1,
    LeatherArmor = 2,
    EnchantedLeatherArmor = 3,
};

enum class ArmorTextureType : int {
    None = -1,
    Leather = 0,
    Chain = 1,
    Iron = 2,
    Diamond = 3,
    Gold = 4,
    Elytra = 5,
    Turtle = 6,
    Netherite = 7,
};

enum class SpawnRuleEnum : int {
    Undefined = -1,
    NoSpawnRules = 0,
    HasSpawnRules = 1,
};

// https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/world/actor/Actor.h#L109

class Actor {
public:
    enum class ActorInitializationMethod : uint8_t {
        INVALID = 0,
        LOADED = 1,
        SPAWNED = 2,
        BORN = 3,
        TRANSFORMED = 4,
        UPDATED = 5,
        EVENT = 6,
        LEGACY = 7,
    };

    EntityContext mEntityContext; // off+8

    // ...

    // vtb+(1.21.50)

#if MC_VERSION == v1_21_2
    // vtb+0
    virtual bool getStatusFlag(ActorFlags flag) const;

    // vtb+1
    virtual void setStatusFlag(ActorFlags flag, bool value);
#endif

    // vtb+0
    virtual bool hasComponent(const HashedString &name) const;

    // vtb+1
    virtual void outOfWorld();

    // vtb+2
    virtual void reloadHardcoded(ActorInitializationMethod method, const VariantParameterList &params);

    // vtb+3
    virtual void reloadHardcodedClient(ActorInitializationMethod method);

    // vtb+4
    virtual void initializeComponents(ActorInitializationMethod method, const VariantParameterList &params);

    // vtb+5
    virtual void reloadComponents(ActorInitializationMethod method, const VariantParameterList &params);

    // vtb+6
    virtual void _serverInitItemStackIds();

    // vtb+7
    virtual void _doInitialMove();

    // vtb+8
    virtual ~Actor();

    // vtb+9
    virtual void resetUserPos(bool);

    // vtb+10
    virtual ActorType getOwnerEntityType();

    // vtb+11
    virtual void remove();

    // vtb+12
    virtual Vec3 getFiringPos() const;

    // vtb+13
    virtual float getInterpolatedBodyRot(float a = 0.0f) const;

    // vtb+14
    virtual float getInterpolatedHeadRot(float a = 0.0f) const;

    // vtb+15
    virtual float getInterpolatedBodyYaw(float a = 0.0f) const;

    // vtb+16
    virtual float getYawSpeedInDegreesPerSecond() const;

    // vtb+17
    virtual Vec3 getInterpolatedRidingOffset(float a, const int) const;

    // vtb+18
    virtual bool isFireImmune() const;

    // vtb+19
    virtual void blockedByShield(const ActorDamageSource &source, Actor &blocker);

    // vtb+20
    virtual bool canDisableShield();

    // vtb+21
    virtual void teleportTo(const Vec3 &pos, bool shouldStopRiding, int cause, int entityType, bool keepVelocity);

    // vtb+22
    virtual void lerpMotion(const Vec3 &delta);

    // vtb+23
    virtual std::unique_ptr<AddActorBasePacket> tryCreateAddActorPacket();

    // vtb+24
    virtual void normalTick();

    // vtb+25
    virtual void baseTick();

    // vtb+26
    virtual void passengerTick();

    // vtb+27
    virtual bool startRiding(Actor &vehicle, bool forceRiding);

    // vtb+28
    virtual void addPassenger(Actor &passenger);

    // vtb+29
    virtual std::string getExitTip(const std::string &kind, InputMode mode, NewInteractionModel scheme) const;

    // vtb+30
    virtual std::string getEntityLocNameString() const;

#if MC_VERSION <= v1_21_50
    // vtb+31
    virtual bool isInWall() const;
#endif

    // vtb+32
    virtual bool isInvisible() const;

    // vtb+33
    virtual bool canShowNameTag() const;

    // vtb+34
    virtual std::string getFormattedNameTag() const;

    // vtb+35
    virtual mce::Color getNameTagTextColor() const;

    // vtb+36
    virtual float getShadowRadius() const;

    // vtb+37
    virtual Vec3 getHeadLookVector(float a = 0.0f) const;

    // vtb+38
    virtual bool canInteractWithOtherEntitiesInGame() const;

    // vtb+39
    virtual float getBrightness(float a, const IConstBlockSource &region) const;

    // vtb+40
    virtual void playerTouch(Player &player);

    // vtb+41
    virtual bool isImmobile() const;

    // vtb+42
    virtual bool isSilentObserver() const;

    // vtb+43
    virtual bool isSleeping() const;

    // vtb+44
    virtual void setSleeping(bool);

    // vtb+45
    virtual void setSneaking(bool value);

    // vtb+46
    virtual bool isBlocking() const;

    // vtb+47
    virtual bool isDamageBlocked(const ActorDamageSource &source) const;

    // vtb+48
    virtual bool isAlive() const;

    // vtb+49
    virtual bool isOnFire() const;

    // vtb+50
    virtual bool isSurfaceMob() const;

    // vtb+51
    virtual bool isTargetable() const;

#if MC_VERSION == v1_21_2
    // vtb+54
    virtual bool canAttack(Actor *entity, bool allowInvulnerable) const;
#endif

    // vtb+52
    virtual void setTarget(Actor *entity);

    // vtb+53
    virtual bool isValidTarget(Actor *attacker) const;

    // vtb+54
    virtual bool attack(Actor &target, const ActorDamageCause &cause);

    // vtb+55
    virtual void performRangedAttack(Actor &target, float power);

    // vtb+56
    virtual void setOwner(const ActorUniqueID ownerId);

    // vtb+57
    virtual void setSitting(bool value);

    // vtb+58
    virtual void onTame();

    // vtb+59
    virtual void onFailedTame();

    // vtb+60
    virtual void setStanding(bool value);

    // vtb+61
    virtual bool canPowerJump() const;

    // vtb+62
    virtual bool isEnchanted() const;

#if MC_VERSION <= v1_21_50
    // vtb+63
    virtual bool shouldRender() const;
#endif

    // vtb+64
    virtual void playAmbientSound();

    // vtb+65
    virtual SharedTypes::Legacy::LevelSoundEvent getAmbientSound() const;

    // vtb+66
    virtual bool isInvulnerableTo(const ActorDamageSource &source) const;

    // vtb+67
    virtual ActorDamageCause getBlockDamageCause(const Block &block) const;

    // vtb+68
    virtual bool doFireHurt(int amount);

    // vtb+69
    virtual void onLightningHit();

    // vtb+70
    virtual void feed(int itemId);

    // vtb+71
    virtual void handleEntityEvent(ActorEvent eventId, int data);

    // vtb+72
    virtual const HashedString &getActorRendererId() const;

    // vtb+73
    virtual void despawn();

    // vtb+74
    virtual void setArmor(ArmorSlot slot, const ItemStack &item);

    // vtb+75
    virtual ArmorMaterialType getArmorMaterialTypeInSlot(ArmorSlot slot) const;

    // vtb+76
    virtual int getArmorTextureIndexInSlot(ArmorSlot slot) const;

    // vtb+77
    virtual float getArmorColorInSlot(ArmorSlot slot, int channelRGBA) const;

    // vtb+78
    virtual void setEquippedSlot(SharedTypes::Legacy::EquipmentSlot slot, const ItemStack &item);

    // vtb+79
    virtual void setCarriedItem(const ItemStack &item);

    // vtb+80
    virtual const ItemStack &getCarriedItem() const;

    // vtb+81
    virtual void setOffhandSlot(const ItemStack &item);

    // vtb+82
    virtual const ItemStack &getEquippedTotem() const;

    // vtb+83
    virtual bool consumeTotem();

#if MC_VERSION == v1_21_2
    // vtb+87
    virtual bool save(CompoundTag &entityTag) const;
#elif MC_VERSION >= v1_21_50

    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4C\x8B\xFA\x48\x89\x55\x00\x4C\x8B\xF1\x48\x89\x4D\x00\x33\xDB")
    SDK_API bool save(CompoundTag &entityTag) const;

#endif

    // vtb+84
    virtual bool load(const CompoundTag &tag, DataLoadHelper &dataLoadHelper);

    // vtb+85
    virtual const HashedString &queryEntityRenderer() const;

    // vtb+86
    virtual ActorUniqueID getSourceUniqueID() const;

    // vtb+87
    virtual bool canFreeze() const;

    // vtb+88
    virtual AABB getLiquidAABB(const MaterialType liquidType) const;

    // vtb+89
    virtual void handleInsidePortal(const BlockPos &portalPos);

    // vtb+90
    virtual bool canChangeDimensionsUsingPortal() const;

    // vtb+92
    virtual void changeDimension(DimensionType toId);

    // vtb+91
    virtual void changeDimension(const ChangeDimensionPacket &packet);

    // vtb+93
    virtual ActorUniqueID getControllingPlayer() const;

    // vtb+94
    virtual float causeFallDamageToActor(float, float, ActorDamageSource);

    // vtb+95
    virtual void onSynchedDataUpdate(int dataId);

    // vtb+96
    virtual bool canAddPassenger(Actor &passenger) const;

    // vtb+97
    virtual bool canPickupItem(const ItemStack &itemStack) const;

    // vtb+98
    virtual bool canBePulledIntoVehicle() const;

    // vtb+99
    virtual bool inCaravan() const;

    // vtb+100
    virtual void sendMotionPacketIfNeeded(const PlayerMovementSettings &playerMovementSettings);

    // vtb+101
    virtual bool canSynchronizeNewEntity() const;

    // vtb+102
    virtual void startSwimming();

    // vtb+103
    virtual void stopSwimming();

    // vtb+104
    virtual void buildDebugInfo(std::string &out) const;

    // vtb+105
    virtual CommandPermissionLevel getCommandPermissionLevel() const;

    // vtb+106
    virtual int getDeathTime() const;

    // vtb+107
    virtual bool canBeAffected(int id) const;

    // vtb+108
    virtual bool canBeAffectedByArrow(const MobEffectInstance &effect) const;

    // vtb+109
    virtual void onEffectRemoved(MobEffectInstance &effect);

    // vtb+110
    virtual bool canObstructSpawningAndBlockPlacement() const;

    // vtb+111
    virtual AnimationComponent &getAnimationComponent();

    // vtb+112
    virtual void openContainerComponent(Player &player);

    // vtb+113
    virtual void swing();

    // vtb+114
    virtual void useItem(ItemStackBase &item, ItemUseMethod itemUseMethod, bool consumeItem);

    // vtb+115
    virtual void getDebugText(std::vector<std::string> &outputInfo);

    // vtb+116
    virtual float getMapDecorationRotation() const;

    // vtb+117
    virtual float getPassengerYRotation(const Actor &passenger) const;

    // vtb+118
    virtual bool add(ItemStack &item);

    // vtb+119
    virtual bool drop(const ItemStack &item, const bool randomly);

    // vtb+120
    virtual bool getInteraction(Player &player, ActorInteraction &interaction, const Vec3 &location);

    // vtb+121
    virtual bool canDestroyBlock(const Block &block) const;

    // vtb+122
    virtual void setAuxValue(int val);

#if MC_VERSION <= v1_21_50
    // vtb+127
    virtual void startSpinAttack();

    // vtb+128
    virtual void stopSpinAttack();
#endif

    // vtb+123
    virtual void renderDebugServerState(const Options &options);

    // vtb+124
    virtual void kill();

    // vtb+125
    virtual void die(const ActorDamageSource &source);

    // vtb+126
    virtual bool shouldDropDeathLoot() const;

    // vtb+127
    virtual void applySnapshot(
        const EntityContext                                   &snapshotEntity,
        const MovementDataExtractionUtility::SnapshotAccessor &originalSnapshotEntity
    );

#if MC_VERSION <= v1_21_50
    // vtb+128
    virtual float getNextStep(const float moveDist);
#endif

    // vtb+129
    virtual void onPush(Actor &);

    // vtb+130
    virtual std::optional<BlockPos> getLastDeathPos() const;

    // vtb+131
    virtual std::optional<DimensionType> getLastDeathDimension() const;

    // vtb+132
    virtual bool hasDiedBefore() const;

    // vtb+133
    virtual void doEnterWaterSplashEffect();

    // vtb+134
    virtual void doExitWaterSplashEffect();

    // vtb+135
    virtual void doWaterSplashEffect();

    // vtb+136
    virtual bool _shouldProvideFeedbackOnHandContainerItemSet(HandSlot handSlot, const ItemStack &item) const;

    // vtb+137
    virtual bool _shouldProvideFeedbackOnArmorSet(ArmorSlot slot, const ItemStack &item) const;

#if MC_VERSION <= v1_21_50
    // vtb+138
    virtual bool shouldTryMakeStepSound();
#endif

    // vtb+139
    virtual bool _hurt(const ActorDamageSource &source, float damage, bool knock, bool ignite);

    // vtb+140
    virtual void readAdditionalSaveData(const CompoundTag &tag, DataLoadHelper &dataLoadHelper);

    // vtb+141
    virtual void addAdditionalSaveData(CompoundTag &tag) const;

#if MC_VERSION <= v1_21_50
    // vtb+142
    virtual void _playStepSound(const BlockPos &pos, const Block &onBlock);
#endif

#if MC_VERSION == v1_21_2
    // vtb+150
    virtual void _doAutoAttackOnTouch(Actor &entity);
#endif

    SPHR_DECL_API("1.21.2", "\x40\x53\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x8B\x41\x00\x48\x8D\x54\x24")
    SPHR_DECL_API("1.21.50,1.21.60", "\x40\x53\x48\x83\xEC\x00\x4C\x8B\x51\x00\xBB\x00\x00\x00\x00\x8B\x51")
    SDK_API const ActorUniqueID &getOrCreateUniqueID() const;

    template <typename T>
    T *tryGetComponent() {
        return mEntityContext.mEnTTRegistry.template try_get<T>(mEntityContext.mEntity);
    }

    template <typename T>
    const T *tryGetComponent() const {
        return mEntityContext.mEnTTRegistry.template try_get<T>(mEntityContext.mEntity);
    }
};