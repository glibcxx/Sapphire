#pragma once

#ifndef SPPHR_INCLUDE_ENTITYCONTEXT_H
#    error "do not include StackResultStorageEntity.h directly, please include EntityContext.h"
#endif

#include "SDK/api/src/common/entity/gamerefs_entity/EntityContext.h"

// size: 32
class StackResultStorageEntity {
public:
    std::optional<EntityContext> mContext; // off+0
};