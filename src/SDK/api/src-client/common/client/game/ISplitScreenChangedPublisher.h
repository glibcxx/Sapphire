#pragma once

#include "SDK/api/src-deps/Core/Utility/pubsub/Subscription.h"

// size: 8
class ISplitScreenChangedPublisher {
public:
    // vtb+0
    virtual Bedrock::PubSub::Subscription registerSplitScreenChangedListener(std::function<void(uint8_t)>) const = 0;

    // vtb+1
    virtual uint8_t getSplitScreenCount() const = 0;
};
