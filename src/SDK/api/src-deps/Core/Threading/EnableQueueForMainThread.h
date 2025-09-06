#pragma once

#include "EnableQueueForThread.h"

namespace Bedrock::Threading {

    // size: 16
    class EnableQueueForMainThread : public EnableQueueForThread {
    public:
        SDK_API EnableQueueForMainThread *ctor(std::string name);
    };
    static_assert(sizeof(EnableQueueForMainThread) == 16);

} // namespace Bedrock::Threading