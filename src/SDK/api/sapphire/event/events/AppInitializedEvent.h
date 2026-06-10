#pragma once

#include "pch.h" // IWYU pragma: keep

#include "../Event.h"

#include "SDK/api/src-deps/Application/AppPlatform.h"

namespace sapphire::event {

    class AppInitializedEvent : public Event {
    public:
        AppPlatform &mPlatform;

        AppInitializedEvent(AppPlatform &platform) :
            mPlatform(platform) {}
    };

} // namespace sapphire::event