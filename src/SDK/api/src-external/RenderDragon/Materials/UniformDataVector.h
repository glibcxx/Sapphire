#pragma once

#include <cstdint>

namespace dragon::materials {

    // size: 16
    template <typename T>
    class UniformDataVector {
    public:
        uint8_t *mDataPtr;  // off+0
        uint16_t mCapacity; // off+8
        uint16_t mSize;     // off+10

        bool empty() const {
            return mSize;
        }

        T &operator[](size_t idx) {
            return data()[idx];
        }

        const T &operator[](size_t idx) const {
            return data()[idx];
        }

        T       *data() { return reinterpret_cast<T *>(this->mDataPtr); }
        const T *data() const { return reinterpret_cast<const T *>(this->mDataPtr); }

        size_t size() const { return mSize; }

        size_t capacity() const { return mCapacity; }

        const T *begin() const { return data(); }
        T       *begin() { return data(); }
        const T *end() const { return data() + mSize; }
        T       *end() { return data() + mSize; }
    };

} // namespace dragon::materials