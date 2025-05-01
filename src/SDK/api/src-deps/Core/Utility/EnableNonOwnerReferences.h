#pragma once

#include <memory>

namespace Bedrock {

    class EnableNonOwnerReferences {
    public:
        struct ControlBlock {
            bool mIsValid;
        };

        std::shared_ptr<ControlBlock> mControlBlock;

        virtual ~EnableNonOwnerReferences() = 0;
    };

} // namespace Bedrock