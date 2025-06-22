#pragma once

#include <memory>
#include <vector>
#include "ITransactionContainer.h"

namespace mce {

    template <typename Shared, typename Weak>
    class SimpleDeferredResourceTracker {
    public:
        using ResourceBlockPtr = Shared;
        using ResourceTrackingBlockPtr = Weak;
        using TransactionContainer = std::unique_ptr<mce::ITransactionContainer>;
        using TransactionPair = std::pair<std::weak_ptr<Weak>, TransactionContainer>;

        std::vector<TransactionPair> mPendingTransactionHandles; // off+0
    };

    template <typename Shared, typename Weak>
    class SimpleResourceTracker {
    public:
        std::vector<Weak>                                mResourceTrackingBlocks;
        mce::SimpleDeferredResourceTracker<Shared, Weak> mDeferredResourceTracker;
    };

} // namespace mce