#pragma once

#include <variant>
#include <gsl/gsl>
#include "SDK/core/Core.h"
#include "src/SDK/api/src-deps/Core/Threading/BurstWorkQueue.h"

class WorkerPool;
class Scheduler;

namespace dragon::task {

    class AsyncTasksScope {};

    class GraphicsTasks {
    public:
        enum class ExecutionMode : int32_t {
            Immediate = 0,
            Deferred = 1,
            RenderThreadLocked = 2,
            Unconstrained = 3,
        };

        struct InitBgfx {
            std::shared_ptr<void /*bgfx::Init*/> mInitParameters; // off+0
            bool                                 mUnk16;          // off+16
            ExecutionMode                        mMode;           // off+20
            bool                                 mUnk24;          // off+24
        };

        struct InitBegin : InitBgfx {};

        struct InitPending : InitBgfx {
            std::shared_ptr<void> mUnk32; // off+32
            std::shared_ptr<void> mUnk48; // off+48
        };

        struct InitFinalize {};

        struct InitEnd {
            bool mUnk0; // off+0
        };

        // size: 72
        struct InitializationState : public std::variant<std::monostate, InitBegin, InitPending, InitFinalize, InitEnd> {
            InitializationState *ctor(const InitializationState &other); 
        };

        std::thread::id                                       mRendererThreadId;   // off+0
        bool                                                  mInitialized;        // off+4
        bool                                                  mSkipPresenting;     // off+5
        ExecutionMode                                         mExecutionMode;      // off+8
        gsl::not_null<WorkerPool *>                           mRendererPool;       // off+16
        std::vector<std::reference_wrapper<WorkerPool>>       mHelperPools;        // off+24
        gsl::not_null<Scheduler *>                            mClientScheduler;    // off+48
        std::shared_ptr<MPMCQueue<std::function<void(void)>>> mResourceTasks;      // off+56
        std::shared_ptr<MPMCQueue<std::function<void(void)>>> mRenderingTasks;     // off+72
        std::shared_ptr<MPMCQueue<std::function<void(void)>>> mRenderingPostTasks; // off+88

        // Scheduler mRenderingCommandScheduler; // off+168

        // std::unique_ptr<Bedrock::Threading::Burst::WorkQueue<std::function<void(void)>>> mMainFrameRenderTasksQueue; // off+304

        SDK_API InitializationState continueInit(InitializationState initState);

        SDK_API bool _queueMainFrameRenderTask(const std::function<void(void)> &task);

        class _lambda_144B71107_at_continueInit {
        public:
            GraphicsTasks *__this;

            SDK_API std::variant<std::monostate, InitBegin, InitPending, InitFinalize, InitEnd> operator()(const InitBegin &begin) const;
        };
    };

}; // namespace dragon::task