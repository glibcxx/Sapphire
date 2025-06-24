#pragma once

#include "SDK/api/src-deps/Core/CheckedResourceService/TransactionContainer.h"

namespace mce {

    enum class ResourceServiceState : uint8_t {
        Enabled = 0,
        Disabled = 1,
    };

    template <typename Resource>
    class CheckedResourceService {
        static_assert(false, "默认实现不可用，请手动特化");
    };

} // namespace mce