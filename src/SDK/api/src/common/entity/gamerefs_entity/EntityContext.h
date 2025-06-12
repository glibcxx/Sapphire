#pragma once

#include <entt/entt.hpp>
#include "../EntityId.h"

class EntityRegistry;

// size: 24
class EntityContext {
public:
    EntityRegistry                 &mRegistry;     // off+0
    entt::basic_registry<EntityId> &mEnTTRegistry; // off+8, entt::basic_registry<EntityId> &
    const EntityId                  mEntity;       // off+16
};