#pragma once

#include <concurrentqueue.h>

// size: 616
template <typename T>
class MPMCQueue
{
public:
    moodycamel::ConcurrentQueue<T, moodycamel::ConcurrentQueueDefaultTraits> mQueue; // off+0
};