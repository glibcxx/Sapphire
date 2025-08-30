#pragma once

namespace bx {

    constexpr uint16_t kInvalidHandle = UINT16_MAX;

    // https://github.com/bkaradzic/bx/blob/eda38395ddfdeba08c46653b93b8d5bb21f2c863/include/bx/handlealloc.h
    class HandleAlloc {
    public:
        HandleAlloc(uint16_t _maxHandles) :
            m_numHandles(0), m_maxHandles(_maxHandles) { reset(); }

        const uint16_t *getHandles() const { return getDensePtr(); }

        uint16_t getHandleAt(uint16_t _at) const { return getDensePtr()[_at]; }

        uint16_t getNumHandles() const { return m_numHandles; }

        uint16_t getMaxHandles() const { return m_maxHandles; }

        uint16_t alloc() {
            if (m_numHandles < m_maxHandles) {
                uint16_t index = m_numHandles;
                ++m_numHandles;

                uint16_t *dense = getDensePtr();
                uint16_t  handle = dense[index];
                uint16_t *sparse = getSparsePtr();
                sparse[handle] = index;
                return handle;
            }

            return kInvalidHandle;
        }

        bool isValid(uint16_t _handle) const {
            uint16_t *dense = getDensePtr();
            uint16_t *sparse = getSparsePtr();
            uint16_t  index = sparse[_handle];

            return index < m_numHandles
                && dense[index] == _handle;
        }

        void free(uint16_t _handle) {
            uint16_t *dense = getDensePtr();
            uint16_t *sparse = getSparsePtr();
            uint16_t  index = sparse[_handle];
            --m_numHandles;
            uint16_t temp = dense[m_numHandles];
            dense[m_numHandles] = _handle;
            sparse[temp] = index;
            dense[index] = temp;
        }

        void reset() {
            m_numHandles = 0;
            uint16_t *dense = getDensePtr();
            for (uint16_t ii = 0, num = m_maxHandles; ii < num; ++ii) {
                dense[ii] = ii;
            }
        }

    private:
        HandleAlloc();

        uint16_t *getDensePtr() const {
            uint8_t *ptr = (uint8_t *)reinterpret_cast<const uint8_t *>(this);
            return (uint16_t *)&ptr[sizeof(HandleAlloc)];
        }

        uint16_t *getSparsePtr() const {
            return &getDensePtr()[m_maxHandles];
        }

        uint16_t m_numHandles;
        uint16_t m_maxHandles;
    };

    template <uint16_t MaxHandlesT>
    class HandleAllocT : public HandleAlloc {
    public:
        uint16_t m_padding[2 * MaxHandlesT];
    };

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