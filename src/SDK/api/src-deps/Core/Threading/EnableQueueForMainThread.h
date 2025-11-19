#pragma once

#include "EnableQueueForThread.h"

namespace Bedrock::Threading {

    // size: 16
    class EnableQueueForMainThread : public EnableQueueForThread {
    public:
        SDK_API EnableQueueForMainThread(std::string name);
#pragma SPHR_LINKER_SYM_ALIAS(                                                                                                       \
    "??0EnableQueueForMainThread@Threading@Bedrock@@QEAA@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z",          \
    "?ctor@EnableQueueForMainThread@Threading@Bedrock@@QEAAPEAV123@V?$basic_string@DU?$char_traits@D@std@@V?$allocator@D@2@@std@@@Z" \
)

        SDK_API EnableQueueForMainThread *ctor(std::string name);
    };
    static_assert(sizeof(EnableQueueForMainThread) == 16);

} // namespace Bedrock::Threading