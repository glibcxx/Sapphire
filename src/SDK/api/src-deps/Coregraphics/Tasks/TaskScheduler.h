#pragma once

#include <memory>
#include <atomic>
#include <functional>
#include <gsl/gsl>

namespace cg {

    namespace details {
        class WorkToken;
    }

    template <typename TExecutor, typename TWorkToken>
    struct SchedulerExecutionTraits {
        using Executor = TExecutor;
        using Context = typename TExecutor::Context;
        using UniqueExecutor = std::unique_ptr<Executor>;
        using WorkItem = std::function<void(Context &)>;
        using WorkToken = TWorkToken;
        using SharedWorkToken = std::shared_ptr<const WorkToken>;
        using SharedWorkTokenLifetime = const std::shared_ptr<gsl::final_action<std::function<void()>>>;
    };

    // size: 16
    template <typename TExecutor>
    class TaskScheduler {
    public:
        using Executor = cg::SchedulerExecutionTraits<TExecutor, cg::details::WorkToken>;
        using WorkItem = std::function<void(typename Executor::Context &)>;
        using SharedWorkToken = std::shared_ptr<const cg::details::WorkToken>;
        using Context = typename Executor::Context;

        std::unique_ptr<TExecutor> mExecutionContext; // off+0
        std::atomic<int>           mPendingWorkCount; // off+8
    };

} // namespace cg