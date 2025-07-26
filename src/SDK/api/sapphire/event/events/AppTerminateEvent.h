#pragma once

#include "../Event.h"

#include "SDK/api/src-deps/Application/AppPlatform.h"

namespace sapphire::event {

    class AppTerminateEvent : public Event {
    public:
        AppPlatform &mPlatform;

        AppTerminateEvent(AppPlatform &platform) :
            mPlatform(platform) {}
    };

} // namespace sapphire::event