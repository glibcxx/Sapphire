#include "EventAtAppPlatformListener.h"
#include "SDK/api/sapphire/event/EventBus.h"
#include "SDK/api/sapphire/event/events/AppTerminateEvent.h"

namespace sapphire::event::impl {

    SapphireEventAppPlatformListener::SapphireEventAppPlatformListener() :
        mPlatform(ServiceLocator<AppPlatform>::get().mService) {
        if (!this->mListenerRegistered && mPlatform) {
            mPlatform->addListener(this, 0.0f);
            this->mListenerRegistered = true;
        }
    }

    void SapphireEventAppPlatformListener::onAppTerminated() {
        EventBus::getInstance().dispatchEvent(AppTerminateEvent{*mPlatform});
    }

} // namespace sapphire::event::impl