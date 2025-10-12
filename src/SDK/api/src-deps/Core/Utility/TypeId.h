#pragma once

namespace Bedrock {

    template <typename Namespace>
    class typeid_t {
    public:
        uint16_t mID; // off+0

        constexpr auto operator<=>(const typeid_t &) const = default;

        constexpr uint16_t value() const { return mID; }
    };

} // namespace Bedrock

template <typename Namespace, typename T>
Bedrock::typeid_t<Namespace> type_id();