#pragma once

namespace cg {

    namespace details {
        template <typename TToken, typename TResult>
        struct DispatchToken {
            TToken                 mToken;  // off+0
            std::optional<TResult> mResult; // off+16
        };

        template <typename T>
        class ScopedFrameBase {};

        template <typename TExecution, size_t N = 0>
        class ScopedFrame : public ScopedFrameBase<TExecution> {
        public:
            typename TExecution::Dispatcher *mDispatcher;
        };

        template <typename TExecution, typename TContext>
        class SpecializedScopedFrame : public ScopedFrame<TExecution> {
        public:
            using WorkItemResult = std::function<void(TContext &)>;
        };

        template <typename TExecution>
        class SpecializedScopedFrame<TExecution, void> : public ScopedFrame<TExecution> {
        public:
        };
    } // namespace details

    template <typename TScheduler>
    class TaskDispatcher {
    public:
        // size: 1
        struct TaskDispatcherExecution {
            using Dispatcher = TaskDispatcher;
        };

        TScheduler mScheduler; // off+0
    };

} // namespace cg
