#pragma once

#include <string_view>
#include <unordered_map>

namespace util {

    class ApiUniqueId {
        std::string_view mData;

    public:
        consteval ApiUniqueId(std::string_view sig) :
            mData(sig) {}

        constexpr ApiUniqueId(const ApiUniqueId &) = default;
        constexpr ApiUniqueId(ApiUniqueId &&) = default;

        constexpr bool operator==(const ApiUniqueId &rhs) const {
            return this->mData == rhs.mData;
        }

        std::size_t hash() const {
            return std::hash<std::string_view>{}(mData);
        }

        template <auto pFn>
        static consteval auto make() {
            return ApiUniqueId{__FUNCDNAME__};
        }
    };

} // namespace util

template <>
struct std::hash<util::ApiUniqueId> {
    std::size_t operator()(const util::ApiUniqueId &k) const {
        return k.hash();
    }
};