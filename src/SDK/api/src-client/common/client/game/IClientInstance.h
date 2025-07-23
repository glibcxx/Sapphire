#pragma once

#include "SDK/core/ApiManager.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

// size: 24
class IClientInstance : public Bedrock::EnableNonOwnerReferences {
public:
    virtual ~IClientInstance() = 0;

    SDK_API static void *const *__vftable;
};