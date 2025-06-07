#pragma once

#include "../Event.h"

#include "SDK/api/src-deps/Application/AppPlatform.h"

class AppInitializedEvent : public Event {
    AppPlatform &mPlatform;

public:
    AppInitializedEvent(AppPlatform &platform) :
        mPlatform(platform) {}
};