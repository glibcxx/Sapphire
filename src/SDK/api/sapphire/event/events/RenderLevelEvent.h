#pragma once

#include "../Event.h"

#include "SDK/api/src-client/common/client/renderer/game/LevelRenderer.h"

class RenderLevelEvent : public Event {
public:
    ScreenContext *mScreen;
    LevelRenderer *mLevelRenderer;

    RenderLevelEvent(LevelRenderer *levelRenderer, ScreenContext &screen) :
        mLevelRenderer(levelRenderer), mScreen(&screen) {}
};