#pragma once

#include "SDK/api/src-deps/Core/Utility/SemVersion.h"

// size: 112 (1.21.2/1.21.50), 32 (1.21.60)
class BaseGameVersion {
public:
    SemVersion mSemVersion;      // off+0
    bool       mNeverCompatible; // off+112, 24 (1.21.60)
};