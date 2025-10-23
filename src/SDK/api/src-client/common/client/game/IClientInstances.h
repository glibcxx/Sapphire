#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class IClientInstance;
class ItemRegistryRef;

// size: 8
class IClientInstances {
public:
    // vtb+0
    virtual ~IClientInstances() = default;

    // vtb+2
    virtual Bedrock::NotNullNonOwnerPtr<IClientInstance> getPrimaryClientInstance() = 0;

    // vtb+1
    virtual Bedrock::NotNullNonOwnerPtr<const IClientInstance> getPrimaryClientInstance() const = 0;

    // vtb+3
    virtual ItemRegistryRef getClientItemRegistry() const = 0;
};
