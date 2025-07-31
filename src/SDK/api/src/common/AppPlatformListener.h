#pragma once

#include "SDK/api/src/common/platform/PushNotificationMessage.h"
#include "SDK/api/src-deps/Core/Platform/PlatformTypes.h"
#include "SDK/api/src-deps/Core/Utility/pubsub/Subscription.h"

#include "SDK/core/ApiManager.h"

// size: 16, 32 (1.21.60+)
class AppPlatformListener {
public:
#if MC_VERSION >= v1_21_60
    Bedrock::PubSub::Subscription mLowMemorySubscription;      // off+8
    bool                          mListenerRegistered = false; // off+24
#else
    bool mListenerRegistered = false; // off+8
#endif

    // vtb+0
    SDK_API virtual ~AppPlatformListener();

    // vtb+1
    virtual void onLowMemory() {}

    // vtb+2
    virtual void onAppPaused() {}

    // vtb+3
    virtual void onAppUnpaused() {}

    // vtb+4
    virtual void onAppPreSuspended() {}

    // vtb+5
    virtual void onAppSuspended() {}

    // vtb+6
    virtual void onAppResumed() {}

    // vtb+7
    virtual void onAppFocusLost() {}

    // vtb+8
    virtual void onAppFocusGained() {}

    // vtb+9
    virtual void onAppTerminated() {}

    // vtb+10
    virtual void onOperationModeChanged(OperationMode operationMode) {}

    // vtb+11
    virtual void onPerformanceModeChanged(const bool boost) {}

    // vtb+12
    virtual void onPushNotificationReceived(const PushNotificationMessage &msg) {}

    // vtb+13
    virtual void onResizeBegin() {}

    // vtb+14
    virtual void onResizeEnd() {}

    // vtb+15
    virtual void onDeviceLost() {}

    // vtb+16
    virtual void onAppSurfaceCreated() {}

    // vtb+17
    virtual void onAppSurfaceDestroyed() {}

    // vtb+18
    virtual void onClipboardCopy(const std::string &msg) {}

    // vtb+19
    virtual void onClipboardPaste(const std::string &msg) {}
};