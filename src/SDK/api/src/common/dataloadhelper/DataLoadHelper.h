#pragma once

#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src/common/world/Direction.h"
#include "SDK/api/src/common/util/RotationEnum.h"
#include "SDK/api/src/common/util/MirrorEnum.h"
#include "SDK/api/src/common/ActorUniqueID.h"
#include "SDK/api/src/common/world/actor/InternalComponentRegistry.h"

class HashedString;

enum class DataLoadHelperType : int {
    Default = 0,
    Structure = 1,
};

// size: 8
class DataLoadHelper {
public:
    // vtb+0
    virtual ~DataLoadHelper() = default;

    // vtb+1
    virtual Vec3 loadPosition(const Vec3 &) = 0;

    // vtb+2
    virtual BlockPos loadBlockPosition(const BlockPos &) = 0;

    // vtb+3
    virtual BlockPos loadBlockPositionOffset(const BlockPos &) = 0;

    // vtb+4
    virtual float loadRotationDegreesX(float) = 0;

    // vtb+5
    virtual float loadRotationDegreesY(float) = 0;

    // vtb+6
    virtual float loadRotationRadiansX(float) = 0;

    // vtb+7
    virtual float loadRotationRadiansY(float) = 0;

    // vtb+8
    virtual FacingID loadFacingID(FacingID) = 0;

    // vtb+9
    virtual Direction::Type loadDirection(Direction::Type) = 0;

    // vtb+10
    virtual Vec3 loadDirection(const Vec3 &) = 0;

    // vtb+11
    virtual Rotation loadRotation(Rotation) = 0;

    // vtb+12
    virtual Mirror loadMirror(Mirror) = 0;

    // vtb+13
    virtual ActorUniqueID loadActorUniqueID(ActorUniqueID) = 0;

    // vtb+14
    virtual ActorUniqueID loadOwnerID(ActorUniqueID) = 0;

    // vtb+15
    virtual DataLoadHelperType getType() const = 0;

    // vtb+16
    virtual const InternalComponentRegistry::ComponentInfo *loadActorInternalComponentInfo(
        const std::unordered_map<HashedString, InternalComponentRegistry::ComponentInfo> &,
        const std::string &
    ) = 0;

    // vtb+17
    virtual bool shouldResetTime() = 0;
};