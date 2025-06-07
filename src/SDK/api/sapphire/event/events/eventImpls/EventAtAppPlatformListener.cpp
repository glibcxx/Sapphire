#include "EventAtAppPlatformListener.h"
#include "SDK/api/sapphire/event/EventManager.h"
#include "SDK/api/sapphire/event/events/AppTerminateEvent.h"

SapphireEventAppPlatformListener::SapphireEventAppPlatformListener() :
    mPlatform(ServiceLocator<AppPlatform>::get().mService) {
    if (mPlatform) {
        mPlatform->addListener(this, 0.0f);
    }
}

void SapphireEventAppPlatformListener::onAppTerminated() {
    EventManager::getInstance().dispatchEvent(AppTerminateEvent{*mPlatform});
}
