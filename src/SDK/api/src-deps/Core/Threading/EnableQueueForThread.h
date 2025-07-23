#pragma once

#include "SDK/api/src-deps/Core/Threading/TaskGroup.h"

namespace Bedrock::Threading {

    // size: 16
    class EnableQueueForThread {
    public:
        std::unique_ptr<TaskGroup> mQueueForMainThreadTaskGroup; // off+8

        // vtb+0
        virtual ~EnableQueueForThread() = default;
    };
    static_assert(sizeof(EnableQueueForThread) == 16);

} // namespace Bedrock::Threading