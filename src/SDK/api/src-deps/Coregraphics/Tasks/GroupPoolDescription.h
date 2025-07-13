#pragma once

class WorkerPool;
class Scheduler;

namespace cg {

    // size: 24
    struct GroupPoolDescription {
        WorkerPool &mPool;      // off+0
        Scheduler  &mScheduler; // off+8
        bool        mIsHelper;  // off+16
    };

} // namespace cg