#pragma once

#include "BackgroundTask.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class Scheduler;
class WorkerPool;

// size: 320 (1.21.2), 336 (1.21.50/1.21.60)
class TaskGroup : public IBackgroundTaskOwner {
public:
    Bedrock::NonOwnerPointer<Scheduler>  mScheduler;        // off+8
    Bedrock::NonOwnerPointer<WorkerPool> mWorkers;          // off+32
    std::string                          mName;             // off+56
    bool                                 mCheckOwnerThread; // off+88
    std::mutex                           mLock;             // off+96
    std::atomic<TaskGroupState>          mState;            // off+176
    std::shared_ptr<BackgroundTaskBase>  mTasks;            // off+184
    size_t                               mTaskCount;        // off+200
    std::shared_ptr<BackgroundTaskBase>  mEnumCurr;         // off+208
    std::shared_ptr<BackgroundTaskBase>  mEnumNext;         // off+224
    std::shared_ptr<void>                mUnk240;           // off+240
    std::mutex                           mUnk256;           // off+256

    // vtb+0
    virtual ~TaskGroup() override;

    // vtb+1
    virtual Bedrock::Threading::IAsyncResult<void>::Handle queue(
        const TaskStartInfoEx<void>              &startInfo,
        brstd::move_only_function<TaskResult()> &&task,
        std::function<void()>                   &&callback
    ) override;

    // vtb+2
    virtual Bedrock::Threading::IAsyncResult<void>::Handle queueSync(
        const TaskStartInfoEx<void> &startInfo, brstd::move_only_function<TaskResult()> &&task
    ) override;

    // vtb+3
    virtual void taskRegister(std::shared_ptr<BackgroundTaskBase> task) override;

    // vtb+4
    virtual void requeueTask(std::shared_ptr<BackgroundTaskBase> task, bool queueImmediate) override;

    // vtb+5
    virtual TaskGroupState getState() const override;

    // vtb+6
    virtual void processCoroutines() override;

    // vtb+7
    virtual void taskComplete(gsl::not_null<BackgroundTaskBase *> task) override;
};