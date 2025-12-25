#pragma once

#include "ActorCategory.h"
#include "ActorTypes.h"
#include "SDK/api/src/common/ActorUniqueID.h"

class Actor;

// https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/world/actor/ActorDamageSource.h#L18

enum class ActorDamageCause : int {
    None = -1,
    Override = 0,
    Contact = 1,
    EntityAttack = 2,
    Projectile = 3,
    Suffocation = 4,
    Fall = 5,
    Fire = 6,
    FireTick = 7,
    Lava = 8,
    Drowning = 9,
    BlockExplosion = 10,
    EntityExplosion = 11,
    Void = 12,
    SelfDestruct = 13,
    Magic = 14,
    Wither = 15,
    Starve = 16,
    Anvil = 17,
    Thorns = 18,
    FallingBlock = 19,
    Piston = 20,
    FlyIntoWall = 21,
    Magma = 22,
    Fireworks = 23,
    Lightning = 24,
    Charging = 25,
    Temperature = 26,
    Freezing = 27,
    Stalactite = 28,
    Stalagmite = 29,
    RamAttack = 30,
    SonicBoom = 31,
    Campfire = 32,
    SoulCampfire = 33,
    MaceSmash = 34,
    All = 35,
};

// size: 16
class ActorDamageSource {
public:
    ActorDamageCause mCause; // off+8

    // vtb+0
    virtual ~ActorDamageSource();

    // vtb+1
    virtual bool isEntitySource() const;

    // vtb+2
    virtual bool isChildEntitySource() const;

    // vtb+3
    virtual bool isBlockSource() const;

    // vtb+4
    virtual bool isFire() const;

    // vtb+5
    virtual bool isReducedByResistanceEffect() const;

    // vtb+6
    virtual bool isReducedByEnchantReduction() const;

    // vtb+7
    virtual bool isReducedByArmorReduction() const;

    // vtb+8
    virtual bool isFallingBlockDamage() const;

    // vtb+9
    virtual bool isFallDamage() const;

    // vtb+10
    virtual std::pair<std::string, std::vector<std::string>> getDeathMessage(std::string deadName, Actor *dead) const;

    // vtb+11
    virtual bool getIsCreative() const;

    // vtb+12
    virtual bool getIsWorldBuilder() const;

    // vtb+13
    virtual ActorUniqueID getEntityUniqueID() const;

    // vtb+14
    virtual ActorType getEntityType() const;

    // vtb+15
    virtual ActorCategory getEntityCategories() const;

    // vtb+16
    virtual bool getDamagingEntityIsCreative() const;

    // vtb+17
    virtual bool getDamagingEntityIsWorldBuilder() const;

    // vtb+18
    virtual ActorUniqueID getDamagingEntityUniqueID() const;

    // vtb+19
    virtual ActorType getDamagingEntityType() const;

    // vtb+20
    virtual ActorCategory getDamagingEntityCategories() const;

    // vtb+21
    virtual std::unique_ptr<ActorDamageSource> clone() const;
};