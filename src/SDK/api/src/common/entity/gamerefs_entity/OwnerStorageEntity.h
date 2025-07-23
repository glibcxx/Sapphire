#pragma once

#ifndef SPPHR_INCLUDE_ENTITYCONTEXT_H
#    error "Do not include WeakStorageEntity.h directly, please include EntityContext.h"
#endif

#include "SDK/api/src/common/entity/gamerefs_entity/EntityContext.h"
#include "SDK/api/src/common/gamerefs/GameRefs.h"

// size: 32
class OwnerStorageEntity {
public:
    enum class EmptyInit : int {
        NoValue = 0,
    };

    enum class VariadicInit : int {
        NonAmbiguous = 0,
    };

    // size: 24
    class EntityContextOwned
        : public EntityContext {
        using Base = EntityContext;
    };

    std::optional<EntityContextOwned> mContext; // off+0
};
