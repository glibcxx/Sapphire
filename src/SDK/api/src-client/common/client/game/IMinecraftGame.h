#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

class IMinecraftGame : public Bedrock::EnableNonOwnerReferences /*TODO: other baseclass*/ {
public:
    // vtb+0
    virtual ~IMinecraftGame();
};