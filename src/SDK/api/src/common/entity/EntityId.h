#pragma once

#include <entt/entt.hpp>

// size: 4
class EntityId {
public:
    constexpr EntityId(const EntityId &) = default;

    constexpr EntityId(EntityId &&) = default;

    constexpr EntityId(uint32_t entity = 0) :
        mRawId(entity) {}

    constexpr EntityId &operator=(const EntityId &other) = default;

    constexpr EntityId &operator=(EntityId &&other) = default;

    constexpr bool operator==(const EntityId &rhs) const { return this->mRawId == rhs.mRawId; }

    constexpr bool operator!=(const EntityId &rhs) const { return this->mRawId != rhs.mRawId; }

    constexpr operator uint32_t &() { return this->mRawId; }

    constexpr operator const uint32_t() const { return this->mRawId; }

    uint32_t mRawId = 0;
};

struct EntityIdTraits {
    using value_type = EntityId;

    using entity_type  = uint32_t;
    using version_type = uint16_t;

    static constexpr entity_type entity_mask  = 0x3FFFF;
    static constexpr entity_type version_mask = 0x3FFF;
};

template <>
struct entt::entt_traits<EntityId>: public entt::basic_entt_traits<EntityIdTraits> {
    static constexpr std::size_t page_size = ENTT_SPARSE_PAGE;
};
