#pragma once

#include <vector>
#include <bitset>
#include <array>
#include "SDK/api/src-deps/Core/Threading/InstancedThreadLocal.h"
#include "SDK/api/src-deps/Core/Memory/RingAllocatorContainer.h"

namespace dragon::platform::registry {

    // size: 176
    template <typename... Components>
    class BasicRenderRegistry {
    public:
        using ComponentType = std::tuple<std::vector<Components>...>;
        using IndexPair = std::pair<std::bitset<sizeof...(Components)>, std::array<uint16_t, sizeof...(Components)>>;
        using IndexVector = std::vector<IndexPair>;
        using Pool = std::pair<IndexVector, ComponentType>;
        using ThreadLocalComponentType = Bedrock::Threading::InstancedThreadLocal<Pool>;

        ThreadLocalComponentType mThreadLocalComponents; // off+0

        std::unique_ptr<Core::RingAllocatorContainer<uint8_t>::AllocationScope> *mFrameAllocator; // off+168

        template <typename... Comps>
        void add(Comps &&...args) {
            Pool     &pool = mThreadLocalComponents.get();
            IndexPair back;
            ([](auto &&type, auto &components, auto &i) {
                using Ty = std::decay_t<Comps>;
                auto &vec = std::get<std::vector<Ty>>(components);
                vec.emplace_back(std::forward<Comps>(type));
                auto idx = componentIndex<Ty>();
                i.first.set(idx);
                i.second[idx] = vec.size() - 1;
            }(std::forward<Comps>(args), pool.second, back),
             ...);
            pool.first.push_back(std::move(back));
        }

        template <typename Comp, std::size_t I = 0>
        static constexpr auto componentIndex() {
            if constexpr (I >= std::tuple_size_v<ComponentType>)
                static_assert(false, "Component not found in tuple");
            else if constexpr (std::is_same_v<std::vector<Comp>, std::tuple_element_t<I, ComponentType>>)
                return I;
            else
                return componentIndex<Comp, I + 1>();
        }
    };

} // namespace dragon::platform::registry