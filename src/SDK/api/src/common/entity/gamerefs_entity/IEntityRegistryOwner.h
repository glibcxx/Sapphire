#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src/common/gamerefs/StackRefResult.h"

class EntityRegistry;

// size: 24
class IEntityRegistryOwner : public Bedrock::EnableNonOwnerReferences {
public:
    // vtb+0
    virtual ~IEntityRegistryOwner() = default;

    // vtb+1
    virtual StackRefResult<const EntityRegistry> getEntityRegistry() const = 0;

    // vtb+2
    virtual StackRefResult<EntityRegistry> getEntityRegistry() = 0;
};