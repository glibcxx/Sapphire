#pragma once

#include "Async.h"

// size: 40
class TaskResult {
public:
    bool                                           mIsDone;            // off+0
    std::chrono::steady_clock::time_point          mRunAtTime;         // off+8
    Bedrock::Threading::IAsyncResult<void>::Handle mWaitOperation;     // off+16
    bool                                           mLinkWaitOperation; // off+32
};