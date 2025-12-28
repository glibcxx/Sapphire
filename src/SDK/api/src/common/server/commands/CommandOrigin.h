#pragma once

#include "CommandPermissionLevel.h"
#include "CommandOriginData.h"
#include "SDK/api/src/common/CommonTypes.h"
#include "SDK/api/src/common/nbt/CompoundTag.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src/common/world/actor/player/Abilities.h"

class NetworkIdentifier;
class Level;
class Dimension;
class Actor;
class CommandPositionFloat;
struct CommandOriginIdentity;

// size: 24
class CommandOrigin {
public:
    mce::UUID mUUID; // off+8

    CommandOrigin();

    // vtb+0
    virtual ~CommandOrigin() = default;

    // vtb+1
    virtual const std::string &getRequestId() const = 0;

    // vtb+2
    virtual std::string getName() const = 0;

    // vtb+3
    virtual BlockPos getBlockPosition() const = 0;

    // vtb+4
    virtual Vec3 getWorldPosition() const = 0;

    // vtb+5
    virtual std::optional<Vec2> getRotation() const = 0;

    // vtb+6
    virtual Level *getLevel() const = 0;

    // vtb+7
    virtual Dimension *getDimension() const = 0;

    // vtb+8
    virtual Actor *getEntity() const = 0;

    // vtb+9
    virtual CommandPermissionLevel getPermissionsLevel() const = 0;

    // vtb+10
    virtual std::unique_ptr<CommandOrigin> clone() const = 0;

    // vtb+11
    virtual std::optional<BlockPos> getCursorHitBlockPos() const;

    // vtb+12
    virtual std::optional<Vec3> getCursorHitPos() const;

    // vtb+13
    virtual bool hasChatPerms() const;

    // vtb+14
    virtual bool hasTellPerms() const;

    // vtb+15
    virtual bool canUseAbility(AbilitiesIndex ability) const;

    // vtb+16
    virtual bool isWorldBuilder() const;

    // vtb+17
    virtual bool canUseCommandsWithoutCheatsEnabled() const;

    // vtb+18
    virtual bool isSelectorExpansionAllowed() const;

    // vtb+19
    virtual const NetworkIdentifier &getSourceId() const;

    // vtb+20
    virtual SubClientId getSourceSubId() const;

    // vtb+21
    virtual CommandOrigin const &getOutputReceiver() const;

    // vtb+22
    virtual CommandOriginIdentity getIdentity() const;

    // vtb+23
    virtual CommandOriginType getOriginType() const = 0;

    // vtb+24
    virtual CommandOriginData toCommandOriginData() const;

    // vtb+25
    virtual const mce::UUID &getUUID() const;

    // vtb+26
    virtual void handleCommandOutputCallback(int, std::string &&) const;

    // vtb+27
    virtual void updateValues();

    // vtb+28
    virtual const Vec3 getExecutePosition(int version, const CommandPositionFloat &commandPosition) const;

    // vtb+29
    virtual CompoundTag serialize() const;

    // vtb+30
    virtual bool isValid() const = 0;

#if MC_VERSION >= v1_21_50
    // vtb+31
    virtual bool requiresValidLevel() const;
#endif

    // vtb+32
    virtual void _setUUID(::mce::UUID const &uuid);
};