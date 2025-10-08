#pragma once

#include "SDK/core/ApiManager.h"
#include <functional>

class Timer {
public:
    float mTicksPerSecond = 0.0f;
    int   mTicks = 0;
    float mAlpha = 0;
    float mTimeScale = 1.0f;
    float mPassedTime = 0.0f;
    float mFrameStepAlignmentRemainder = 0.0f;
    float mLastTimeSeconds;
    float mLastTimestep = 0.0f;
#if MC_VERSION >= v1_21_60
    float mOverflowTime;
#endif
    int64_t                  mLastMs;
    int64_t                  mLastMsSysTime;
    float                    mAdjustTime = 1.0f;
    int                      mSteppingTick = -1;
    std::function<int64_t()> mGetTimeMSCallback;

    SDK_API void advanceTime(float preferredFrameStep);
};