#pragma once

#include "SDK/api/src-deps/Application/AppPlatform.h"

namespace sapphire::event {

    class SapphireEventAppPlatformListener : public AppPlatformListener {
        Bedrock::NonOwnerPointer<AppPlatform> mPlatform;

    public:
        SapphireEventAppPlatformListener();

        virtual ~SapphireEventAppPlatformListener() {}

        virtual void onLowMemory() {}
        virtual void onAppPaused() {}
        virtual void onAppUnpaused() {}
        virtual void onAppPreSuspended() {}
        virtual void onAppSuspended() {}
        virtual void onAppResumed() {}
        virtual void onAppFocusLost() {}
        virtual void onAppFocusGained() {}
        virtual void onAppTerminated();
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
    };

} // namespace sapphire::event