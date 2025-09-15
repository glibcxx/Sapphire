#pragma once

#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"
#include "TaskGroup.h"

class TaskResult;
class Scheduler;
class WorkerPool;

namespace Bedrock::Threading::Burst {

    namespace Strategy {

        namespace Execution {

            // size: 1
            class DefaultExecution {};

            // size: 1
            class GreedyExecution {};

        } // namespace Execution

        // size: 16
        class Default {
            uint64_t mPrimaryWorkerCount;   // off+0
            uint64_t mSecondaryWorkerCount; // off+8
        };

        // size: 16
        class Console {
            uint64_t mPrimaryWorkerCount;   // off+0
            uint64_t mSecondaryWorkerCount; // off+8
        };

    } // namespace Strategy

    namespace Details {

        // size: 8
        class Lifetime {
            std::atomic<uint64_t> mLifetime; // off+0
        };

        // size: 136
        template <typename T>
        class WorkerTask {
        public:
            const std::string     mName;     // off+0
            const uint32_t        mPriority; // off+32
            const std::thread::id mAffinity; // off+36

            std::shared_ptr<Lifetime>             mCurrentLifetime;   // off+40
            TaskGroup                            &mTaskGroup;         // off+56
            const std::function<TaskResult(void)> mCallback;          // off+64
            Strategy::Execution::GreedyExecution  mExecutionStrategy; // off+128
        };

    } // namespace Details

    // size: 104
    struct QueueConfig {
        std::string                                     mName;                 // off+0
        uint32_t                                        mPriority;             // off+32
        Scheduler                                      &mScheduler;            // off+40
        WorkerPool                                     &mPrimaryWorkerPool;    // off+48
        std::vector<std::reference_wrapper<WorkerPool>> mSecondaryWorkerPools; // off+56
        std::vector<std::thread::id>                    mAffinities;           // off+80
    };

    // size: 976
    template <typename Fn, typename Console = Strategy::Console, typename Execution = Strategy::Execution::GreedyExecution>
    class WorkQueue : /*private*/ public Console /* size: 16 */ {
    public:
        using WorkerTask = Details::WorkerTask<Execution>;

        struct TaskGroupTasks {
            TaskGroup                                                                               mTaskGroup;
            std::vector<std::unique_ptr<Details::WorkerTask<Strategy::Execution::GreedyExecution>>> mTasks;
        };

        std::vector<TaskGroup *> mTaskGroups; // off+16

        using WorkItem = Fn; // no comment

        const std::string                            mName;            // off+40
        const uint32_t                               mPriority;        // off+72
        std::function<void(WorkItem &&)>             mPredicate;       // off+80
        std::atomic<size_t>                          mTotalItems;      // off+144
        MPMCQueue<WorkItem>                          mQueue;           // off+152
        std::mutex                                   mWaitMutex;       // off+768
        std::condition_variable                      mWait;            // off+848
        std::unique_ptr<TaskGroupTasks>              mPrimaryGroup;    // off+920
        std::vector<std::unique_ptr<TaskGroupTasks>> mSecondaryGroups; // off+928
        std::vector<std::thread::id>                 mAffinities;      // off+952
    };
    static_assert(sizeof(WorkQueue<std::function<void()>>) == 976);

} // namespace Bedrock::Threading::Burst