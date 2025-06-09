#pragma once

#include <memory>
#include "SDK/core/Core.h"

namespace Bedrock {

    // size: 24
    class EnableNonOwnerReferences {
    public:
        struct ControlBlock {
#if MC_VERSION == v1_21_2
            EnableNonOwnerReferences* mPtr;
#elif MC_VERSION >= v1_21_50
            bool mIsValid;
#endif
        };

        std::shared_ptr<ControlBlock> mControlBlock; // off+8

        virtual ~EnableNonOwnerReferences() {}
    };

} // namespace Bedrock