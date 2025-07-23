#pragma once

#ifndef SPPHR_INCLUDE_ENTITYCONTEXT_H
#    define SPPHR_INCLUDE_ENTITYCONTEXT_H
#endif

#include <entt/entt.hpp>
#include "WeakStorageEntity.h"

class EntityRegistry;
class OwnerStorageEntity;

// size: 24
class EntityContext {
public:
    EntityRegistry                 &mRegistry;     // off+0
    entt::basic_registry<EntityId> &mEnTTRegistry; // off+8
    const EntityId                  mEntity;       // off+16
};

#include "OwnerStorageEntity.h"
#include "StackResultStorageEntity.h"

template <>
struct GameRefs<EntityContext> {
    using OwnerStorage = OwnerStorageEntity;
    using StackResultStorage = StackResultStorageEntity;
    using WeakStorage = WeakStorageEntity;
    using StackRef = EntityContext;
};
