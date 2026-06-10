#pragma once

#include "pch.h" // IWYU pragma: keep

namespace Bedrock {

    class StaticOptimizedString {
    public:
        const char *data;

        enum class StorageType : int {
            Static = 0,
            Dynamic = 1,
        };
    };

} // namespace Bedrock