#pragma once

#include <memory>

namespace Bedrock {

    // size: 24
    class EnableNonOwnerReferences {
    public:
        struct ControlBlock {
            bool mIsValid;
        };

        std::shared_ptr<ControlBlock> mControlBlock; // off+8

        virtual ~EnableNonOwnerReferences() = 0;
    };

} // namespace Bedrock