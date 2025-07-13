#pragma once

namespace cg {

    namespace details {
        template <typename TContext, size_t U = 0>
        class LambdaExecutorContextContainer {
        public:
            TContext mContext; // off+0
        };
    } // namespace details

    // size: 32
    template <typename TContext, typename TPolicy, typename T2, typename T3>
    class LambdaExecutor {
    public:
        using ContextContainer = details::LambdaExecutorContextContainer<TContext>;
        using Executor = LambdaExecutor;
        using Context = TContext;
        using WorkItem = std::function<void(TContext &)>;
        using Policy = TPolicy;

        ContextContainer mContextContainer; // off+0
        Context         &mContext;          // off+8
        TPolicy          mExecutionPolicy;  // off+16
    };

} // namespace cg