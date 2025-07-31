#pragma once

#include "SDK/api/src/common/util/Timer.h"

class CamTimeline {
    size_t mCurrentTicks = 0;
    float  mFrameAlpha = 0.0f;

public:
    void reset();

    void setTick(size_t tick);

    void update(const Timer &timer);

    size_t getTick() const { return this->mCurrentTicks; }

    float getAlpha() const { return this->mFrameAlpha; }
};