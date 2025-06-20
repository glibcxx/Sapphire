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
        using IndexPair = std::pair<std::bitset<sizeof...(Components)>, std::array<uint16_t, sizeof...(Components)>>;
        using IndexVector = std::vector<IndexPair>;
        using Pool = std::pair<IndexVector, ComponentType>;
        using ThreadLocalComponentType = Bedrock::Threading::InstancedThreadLocal<Pool>;

        ThreadLocalComponentType mThreadLocalComponents; // off+0
        // ...

        template <typename... Comps>
        void add(Comps &&...args) {
            Pool     &pool = mThreadLocalComponents.get();
            IndexPair back;
            ([](auto &&type, auto &components, auto &i) {
                using Ty = std::decay_t<Comps>;
                auto &vec = std::get<std::vector<Ty>>(components);
                vec.emplace_back(std::forward<Comps>(type));
                auto idx = tupleIndex<std::vector<Ty>, ComponentType>();
                i.first.set(idx);
                i.second[idx] = vec.size() - 1;
            }(std::forward<Comps>(args), pool.second, back),
             ...);
            pool.first.push_back(std::move(back));
        }

        template <typename T, typename Tuple, std::size_t I = 0>
        static constexpr std::size_t tupleIndex() {
            if constexpr (I >= std::tuple_size_v<Tuple>)
                static_assert(false, "Type not found in tuple");
            else if constexpr (std::is_same_v<T, std::tuple_element_t<I, Tuple>>)
                return I;
            else
                return tupleIndex<T, Tuple, I + 1>();
        }
    };

} // namespace dragon::platform::registry