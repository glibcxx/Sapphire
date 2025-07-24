#include "EventAtAppPlatformListener.h"
#include "SDK/api/sapphire/event/EventManager.h"
#include "SDK/api/sapphire/event/events/AppTerminateEvent.h"

namespace sapphire::event {

    SapphireEventAppPlatformListener::SapphireEventAppPlatformListener() :
        mPlatform(ServiceLocator<AppPlatform>::get().mService) {
        if (mPlatform) {
            mPlatform->addListener(this, 0.0f);
        }
    }

    void SapphireEventAppPlatformListener::onAppTerminated() {
        EventManager::getInstance().dispatchEvent(AppTerminateEvent{*mPlatform});
    }

} // namespace sapphire::event