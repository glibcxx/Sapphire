#pragma once

#include <condition_variable>
#include <atomic>
#include <mutex>
#include <functional>

#include "SDK/core/Core.h"

namespace dragon::rendering {
    class GraphicsTasks;
}

// size: 80
class lambda_144B72F30 {
public:
    dragon::rendering::GraphicsTasks *const __this;               // off+0
    std::function<void()>                   frameContentCallback; // off+8
    bool                                    mUnk72;               // off+72

    SDK_API void operator()() const;
};
static_assert(sizeof(lambda_144B72F30) == 80);

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

        template <typename Callback>
        DeferredTask *ctor(Callback &&callback);

        template <>
        SDK_API DeferredTask *ctor(lambda_144B72F30 &&callback);

        SDK_API bool tryExecute();
    };
    static_assert(sizeof(DeferredTask) == 232);

} // namespace Core