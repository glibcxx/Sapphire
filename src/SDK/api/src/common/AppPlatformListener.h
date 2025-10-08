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
    SPHR_API virtual ~AppPlatformListener();

#if MC_VERSION < v1_21_60
    virtual void onLowMemory() {}
#endif

    virtual void onAppPaused() {}

    virtual void onAppUnpaused() {}

    virtual void onAppPreSuspended() {}

    virtual void onAppSuspended() {}

    virtual void onAppResumed() {}

    virtual void onAppFocusLost() {}

    virtual void onAppFocusGained() {}

    virtual void onAppTerminated() {}

    virtual void onOperationModeChanged(OperationMode operationMode) {}

    virtual void onPerformanceModeChanged(const bool boost) {}

    virtual void onPushNotificationReceived(const PushNotificationMessage &msg) {}

    virtual void onResizeBegin() {}

    virtual void onResizeEnd() {}

    virtual void onDeviceLost() {}

    virtual void onAppSurfaceCreated() {}

    virtual void onAppSurfaceDestroyed() {}

    virtual void onClipboardCopy(const std::string &msg) {}

    virtual void onClipboardPaste(const std::string &msg) {}

#if MC_VERSION >= v1_21_60
    virtual void onLowMemory() {}
#endif
};