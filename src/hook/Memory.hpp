#pragma once

#include <cstdint>

namespace memory {
    template <typename T>
    T &memAccess(void *base, uintptr_t offset) {
        return *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(base) + offset);
    }

} // namespace memory