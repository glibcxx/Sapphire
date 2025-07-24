#pragma once

#include <variant>
#include <gsl/gsl>
#include "SDK/core/ApiManager.h"
#include "SDK/api/src-deps/Core/Threading/BurstWorkQueue.h"
#include "SDK/api/src-deps/Core/Threading/DeferredTask.h"
#include "SDK/api/src-deps/Core/Threading/Scheduler.h"
#include "SDK/api/src-deps/Coregraphics/Tasks/TaskDispatcher.h"
#include "SDK/api/src-deps/Coregraphics/Tasks/TaskScheduler.h"
#include "SDK/api/src-deps/Coregraphics/Tasks/LambdaExecutor.h"
#include "SDK/api/src-deps/Coregraphics/Tasks/TaskPoolGroupExecutionPolicy.h"
#include "SDK/api/src-deps/Coregraphics/Tasks/GroupPoolDescription.h"
#include "SDK/api/src-external/RenderDragon/Rendering/RenderContext.h"

class WorkerPool;
class Scheduler;

namespace dragon::task {

    class AsyncTasksScope {
    public:
        MPMCQueue<
            std::shared_ptr<
                const cg::details::DispatchToken<
                    std::shared_ptr<const cg::details::WorkToken>,
                    std::shared_ptr<std::function<void(dragon::rendering::RenderContext &)>>>>>
            mAsyncTasks; // off+0

        std::atomic<bool> mCompleted; // off+616
        cg::details::SpecializedScopedFrame<
            cg::TaskDispatcher<
                cg::TaskScheduler<
                    cg::LambdaExecutor<
                        dragon::rendering::RenderContext,
                        cg::TaskPoolGroupExecutionPolicy<
                            dragon::rendering::RenderContext,
                            1>,
                        const std::string &,
                        const std::vector<cg::GroupPoolDescription> &>>>::TaskDispatcherExecution,
            void>
            mScopedFrame; // off+624
    };

    // size: 336
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

        std::thread::id                                       mRendererThreadId;          // off+0
        bool                                                  mInitialized;               // off+4
        bool                                                  mSkipPresenting;            // off+5
        ExecutionMode                                         mExecutionMode;             // off+8
        gsl::not_null<WorkerPool *>                           mRendererPool;              // off+16
        std::vector<std::reference_wrapper<WorkerPool>>       mHelperPools;               // off+24
        gsl::not_null<Scheduler *>                            mClientScheduler;           // off+48
        std::shared_ptr<MPMCQueue<std::function<void(void)>>> mUnk56;                     // off+56
        std::shared_ptr<MPMCQueue<std::function<void(void)>>> mUnk72;                     // off+72
        std::shared_ptr<MPMCQueue<std::function<void(void)>>> mResourceTasks;             // off+88
        std::shared_ptr<MPMCQueue<std::function<void(void)>>> mRenderingTasks;            // off+104
        std::shared_ptr<MPMCQueue<std::function<void(void)>>> mRenderingPostTasks;        // off+120
        std::shared_ptr<Core::DeferredTask>                   mFrameContentCallback;      // off+136
        std::shared_ptr<Core::DeferredTask>                   mRenderingFrameWorker;      // off+152
        Scheduler                                             mRenderingCommandScheduler; // off+168
        cg::TaskDispatcher<
            cg::TaskScheduler<
                cg::LambdaExecutor<
                    dragon::rendering::RenderContext,
                    cg::TaskPoolGroupExecutionPolicy<dragon::rendering::RenderContext>,
                    const std::string &,
                    const std::vector<cg::GroupPoolDescription> &>>>
            mGraphicsDispatcher; // off+272
        cg::TaskDispatcher<
            cg::TaskScheduler<
                cg::LambdaExecutor<
                    dragon::rendering::RenderContext,
                    cg::TaskPoolGroupExecutionPolicy<dragon::rendering::RenderContext>,
                    const std::string &,
                    const std::vector<cg::GroupPoolDescription> &>>>
            mParallelGraphicsDispatcher; // off+288

        using BurstQueue = Bedrock::Threading::Burst::WorkQueue<std::function<void()>>;

        std::unique_ptr<BurstQueue>         mMainFrameRenderTasksQueue; // off+304
        std::atomic<uint32_t>               mFrameCount;                // off+312
        size_t                              mWorkersCount;              // off+320
        std::chrono::steady_clock::duration mUnk328;                    // off+328

        SDK_API GraphicsTasks *ctor(
            WorkerPool                                   &rendererPool,
            gsl::span<std::reference_wrapper<WorkerPool>> helperPools,
            Scheduler                                    &clientScheduler
        );
        MARK_HOOKABLE(&GraphicsTasks::ctor)

        SDK_API InitializationState continueInit(InitializationState initState);
        MARK_HOOKABLE(&GraphicsTasks::ctor)

        SDK_API bool _queueMainFrameRenderTask(const std::function<void(void)> &task);
        MARK_HOOKABLE(&GraphicsTasks::_queueMainFrameRenderTask)

        SDK_API void waitForPreviousFrame();
        MARK_HOOKABLE(&GraphicsTasks::waitForPreviousFrame)

        SDK_API void frame(
            const std::function<void(void)>                                                               &frameContentCallback,
            const std::function<std::chrono::steady_clock::duration(std::chrono::steady_clock::duration)> &a2
        );
        MARK_HOOKABLE(&GraphicsTasks::frame)

        class _lambda_144B71107_at_continueInit {
        public:
            GraphicsTasks *__this;

            SDK_API std::variant<std::monostate, InitBegin, InitPending, InitFinalize, InitEnd> operator()(const InitBegin &begin) const;
            MARK_HOOKABLE(&_lambda_144B71107_at_continueInit::operator())
        };
    };
    static_assert(sizeof(GraphicsTasks) == 336);

}; // namespace dragon::task