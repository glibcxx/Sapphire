#pragma once

#include "SDK/api/src-deps/Core/Threading/TaskStartInfo.h"

class TaskGroup;

// size: 264
class WatchdogTimer {
public:
    std::unique_ptr<TaskGroup>                             mTaskGroup;           // off+0
    TaskStartInfoEx<void>                                  mTaskStartInfo;       // off+8
    const std::chrono::seconds                             mTimeoutTimeSeconds;  // off+64
    std::function<std::chrono::steady_clock::time_point()> mTimeoutCallback;     // off+72
    Bedrock::Threading::IAsyncResult<void>::Handle         mCurrentTaskHandle;   // off+136
    std::mutex                                             mLock;                // off+152
    bool                                                   mHasBeenTerminated;   // off+232
    const std::chrono::seconds                             mDeadlineSeconds;     // off+240
    bool                                                   mAssertAfterDeadline; // off+248
    std::chrono::steady_clock::time_point                  mLastQueueTime;       // off+256
};