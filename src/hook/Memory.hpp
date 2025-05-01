#pragma once

#include <cstdint>

namespace memory {
    template <typename T>
    T &memAccess(void *base, uintptr_t offset) {
        return *reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(base) + offset);
    }

    template <typename T = void **>
    T getVftable(void *obj) {
        return std::bit_cast<T>(*reinterpret_cast<void ***>(obj));
    }

    template <typename T = void **>
    T getVftable(const void *obj) {
        return std::bit_cast<T>(*reinterpret_cast<void ***>(const_cast<void *>(obj)));
    }

    template <typename Ret = void, typename... Args>
    Ret vCall(void *obj, size_t fnIdx, Args &&...args) {
        return reinterpret_cast<Ret (*)(void *, Args &&...)>(
            getVftable(obj)[fnIdx]
        )(obj, std::forward<Args>(args)...);
    }

    template <typename Ret = void, typename... Args>
    Ret vCall(const void *obj, size_t fnIdx, Args &&...args) {
        return reinterpret_cast<Ret (*)(const void *, Args &&...)>(
            getVftable(obj)[fnIdx]
        )(obj, std::forward<Args>(args)...);
    }

    template <typename Ret = void, typename... Args>
    Ret vCallOffset(void *obj, intptr_t offset, size_t fnIdx, Args &&...args) {
        return reinterpret_cast<Ret (*)(void *, Args &&...)>(
            getVftable(reinterpret_cast<void *>(reinterpret_cast<uintptr_t>(obj) + offset))[fnIdx]
        )(obj, std::forward<Args>(args)...);
    }

} // namespace memory