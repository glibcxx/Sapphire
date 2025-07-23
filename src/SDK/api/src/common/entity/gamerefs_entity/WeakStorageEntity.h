#pragma once

#include <optional>
#include "SDK/api/src/common/gamerefs/WeakRef.h"
#include "../EntityId.h"

class EntityRegistry;

// size: 24
class WeakStorageEntity {
public:
    enum class EmptyInit : int {
        NoValue = 0,
    };

    enum class VariadicInit : int {
        NonAmbiguous = 0,
    };

    WeakRef<EntityRegistry> mRegistry; // off+0
    std::optional<EntityId> mEntity;   // off+16
};