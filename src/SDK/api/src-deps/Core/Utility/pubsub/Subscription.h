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
    };

} // namespace Bedrock::PubSub