#pragma once

#include "../Event.h"

#include "SDK/api/src-deps/Application/AppPlatform.h"

class AppTerminateEvent : public Event {
    AppPlatform &mPlatform;

public:
    AppTerminateEvent(AppPlatform &platform) :
        mPlatform(platform) {}
};