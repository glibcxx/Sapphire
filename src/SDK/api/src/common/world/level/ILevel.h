#pragma once

#include <string>
#include <functional>
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src/common/world/level/dimension/DimensionType.h"
#include "SDK/api/src/common/gamerefs/WeakRef.h"

class LevelSettings;
class Experiments;

// size: 24, incomplete
class ILevel : public Bedrock::EnableNonOwnerReferences {
public:
    // vtb+0
    virtual ~ILevel();

    // vtb+1
    virtual bool initialize(
        const std::string   &levelName,
        const LevelSettings &levelSettings,
        class LevelData     *levelData,
        const Experiments   &experiments,
        const std::string   *levelId
    ) = 0;

    // vtb+2
    virtual void startLeaveGame() = 0;

    // vtb+3
    virtual bool isLeaveGameDone() = 0;

    // vtb+4
    virtual WeakRef<Dimension> getOrCreateDimension(DimensionType type) = 0;

    // vtb+5
    virtual WeakRef<Dimension> getDimension(DimensionType type) const = 0;

    // vtb+6
    virtual DimensionType getLastOrDefaultSpawnDimensionId(DimensionType type) const = 0;

    // vtb+7
    virtual void forEachDimension(std::function<bool(const Dimension &)> callback) const = 0;

    // vtb+8
    virtual void forEachDimension(std::function<bool(Dimension &)> callback) = 0;

    // ...
};