#pragma once

#include "EnableQueueForThread.h"

namespace Bedrock::Threading {

    // size: 16
    class EnableQueueForMainThread : public EnableQueueForThread {
    public:
        SDK_API EnableQueueForMainThread(std::string name);

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x49\x89\x6E\x00\x49\x89\x6E\x00\x48\x8D\x05")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x4C\x89\x00\x00\x4C\x89\x00\x00\x4C\x8D")
        SPHR_CTOR_ALIAS SDK_API EnableQueueForMainThread *ctor(std::string name);
    };
    static_assert(sizeof(EnableQueueForMainThread) == 16);

} // namespace Bedrock::Threading