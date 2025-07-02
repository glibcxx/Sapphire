#pragma once

#include "SDK/api/src-deps/Core/Threading/MPMCQueue.h"

namespace Core {

    // size: 616
    class SharedMemoryTracker {
    public:
        class SharedMemoryTrackerBase {
        public:
            virtual ~SharedMemoryTrackerBase() = 0;
        };

        MPMCQueue<std::shared_ptr<SharedMemoryTrackerBase> > mTrackedMemory; // off+0
    };

} // namespace Core