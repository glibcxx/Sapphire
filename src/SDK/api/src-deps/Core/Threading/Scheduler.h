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

    SDK_API Scheduler(std::string name, uint32_t FPS);

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xF0\x48\x89\x37\x48\x8B\xC7\x4C\x8D\x5C\x24\x00\x49\x8B\x5B\x00\x49\x8B\x73")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xC4\x44\x8B\x65")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\xEB\x00\x48\x8B\xC6\x8B\x75")
    SPHR_CTOR_ALIAS SDK_API Scheduler *ctor(std::string name, uint32_t FPS);
};
static_assert(sizeof(Scheduler) == 104);