#pragma once

#include "../Event.h"

#include "SDK/api/src-client/common/client/gui/screen/ScreenContext.h"

class RenderLevelEvent : public Event {
public:
    ScreenContext *mScreen;

    RenderLevelEvent(ScreenContext &screen) :
        mScreen(&screen) {}
};