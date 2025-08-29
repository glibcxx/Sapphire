#pragma once

namespace bx {

    template <uint32_t MaxCapacityT, typename KeyT = uint32_t>
    class HandleHashMapT {
    public:
        struct Iterator {
            uint16_t handle;
            uint32_t pos;
            uint32_t num;
        };

        uint32_t m_maxCapacity;
        uint32_t m_numElements;

        KeyT     m_key[MaxCapacityT];
        uint16_t m_handle[MaxCapacityT];
    };

} // namespace bx