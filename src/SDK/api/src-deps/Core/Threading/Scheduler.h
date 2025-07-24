#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

class WorkerPool;

// size: 104
class Scheduler : public Bedrock::EnableNonOwnerReferences {
public:
    uint32_t                              mTotalFrames;             // off+24
    uint32_t                              mStarvedFrames;           // off+28
    uint32_t                              mPromotionFrames;         // off+32
    uint32_t                              mTargetFPS;               // off+36
    uint32_t                              mEffectiveFPS;            // off+40
    uint32_t                              mFramesOverBound;         // off+44
    double                                mAverageCallbackDuration; // off+48
    double                                mTotalCoroutineDuration;  // off+56
    uint64_t                              mTotalRunCoroutines;      // off+64
    double                                mCoroutineTimeLimit;      // off+72
    std::unique_ptr<WorkerPool>           mCoroutinePool;           // off+80
    std::chrono::steady_clock::time_point mNextStarveCheckTime;     // off+88
    std::thread::id                       mThreadId;                // off+96

    SDK_API Scheduler *ctor(std::string name, uint32_t FPS);
    MARK_HOOKABLE(&Scheduler::ctor)
};
static_assert(sizeof(Scheduler) == 104);