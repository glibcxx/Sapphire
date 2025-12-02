#pragma once

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

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x40\x53\x48\x83\xEC\x30\x48\x8B\xD9\x0F\x29\x74\x24\x20\x8B")
    SDK_API void advanceTime(float preferredFrameStep);
};