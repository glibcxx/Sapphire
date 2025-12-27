#pragma once

#include <type_traits>

namespace Bedrock {

    template <typename Namespace>
    class typeid_t {
    public:
        uint16_t mID; // off+0

        constexpr typeid_t() = default;
        constexpr typeid_t(const typeid_t &other) : mID(other.mID) {}
        constexpr typeid_t &operator=(const typeid_t &other) {
            mID = other.mID;
            return *this;
        }

        constexpr auto operator<=>(const typeid_t &) const = default;

        constexpr uint16_t value() const { return mID; }
    };
    static_assert(!std::is_trivially_copy_constructible_v<typeid_t<void>>);
    static_assert(!std::is_trivially_copy_assignable_v<typeid_t<void>>);

} // namespace Bedrock

template <typename Namespace, typename T>
Bedrock::typeid_t<Namespace> type_id();