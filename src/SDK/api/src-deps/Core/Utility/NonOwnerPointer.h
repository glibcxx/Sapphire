#pragma once

#include "EnableNonOwnerReferences.h"

namespace Bedrock {

    template <typename T>
    class NonOwnerPointer {
    public:
        std::shared_ptr<EnableNonOwnerReferences::ControlBlock> mControlBlock; // off+0
#if MC_VERSION >= v1_21_50
        T *mPtr; // off+16
#endif
    };

} // namespace Bedrock