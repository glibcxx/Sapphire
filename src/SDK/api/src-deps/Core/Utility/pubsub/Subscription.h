#pragma once

#include <memory>

namespace Bedrock::PubSub {

    namespace Detail {
        class SubscriptionBodyInterface;
    } // namespace Detail

    // size: 16
    class Subscription {
    public:
        std::weak_ptr<Bedrock::PubSub::Detail::SubscriptionBodyInterface> mBody; // off+0

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x8B\x00\x00\x00\x00\x33\xF6\x48\x85\xC9\x74\x00\xE8")
        SDK_API void disconnect();
    };

    // size: 16
    class ScopedSubscription : public Subscription {
    public:
        ~ScopedSubscription() { disconnect(); }
    };

    // size: 16
    class RawSubscription : public Subscription {};

    // size: 16
    class DeferredSubscription : public Subscription {};

} // namespace Bedrock::PubSub