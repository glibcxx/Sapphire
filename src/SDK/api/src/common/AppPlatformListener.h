#pragma once

#include "SDK/api/src/common/platform/PushNotificationMessage.h"

enum class OperationMode : int {
    Handheld = 0,
    Console = 1,
}; // OperationMode

// size: 16
class AppPlatformListener {
public:
    bool mListenerRegistered; // off+8

    // vtb+0
    virtual ~AppPlatformListener() {}

    // vtb+1
    virtual void onLowMemory() = 0;

    // vtb+2
    virtual void onAppPaused() = 0;

    // vtb+3
    virtual void onAppUnpaused() = 0;

    // vtb+4
    virtual void onAppPreSuspended() = 0;

    // vtb+5
    virtual void onAppSuspended() = 0;

    // vtb+6
    virtual void onAppResumed() = 0;

    // vtb+7
    virtual void onAppFocusLost() = 0;

    // vtb+8
    virtual void onAppFocusGained() = 0;

    // vtb+9
    virtual void onAppTerminated() = 0;

    // vtb+10
    virtual void onOperationModeChanged(OperationMode operationMode) = 0;

    // vtb+11
    virtual void onPerformanceModeChanged(const bool boost) = 0;

    // vtb+12
    virtual void onPushNotificationReceived(const PushNotificationMessage &msg) = 0;

    // vtb+13
    virtual void onResizeBegin() = 0;

    // vtb+14
    virtual void onResizeEnd() = 0;

    // vtb+15
    virtual void onDeviceLost() = 0;

    // vtb+16
    virtual void onAppSurfaceCreated() = 0;

    // vtb+17
    virtual void onAppSurfaceDestroyed() = 0;

    // vtb+18
    virtual void onClipboardCopy(const std::string &msg) = 0;

    // vtb+19
    virtual void onClipboardPaste(const std::string &msg) = 0;
};