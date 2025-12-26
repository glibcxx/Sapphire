#pragma once

#include "Actor.h"
#include "SDK/api/src/common/world/item/EquipmentSlot.h"

class BodyControl;

class Mob : public Actor {
public:
    virtual void knockback(
        Actor      *source,
        int         dmg,
        const float xd,
        const float zd,
        float       horizontalPower,
        const float verticalPower,
        const float heightCap
    );

    virtual void spawnAnim();

    virtual void setSprinting(bool shouldSprint);

    virtual SharedTypes::Legacy::LevelSoundEvent getDeathSound();

    virtual float getSpeed() const;

    virtual void setSpeed(float speed);

    virtual void hurtEffects(const ActorDamageSource &source, float damage, bool knock, bool ignite);

    virtual void aiStep();

    virtual void pushActors();

    virtual bool checkSpawnRules(bool fromSpawner);

    virtual bool checkSpawnObstruction() const;

    virtual int getItemUseDuration() const;

    virtual float getItemUseStartupProgress() const;

    virtual float getItemUseIntervalProgress() const;

    virtual float getMaxHeadXRot();

    virtual bool isAlliedTo(Mob *other) const;

    virtual bool doHurtTarget(Actor *target, const ActorDamageCause &cause);

    virtual int getArmorValue();

    virtual void hurtArmorSlots(const ActorDamageSource &source, int dmg, const std::bitset<4> hurtSlots);

    virtual void setDamagedArmor(ArmorSlot slot, const ItemStack &item);

    virtual void sendArmorDamage(const std::bitset<4> damagedSlots);

    virtual void sendArmor(const std::bitset<4> armorSlots);

    virtual std::vector<const ItemStack *> getAllHand() const;

    virtual std::vector<const ItemStack *> getAllEquipment() const;

    virtual void dropEquipmentOnDeath();

    virtual void dropEquipmentOnDeath(const ActorDamageSource &source, int);

    virtual void clearVanishEnchantedItemsOnDeath();

    virtual void sendInventory(bool shouldSelectSlot);

    virtual float getDamageAfterEnchantReduction(const ActorDamageSource &source, float dmg) const;

    virtual bool createAIGoals();

    virtual void onBorn(Actor &parentLeft, Actor &parentRight);

    virtual bool setItemSlot(SharedTypes::Legacy::EquipmentSlot slot, const ItemStack &item);

    virtual void setTransitioningSitting(bool value);

    virtual float _getWalkTargetValue(const BlockPos &pos);

    virtual bool canExistWhenDisallowMob();

#if MC_VERSION >= v1_21_60
    virtual void tickDeath();
#endif

    virtual std::unique_ptr<BodyControl> initBodyControl();

    virtual void newServerAiStep();

#if MC_VERSION <= v1_21_50
    virtual void tickDeath();
#endif

    virtual AABB _getAdjustedAABBForSpawnCheck(const AABB &aabb, const Vec3 &) const;
};