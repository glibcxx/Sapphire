#pragma once

#include "EnableNonOwnerReferences.h"

namespace Bedrock {

    // size: 16
    template <class T>
    class UniqueOwnerPointer {
    public:
        std::unique_ptr<Bedrock::EnableNonOwnerReferences> mEnableNonOwnerReferences;
        std::unique_ptr<T>                                 mValue;
    };

} // namespace Bedrock