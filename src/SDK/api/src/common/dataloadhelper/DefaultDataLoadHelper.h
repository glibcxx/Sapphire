#pragma once

#include "DataLoadHelper.h"

// size: 8
class DefaultDataLoadHelper : public DataLoadHelper {
public:
    SDK_API static void *const *__vftable;

    SPHR_API DefaultDataLoadHelper();
#pragma SPHR_LINKER_SYM_ALIAS("??0DefaultDataLoadHelper@@QEAA@XZ", "?ctor@DefaultDataLoadHelper@@QEAAPEAV1@XZ")

    SPHR_API DefaultDataLoadHelper *ctor() {
        return (DefaultDataLoadHelper *)::memcpy((void *)this, &__vftable, sizeof(__vftable));
    }

    // vtb+0
    virtual ~DefaultDataLoadHelper() = default;

    // vtb+1
    virtual Vec3 loadPosition(const Vec3 &) override;

    // vtb+2
    virtual BlockPos loadBlockPosition(const BlockPos &) override;

    // vtb+3
    virtual BlockPos loadBlockPositionOffset(const BlockPos &) override;

    // vtb+4
    virtual float loadRotationDegreesX(float) override;

    // vtb+5
    virtual float loadRotationDegreesY(float) override;

    // vtb+6
    virtual float loadRotationRadiansX(float) override;

    // vtb+7
    virtual float loadRotationRadiansY(float) override;

    // vtb+8
    virtual FacingID loadFacingID(FacingID) override;

    // vtb+9
    virtual Direction::Type loadDirection(Direction::Type) override;

    // vtb+10
    virtual Vec3 loadDirection(const Vec3 &) override;

    // vtb+11
    virtual Rotation loadRotation(Rotation) override;

    // vtb+12
    virtual Mirror loadMirror(Mirror) override;

    // vtb+13
    virtual ActorUniqueID loadActorUniqueID(ActorUniqueID) override;

    // vtb+14
    virtual ActorUniqueID loadOwnerID(ActorUniqueID) override;

    // vtb+15
    virtual DataLoadHelperType getType() const override;

    // vtb+16
    virtual const InternalComponentRegistry::ComponentInfo *loadActorInternalComponentInfo(
        const std::unordered_map<HashedString, InternalComponentRegistry::ComponentInfo> &,
        const std::string &
    ) override;

    // vtb+17
    virtual bool shouldResetTime() override;
};