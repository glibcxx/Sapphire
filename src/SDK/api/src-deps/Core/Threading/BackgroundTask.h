#pragma once

#include "TaskStartInfo.h"
#include "TaskGroupState.h"
#include "SDK/api/src-deps/Core/Utility/brstd/move_only_function.h"

class TaskResult;
class TaskGroup;

enum class TaskRunResult : int {
    Requeue = 0,
    Done = 1,
    Noop = 2,
};

// size: 8
class ITaskExecutionContext {
public:
    // vtb+0
    virtual ~ITaskExecutionContext() = default;

    // vtb+2
    virtual bool isAsync() const = 0;

    // vtb+3
    virtual bool canTaskRunAgain() const = 0;
};

// size: 72
class BackgroundTaskBase {
public:
    // size: 1
    class PriorityComparer {
    public:
        bool operator()(
            const std::shared_ptr<BackgroundTaskBase> &lhs, const std::shared_ptr<BackgroundTaskBase> &rhs
        ) const { return lhs->mPriority > rhs->mPriority; }
    };

    // size: 1
    class PendingComparer {
    public:
        bool operator()(
            const std::shared_ptr<BackgroundTaskBase> &lhs, const std::shared_ptr<BackgroundTaskBase> &rhs
        ) const { return lhs->mStartAfterTime < rhs->mStartAfterTime; }
    };

    const bool                            mIsAsync;                // off+8
    TaskGroup                            *mGroup;                  // off+16
    BackgroundTaskBase                   *mPrevTask;               // off+24
    std::shared_ptr<BackgroundTaskBase>   mNextTask;               // off+32
    int                                   mPriority;               // off+48
    std::thread::id                       mAffinity;               // off+52
    const int                             mBackDownPriorityAmount; // off+56
    std::chrono::steady_clock::time_point mStartAfterTime;         // off+64

    // size: 8
    class CurrentTaskAutoScope {
        BackgroundTaskBase *mPrev; // off+0

        CurrentTaskAutoScope();
        ~CurrentTaskAutoScope();
    };

    // vtb+0
    virtual ~BackgroundTaskBase();

    // vtb+1
    virtual void cancel() = 0;

    // vtb+2
    virtual bool isReady() const = 0;

    // vtb+3
    virtual TaskRunResult run(const ITaskExecutionContext *) = 0;
};

namespace TaskDetails {

    template <typename T1, typename T2>
    class BackgroundTaskDetails : public Bedrock::Threading::IAsyncResult<T2> {
    public:
        using AsyncResultType = T2;
        using TaskFunction = brstd::move_only_function<T1()>;
        using PredecessorHandle = std::shared_ptr<Bedrock::Threading::IAsyncResult<AsyncResultType>>;
        using CompletionHandler = std::function<void(const Bedrock::Threading::IAsyncResult<AsyncResultType> &)>;

        TaskFunction mTask; // off+24

        std::optional<std::conditional_t<std::is_void_v<T2>, char, T2>> mAsyncResult;
        std::optional<std::conditional_t<std::is_void_v<T1>, char, T1>> mTaskResult;

        // vtb+0
        virtual ~BackgroundTaskDetails() override;
    };

    // size: 88
    template <>
    class BackgroundTaskDetails<TaskResult, void> : public Bedrock::Threading::IAsyncResult<void> {
    public:
        using AsyncResultType = void;
        using TaskFunction = brstd::move_only_function<TaskResult()>;
        using PredecessorHandle = std::shared_ptr<Bedrock::Threading::IAsyncResult<AsyncResultType>>;
        using CompletionHandler = std::function<void(const Bedrock::Threading::IAsyncResult<AsyncResultType> &)>;

        TaskFunction mTask; // off+24

        // vtb+0
        virtual ~BackgroundTaskDetails() override;
    };

} // namespace TaskDetails

// size: 296 (BackgroundTask<TaskResult, void>)
template <typename T1, typename T2 = void>
class BackgroundTask : public /* off+0  */ BackgroundTaskBase,
                       public /* off+72 */ TaskDetails::BackgroundTaskDetails<T1, T2> {
public:
    using AsyncResultType = T2;
    using TaskFunction = brstd::move_only_function<T1()>;
    using PredecessorHandle = Bedrock::Threading::IAsyncResult<AsyncResultType>::Handle;
    using CompletionHandler = std::function<void(const Bedrock::Threading::IAsyncResult<AsyncResultType> &)>;

    enum class TaskStatus : int {
        WaitingForPredecessor = 0,
        Pending = 1,
        Running = 2,
        CancelPending = 3,
        Canceled = 4,
        Error = 5,
        Done = 6,
    };

    std::mutex                     mLock;             // off+160
    TaskStatus                     mStatus;           // off+240
    std::vector<CompletionHandler> mComplete;         // off+248
    bool                           mLinkCancellation; // off+272
    PredecessorHandle              mPredecessor;      // off+280
};

// sise: 8
class IBackgroundTaskOwner {
public:
    // vtb+0
    virtual ~IBackgroundTaskOwner();

    // vtb+1
    virtual Bedrock::Threading::IAsyncResult<void>::Handle queue(
        const TaskStartInfoEx<void>              &startInfo,
        brstd::move_only_function<TaskResult()> &&task,
        std::function<void()>                   &&callback
    ) = 0;

    // vtb+2
    virtual Bedrock::Threading::IAsyncResult<void>::Handle queueSync(
        const TaskStartInfoEx<void> &startInfo, brstd::move_only_function<TaskResult()> &&task
    ) = 0;

    // vtb+3
    virtual void taskRegister(std::shared_ptr<BackgroundTaskBase> task) = 0;

    // vtb+4
    virtual void requeueTask(std::shared_ptr<BackgroundTaskBase> task, bool queueImmediate) = 0;

    // vtb+5
    virtual TaskGroupState getState() const = 0;

    // vtb+6
    virtual void processCoroutines() = 0;

    // vtb+7
    virtual void taskComplete(gsl::not_null<BackgroundTaskBase *> task) = 0;
};
