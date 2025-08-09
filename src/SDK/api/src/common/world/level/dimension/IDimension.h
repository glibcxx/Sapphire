#pragma once

#include "macros/Macros.h"
#include "DimensionType.h"
#include "SDK/api/src/common/world/level/BlockPos.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src/common/ActorUniqueID.h"

class Packet;
class Actor;
class Player;
class BiomeRegistry;
class LevelStorageManager;

// size: 8
class IDimension {
public:
    // vtb+0
    ~IDimension() = default;

    // vtb+1
    virtual bool isNaturalDimension() const = 0;

    // vtb+2
    virtual DimensionType getDimensionId() const = 0;

    // vtb+3
    virtual void sendPacketForPosition(const BlockPos &position, const Packet &packet, const Player *except) = 0;

#if MC_VERION == v1_21_50 || MC_VERION == v1_21_60
    // vtb+4
    virtual void sendPacketForEntity(const Actor &entity, const Packet &packet, const Player *except) = 0;
#endif

    // vtb+5
    virtual void flushLevelChunkGarbageCollector() = 0;

    // vtb+6
    virtual void initializeWithLevelStorageManager(LevelStorageManager &mgr) = 0;

    // vtb+7
    virtual BiomeRegistry &getBiomeRegistry() = 0;

    // vtb+8
    virtual const BiomeRegistry &getBiomeRegistry() const = 0;

    // vtb+9
    virtual Vec3 translatePosAcrossDimension(const Vec3 &pos, DimensionType type) const = 0;

    // vtb+10
    virtual void forEachPlayer(std::function<bool(Player &)> callback) const = 0;

    // vtb+11
    virtual Actor *fetchEntity(ActorUniqueID actorID, bool getRemoved) const = 0;
};