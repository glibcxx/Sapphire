#pragma once

#include "DeferredSubscriptionHubBase.h"
#include "SDK/api/src-deps/Core/Threading/MPSCQueue.h"

namespace Bedrock::PubSub {

    // https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/deps/core/utility/pub_sub/PriorityDeferredSubscriptionHub.h#L13

    // size+ 632 (1.21.50)
    class PriorityDeferredSubscriptionHub : public DeferredSubscriptionHubBase {
    public:
        // size: 72 (1.21.50)
        struct DequeuedEntry {
            std::function<void()> mUnk0;  // off+0
            __int32               mUnk64; // off+64
        };

        // size: 80 (1.21.50)
        struct QueueEntry {
            std::function<void()> mFn;       // off+0
            std::optional<int>    mGroup;    // off+64
            ConnectPosition       mPosition; // off+72
        };

        MPSCQueue<QueueEntry>     mUnk32;  // off+32
        std::deque<DequeuedEntry> mUnk592; // off+592

        // vtb+0
        virtual ~PriorityDeferredSubscriptionHub() override = default;

        // vtb+12
        virtual void _runDequeuedEntry(const DequeuedEntry &entry);
    };

} // namespace Bedrock::PubSub