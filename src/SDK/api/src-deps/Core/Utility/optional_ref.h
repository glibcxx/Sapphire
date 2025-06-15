#pragma once

#include <cstddef>

// size: 8
template <typename T>
class optional_ref {
public:
    optional_ref() = default;

    optional_ref(const T &t) :
        ptr(&t) {}

    optional_ref(std::nullptr_t) {}

    T &unwrap() const { return *ptr; }

    bool operator==(T &rhs) const { return *ptr == rhs; }
    bool operator!=(T &rhs) const { return *ptr != rhs; }

    bool operator!=(const optional_ref<T> &rhs) const {
        return *ptr != rhs.unwrap();
    }

    operator T &() const { return *ptr; }
    operator const T &() const { return *ptr; }
    T &operator*() const { return *ptr; }
    T *operator->() const { return ptr; }

    T *ptr = nullptr; // off+0
};