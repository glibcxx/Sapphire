#pragma once

#include <vector>
#include <bitset>
#include <array>
#include "SDK/api/src-deps/Core/Threading/InstancedThreadLocal.h"

namespace dragon::platform::registry {

    template <typename... Components>
    class BasicRenderRegistry {
    public:
        using ComponentType = std::tuple<std::vector<Components>...>;
        using IndexPair = std::pair<std::bitset<sizeof...(Components)>, std::array<uint16_t, sizeof...(Components)> >;
        using IndexVector = std::vector<IndexPair>;
        using Pool = std::pair<IndexVector, ComponentType>;
        using ThreadLocalComponentType = Bedrock::Threading::InstancedThreadLocal<Pool>;

        ThreadLocalComponentType mThreadLocalComponents; // off+0
        // ...
    };

} // namespace dragon::platform::registry