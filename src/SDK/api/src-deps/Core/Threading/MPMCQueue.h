#pragma once

#include <concurrentqueue.h>

// size: 616
template <typename T>
class MPMCQueue {
public:
    moodycamel::ConcurrentQueue<T, moodycamel::ConcurrentQueueDefaultTraits> mQueue; // off+0
};

namespace mpmc {

    template <typename T>
    class Sender {
    public:
        std::shared_ptr<MPMCQueue<T>> mQueue; // off+0
    };

} // namespace mpmc