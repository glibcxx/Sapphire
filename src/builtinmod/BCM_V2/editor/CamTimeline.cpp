#include "CamTimeline.h"

void CamTimeline::reset() {
    this->mCurrentTicks = 0;
    this->mFrameAlpha = 0.0f;
}

void CamTimeline::setTick(size_t tick) {
    this->mCurrentTicks = tick;
    this->mFrameAlpha = 0.0f;
}

void CamTimeline::update(const Timer &timer) {
    this->mCurrentTicks += timer.mTicks;
    this->mFrameAlpha = timer.mAlpha;
}
