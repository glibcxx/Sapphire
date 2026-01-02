#pragma once

#include <map>
#include <mutex>
#include <memory>
#include "SDK/api/src-deps/Core/Utility/pubsub/PriorityDeferredSubscriptionHub.h"

namespace Bedrock {

    // size: 104
    class SignalReceiver {
    public:
        std::mutex                                                                mMutex; // off+0
        std::unique_ptr<PubSub::PriorityDeferredSubscriptionHub>                  mUnk80; // off+80
        std::map<std::aligned_storage_t<114, 8> *, std::aligned_storage_t<24, 8>> mUnk88; // off+88
    };

} // namespace Bedrock
