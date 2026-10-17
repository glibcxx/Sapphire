#pragma once

#include "SDK/api/src-deps/Core/Utility/UUID.h"

// size: 24
class ContentIdentity {
public:
    mce::UUID mUUID;  // off+0
    bool      mValid; // off+16
};
