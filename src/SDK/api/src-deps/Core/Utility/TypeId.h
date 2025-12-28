#pragma once

#include <type_traits>

namespace Bedrock {

    template <typename Namespace>
    class typeid_t {
    public:
        enum NewIDType {
            New = 0,
        };

        uint16_t mID; // off+0

        static std::atomic<uint16_t> count;

        constexpr typeid_t() = default;
        constexpr typeid_t(const typeid_t &other) : mID(other.mID) {}
        constexpr typeid_t &operator=(const typeid_t &other) {
            mID = other.mID;
            return *this;
        }
        constexpr auto     operator<=>(const typeid_t &) const = default;
        constexpr uint16_t value() const { return mID; }

        typeid_t(NewIDType) :
            mID(count.fetch_add(1, std::memory_order::release)) {}
    };
    static_assert(!std::is_trivially_copy_constructible_v<typeid_t<void>>);
    static_assert(!std::is_trivially_copy_assignable_v<typeid_t<void>>);

} // namespace Bedrock

template <typename Namespace, typename T>
Bedrock::typeid_t<Namespace> type_id() {
    static Bedrock::typeid_t<Namespace> id{Bedrock::typeid_t<Namespace>::NewIDType::New};
    return id;
}

template <>
class Bedrock::typeid_t<class CommandRegistry> {
public:
    enum NewIDType {
        New = 0,
    };

    uint16_t mID; // off+0

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "disp:2,move", "\x66\xF0\x0F\xC1\x05\x00\x00\x00\x00\x48\x8D\x0D\x00\x00\x00\x00\x66\x89\x05\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x0F\xB7\x05\x00\x00\x00\x00\x66\x89\x03\x48\x8B\xC3\x48\x83\xC4\x00\x5B\xC3\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24")
    SDK_API static std::atomic<uint16_t> count;

    constexpr typeid_t() = default;
    constexpr typeid_t(const typeid_t &other) : mID(other.mID) {}
    constexpr typeid_t &operator=(const typeid_t &other) {
        mID = other.mID;
        return *this;
    }
    constexpr auto     operator<=>(const typeid_t &) const = default;
    constexpr uint16_t value() const { return mID; }

    typeid_t(NewIDType) :
        mID(count.fetch_add(1, std::memory_order::release)) {}
};
