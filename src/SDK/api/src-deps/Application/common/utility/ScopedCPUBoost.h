#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
class AppPlatform;

// size: 24 (1.21.2), 32 (1.21.50+)
class ScopedCPUBoost {
public:
    Bedrock::NonOwnerPointer<AppPlatform> mAppPlatform;     // off+0
    bool                                  mCPUBoostEnabled; // off+16
};