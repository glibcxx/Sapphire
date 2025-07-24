#pragma once

#include "../Event.h"

#include "SDK/api/src-deps/Application/AppPlatform.h"

namespace sapphire::event {

    class AppInitializedEvent : public Event {
        AppPlatform &mPlatform;

    public:
        AppInitializedEvent(AppPlatform &platform) :
            mPlatform(platform) {}
    };

} // namespace sapphire::event