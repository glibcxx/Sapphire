#pragma once

#include <condition_variable>
#include <atomic>
#include <mutex>
#include <functional>

#include "SDK/core/SymbolResolver.h"

namespace dragon::rendering {
    class GraphicsTasks;
}

// size: 80
class lambda_144B72F30 {
public:
    dragon::rendering::GraphicsTasks *const __this;               // off+0
    std::function<void()>                   frameContentCallback; // off+8
    bool                                    mUnk72;               // off+72

#if MC_VERSION == v1_21_2
    SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xF1\x33\xDB\x0F\xB6\x69")
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    SAPPHIRE_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x54\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xF1\xFF\x15")
#endif
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
        SAPPHIRE_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF9\x48\x89\x4C\x24\x00\x48\xC7\x44\x24")
        SAPPHIRE_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xFC\x48\x8D\x47\x00\x48\x89\x83")
        SDK_API DeferredTask *ctor(lambda_144B72F30 &&callback);

        SAPPHIRE_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\xEB\x00\x00\x85\xFF\x74\x00\xF0\xFF\x47")
        SAPPHIRE_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xEB\x00\x4D\x85\xFF\x74\x00\xF0\xFF\x47")
        SDK_API bool tryExecute();
    };
    static_assert(sizeof(DeferredTask) == 232);

} // namespace Core