#pragma once

#include "EventResult.h"
#include "MinecraftEventing.h"
#include "SDK/api/src/common/world/level/dimension/DimensionType.h"
#include "SDK/api/src/common/world/level/BlockPos.h"
#include "SDK/api/src/common/world/phys/Vec2.h"
#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src-client/common/client/game/IClientInstance.h"

class Actor;
class Block;
class EntityContext;
class PlayerNotificationEvent;
class ItemDescriptor;
class ItemStack;
class ItemEnchants;
class ItemInstance;
class LocalPlayer;
class Player;
enum class PlayerActionType : int;

class PlayerEventListener {
public:
    // vtb+0
    virtual ~PlayerEventListener() {}

    // vtb+1
    virtual EventResult onPlayerAwardAchievement(Player &player, MinecraftEventing::AchievementIds achievement) { return EventResult::KeepGoing; }

    // vtb+2
    virtual EventResult onPlayerPortalBuilt(Player &player, DimensionType dimensionBuiltIn) { return EventResult::KeepGoing; }

    // vtb+3
    virtual EventResult onPlayerPortalUsed(Player &player, DimensionType fromDimension, DimensionType toDimension) { return EventResult::KeepGoing; }

    // vtb+4
    virtual EventResult onPlayerPoweredBeacon(const Player &player, int level) { return EventResult::KeepGoing; }

    // vtb+5
    virtual EventResult onPlayerCaravanChanged(const Actor &mob, int caravanCount) { return EventResult::KeepGoing; }

    // vtb+6
    virtual EventResult onPlayerSaved(Player &player) { return EventResult::KeepGoing; }

    // vtb+7
    virtual EventResult onPlayerInput(EntityContext &player) { return EventResult::KeepGoing; }

    // vtb+8
    virtual EventResult onPlayerAuthInputReceived(Player &player) { return EventResult::KeepGoing; }

    // vtb+9
    virtual EventResult onPlayerAuthInputApplied(Player &player) { return EventResult::KeepGoing; }

    // vtb+10
    virtual EventResult onPlayerTurn(Player &player, Vec2 &turnDelta) { return EventResult::KeepGoing; }

    // vtb+11
    virtual EventResult onCameraSetPlayerRot(Player &, const Vec2 &) { return EventResult::KeepGoing; }

    // vtb+12
    virtual EventResult onStartDestroyBlock(Player &player, const BlockPos &pos, FacingID &face) { return EventResult::KeepGoing; }

    // vtb+13
    virtual EventResult onPlayerAction(Player &player, PlayerActionType type, const BlockPos &pos, int data) { return EventResult::KeepGoing; }

    // vtb+14
    virtual EventResult onLocalPlayerDeath(IClientInstance &client, LocalPlayer &player) { return EventResult::KeepGoing; }

    // vtb+15
    virtual EventResult onLocalPlayerRespawn(IClientInstance &client, LocalPlayer &player) { return EventResult::KeepGoing; }

    // vtb+16
    virtual EventResult onPlayerMove(Player &player) { return EventResult::KeepGoing; }

    // vtb+17
    virtual EventResult onPlayerSlide(Player &player) { return EventResult::KeepGoing; }

    // vtb+18
    virtual EventResult onPlayerTargetBlockHit(Player &player, const int signalStrength) { return EventResult::KeepGoing; }

    // vtb+19
    virtual EventResult onPlayerTick(Player &player) { return EventResult::KeepGoing; }

    // vtb+20
    virtual EventResult onPlayerStartRiding(Player &player, Actor &vehicle) { return EventResult::KeepGoing; }

    // vtb+21
    virtual EventResult onPlayerStopRiding(
        Player &player, bool exitFromPassenger, bool entityIsBeingDestroyed, bool switchingVehicles
    ) { return EventResult::KeepGoing; }

    // vtb+22
    virtual EventResult onPlayerCreated(
        LocalPlayer       &player,
        const std::string &personaSlot,
        const std::string &classicSkinId,
        bool               usingClassicSkin
    ) { return EventResult::KeepGoing; }

    // vtb+23
    virtual EventResult onPlayerTeleported(Player &player, float a) { return EventResult::KeepGoing; }

    // vtb+24
    virtual EventResult onPlayerTeleported(Player &player) { return EventResult::KeepGoing; }

    // vtb+25
    virtual EventResult onPlayerAttackedActor(Player &player, Actor &target) { return EventResult::KeepGoing; }

    // vtb+26
    virtual EventResult onPlayerDestroyedBlock(Player &player, const Block &block) { return EventResult::KeepGoing; }

    // vtb+27
    virtual EventResult onPlayerDestroyedBlock(Player &player, int x, int y, int z) { return EventResult::KeepGoing; }

    // vtb+28
    virtual EventResult onPlayerEquippedArmor(Player &player, const ItemDescriptor &item) { return EventResult::KeepGoing; }

    // vtb+29
    virtual EventResult onPlayerEnchantedItem(Player &player, const ItemStack &item, const ItemEnchants &enchants) { return EventResult::KeepGoing; }

    // vtb+30
    virtual EventResult onPlayerNamedItem(Player &player, const ItemDescriptor &item) { return EventResult::KeepGoing; }

    // vtb+31
    virtual EventResult onPlayerItemUseInteraction(Player &player, ItemInstance const &itemBeforeUse) { return EventResult::KeepGoing; }

    // vtb+32
    virtual EventResult onPlayerItemPlaceInteraction(Player &player, ItemInstance const &itemBeforeUse) { return EventResult::KeepGoing; }

    // vtb+33
    virtual EventResult onPlayerCraftedItem(
        Player                   &player,
        const ItemInstance       &craftedItem,
        bool                      recipeBook,
        bool                      hadSearchString,
        bool                      craftedAutomatically,
        int                       startingTabId,
        int                       endingTabId,
        int                       numTabsChanged,
        bool                      filterOn,
        bool                      recipeBookShown,
        const std::vector<short> &ingredientItemIDs
    ) { return EventResult::KeepGoing; }

    // vtb+34
    virtual EventResult onPlayerSmithiedItem(Player &player, const ItemDescriptor &item) { return EventResult::KeepGoing; }

    // vtb+35
    virtual EventResult onPlayerItemEquipped(Player &player, const ItemInstance &equippedItem, int equipmentSlotId) { return EventResult::KeepGoing; }

    // vtb+36
    virtual EventResult onPlayerPiglinBarter(Player &player, const std::string &item, bool wasTargetingBarteringPlayer) { return EventResult::KeepGoing; }

    // vtb+37
    virtual EventResult onPlayerWaxOnWaxOff(Player &player, int blockID) { return EventResult::KeepGoing; }

    // vtb+38
    virtual EventResult onEvent(const PlayerNotificationEvent &event) { return EventResult::KeepGoing; }
};