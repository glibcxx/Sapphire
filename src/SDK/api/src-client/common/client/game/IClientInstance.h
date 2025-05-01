#pragma once

#include "SDK/core/Core.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

class IClientInstance : public Bedrock::EnableNonOwnerReferences {
public:
    virtual ~IClientInstance() = 0;

    SDK_API static void *const *__vftable;
};