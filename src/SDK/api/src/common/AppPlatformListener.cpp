#include "AppPlatformListener.h"
#include "SDK/api/src-deps/Application/AppPlatform.h"

AppPlatformListener::~AppPlatformListener() {
    ServiceLocator<AppPlatform>::get().mService->removeListener(this);
}