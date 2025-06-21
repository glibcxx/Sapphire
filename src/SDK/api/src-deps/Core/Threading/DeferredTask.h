#pragma once

#include <condition_variable>
#include <atomic>
#include <mutex>
#include <functional>

#include "SDK/core/Core.h"

namespace Core {

    // size: 232
    class DeferredTask {
    public:
        enum class State : int {
            Ready = 0,
            Executing = 1,
            Done = 2,
        };

        std::atomic<State>        mState;            // off+0
        std::condition_variable   mCondition;        // off+8
        std::mutex                mExecutionMutex;   // off+80
        std::function<void(void)> mCallback;         // off+160
        const bool                mCanSkipExecution; // off+224

        SDK_API bool tryExecute();
    };
    static_assert(sizeof(DeferredTask) == 232);

} // namespace Core