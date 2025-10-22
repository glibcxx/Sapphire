#pragma once

#include "Async.h"

enum class TaskOptions : uint32_t {
    Default = 0,
    TaskProfiled = 1,
    LinkPredecessor = 2,
};

// size: 40
struct TaskStartInfoBase {
    std::string_view                      name;             // off+0
    std::thread::id                       affinity;         // off+16
    uint32_t                              priority;         // off+20
    int32_t                               priorityBackDown; // off+24
    TaskOptions                           options;          // off+28
    std::chrono::steady_clock::time_point startAtTime;      // off+32
};

// size: 56
template <typename TResult>
struct TaskStartInfoEx : public TaskStartInfoBase {
public:
    Bedrock::Threading::IAsyncResult<TResult>::Handle predecessor; // off+40
};