#pragma once

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
namespace bgfx {
    struct Init;
}

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
            std::shared_ptr<bgfx::Init> mInitParameters; // off+0
            bool                        mUnk16;          // off+16
            ExecutionMode               mMode;           // off+20
            bool                        mUnk24;          // off+24
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
        struct InitializationState
            : public std::variant<std::monostate, InitBegin, InitPending, InitFinalize, InitEnd> {
            SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\xC6\x45\x00\x00\xEB\x00\x49\x8B\xD0")
            SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xC6\x45\x00\x00\xEB\x00\x49\x83\xC0")
            InitializationState *ctor(const InitializationState &other);
        };

        std::thread::id                                   mRendererThreadId;          // off+0
        bool                                              mInitialized;               // off+4
        bool                                              mSkipPresenting;            // off+5
        ExecutionMode                                     mExecutionMode;             // off+8
        gsl::not_null<WorkerPool *>                       mRendererPool;              // off+16
        std::vector<std::reference_wrapper<WorkerPool>>   mHelperPools;               // off+24
        gsl::not_null<Scheduler *>                        mClientScheduler;           // off+48
        std::shared_ptr<MPMCQueue<std::function<void()>>> mUnk56;                     // off+56
        std::shared_ptr<MPMCQueue<std::function<void()>>> mUnk72;                     // off+72
        std::shared_ptr<MPMCQueue<std::function<void()>>> mResourceTasks;             // off+88
        std::shared_ptr<MPMCQueue<std::function<void()>>> mRenderingTasks;            // off+104
        std::shared_ptr<MPMCQueue<std::function<void()>>> mRenderingPostTasks;        // off+120
        std::shared_ptr<Core::DeferredTask>               mFrameContentCallback;      // off+136
        std::shared_ptr<Core::DeferredTask>               mRenderingFrameWorker;      // off+152
        Scheduler                                         mRenderingCommandScheduler; // off+168
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

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "disp:15,call", "\x4C\x8D\x44\x24\x00\x00\x8B\x00\x00\x8D\x00\x08\x01\x00\x00\xE8\x00\x00\x00\x00\x90\x00\x89")
        SDK_API GraphicsTasks *ctor(
            WorkerPool                                   &rendererPool,
            gsl::span<std::reference_wrapper<WorkerPool>> helperPools,
            Scheduler                                    &clientScheduler
        );

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x48\x0F\xBE\x4D\x00\x48\x83\xF9\x00\x75")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x80\x7D\x00\x00\x75\x00\x0F\xB6\x45")
        SDK_API InitializationState continueInit(InitializationState initState);

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x84\xC0\x41\x0F\x94\xC7\x48\x8B\x4C\x24")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x84\xC0\x75\x00\xB3\x01\x00\x8B")
        SDK_API bool _queueMainFrameRenderTask(const std::function<void()> &task);

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x96\x00\x00\x00\x00\x48\x83\xEA\x00\x4C")
        SDK_API void waitForPreviousFrame();

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x8D\x00\x00\x00\x00\x48\x85\xC9\x74\x00\x48\x8B\x01\x48\x8D\x95\x00\x00\x00\x00\x48\x3B\xCA\x0F\x95\xC2\x48\x8B\x40\x00\xFF\x15\x00\x00\x00\x00\x90\x80\xBD")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x48\x8B\x8D\x00\x00\x00\x00\x48\x85\xC9\x74\x00\x48\x8B\x01\x48\x8D\x95\x00\x00\x00\x00\x48\x3B\xCA\x0F\x95\xC2\x48\x8B\x40\x00\xFF\x15\x00\x00\x00\x00\x90\xBF")
        SDK_API void frame(
            const std::function<void()>                                                                   &frameContentCallback,
            const std::function<std::chrono::steady_clock::duration(std::chrono::steady_clock::duration)> &a2
        );

        class _lambda_144B71107_at_continueInit {
        public:
            GraphicsTasks *__this;

            SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\xEB\x00\x4C\x8B\xC7\x48\x8D\x55")
            SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xEB\x00\x4C\x8B\xC3\x48\x8D\x54\x24\x00\x48\x8D\x8C\x24")
            SDK_API std::variant<std::monostate, InitBegin, InitPending, InitFinalize, InitEnd> operator()(const InitBegin &begin) const;
        };
    };
    static_assert(sizeof(GraphicsTasks) == 336);

}; // namespace dragon::task