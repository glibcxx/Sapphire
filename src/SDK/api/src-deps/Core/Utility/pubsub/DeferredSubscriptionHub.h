#pragma once

namespace Bedrock::PubSub {

    class RawSubscription;
    class DeferredSubscription;

    // https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/deps/core/utility/pub_sub/DeferredSubscriptionHub.h#L16

    enum class ConnectPosition : int {
        AtBack = 0,
        AtFront = 1,
    };

    // size: 8 (1.21.50)
    class DeferredSubscriptionHub {
    public:
        enum class HubType : int {
            Priority = 0,
            Fifo = 1,
            RecursiveFIFO = 2,
        };

        // vtb+0
        virtual ~DeferredSubscriptionHub() = default;

        // vtb+1
        virtual size_t runDeferredEvents(size_t maxToRun) = 0;

        // vtb+2
        virtual size_t runDeferredEventsForDuration(std::chrono::milliseconds msec) = 0;

        // vtb+3
        virtual void flushPendingEvents() = 0;

        // vtb+4
        virtual void clear() = 0;

        // vtb+5
        virtual size_t erase(RawSubscription &subscription) = 0;

        // vtb+6
        virtual size_t size() const = 0;

        // vtb+7
        virtual bool empty() const = 0;

        // vtb+8
        virtual HubType getHubType() const = 0;

        // vtb+9
        virtual void _join(DeferredSubscription &&subscription) = 0;

        // vtb+10
        virtual void _enqueue(std::function<void()> fn, ConnectPosition at, std::optional<int> group) = 0;
    };

} // namespace Bedrock::PubSub