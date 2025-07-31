#pragma once

#include "SDK/api/src-deps/Application/AppPlatform.h"
#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"

// size: 1784 (1.21.2), 1792 (1.21.50/1.21.60)
class AppPlatformWindows : public AppPlatform {
public:
    ;                                                         // offset: (1.21.50/1.21.60), (1.21.2 = 1.21.50 - 8)
    bool                             mGraphicsTearingSupport; // off+1040
    Core::HeapPathBuffer             mExternalStoragePath;    // off+1048
    Core::HeapPathBuffer             mPlatformTempPath;       // off+1080
    Core::HeapPathBuffer             mInternalStoragePath;    // off+1112
    Core::HeapPathBuffer             mHomePath;               // off+1144
    MPMCQueue<std::function<void()>> mMainThreadQueue;        // off+1176
};
#if MC_VERSION == v1_21_2
static_assert(sizeof(AppPlatformWindows) == 1784);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
static_assert(sizeof(AppPlatformWindows) == 1792);
#endif
