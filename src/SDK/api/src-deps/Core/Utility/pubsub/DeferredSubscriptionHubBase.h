#pragma once

#include "DeferredSubscriptionHub.h"

namespace Bedrock::PubSub {

    class Subscription;

    // https://github.com/LiteLDev/LeviLamina/blob/4d08de07ef76fb4f101aebb8b251e2652dc4cfbd/src/mc/deps/core/utility/pub_sub/DeferredSubscriptionHubBase.h#L16

    // size: 32 (1.21.50)
    class DeferredSubscriptionHubBase : public DeferredSubscriptionHub {
    public:
        std::vector<Subscription> mUnk24; // off+24

        // vtb+0
        virtual ~DeferredSubscriptionHubBase() override;

        // vtb+11
        virtual bool _runOneEvent() = 0;
    };

} // namespace Bedrock::PubSub