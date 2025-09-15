#pragma once

#include "SDK/api/src-deps/Core/Threading/BurstWorkQueue.h"

namespace cg {

    // size: 16
    template <typename TContext, size_t U = 1>
    class TaskPoolGroupExecutionPolicy {
    public:
        using Lambda = std::function<void(TContext &)>;
        using WorkItem = Lambda;
        using BurstQueue = Bedrock::Threading::Burst::WorkQueue<WorkItem>;

        std::unique_ptr<BurstQueue> mWorkQueue; // off+0
        TContext                   &mContext;   // off+8
    };

} // namespace cg