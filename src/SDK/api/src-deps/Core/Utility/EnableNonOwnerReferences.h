#pragma once

#include <memory>

namespace Bedrock {

    // size: 24
    class EnableNonOwnerReferences {
    public:
        struct ControlBlock {
#if MC_VERSION == v1_21_2
            EnableNonOwnerReferences *mPtr;
#elif MC_VERSION >= v1_21_50
            bool mIsValid = true;
#endif
            ~ControlBlock() {
#if MC_VERSION == v1_21_2
                mPtr = nullptr;
#elif MC_VERSION >= v1_21_50
                mIsValid = false;
#endif
            }
        };

        std::shared_ptr<ControlBlock> mControlBlock; // off+8

        // vtb+0
        virtual ~EnableNonOwnerReferences() = default;
    };

} // namespace Bedrock