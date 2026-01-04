#pragma once

#include <cstddef>
#include <type_traits>

// size: 8
template <typename T>
    requires(!std::is_reference_v<T>)
class optional_ref {
public:
    T *ptr = nullptr; // off+0

    constexpr optional_ref() = default;
    constexpr optional_ref(const T &t) :
        ptr(&t) {}

    constexpr optional_ref(std::nullptr_t) {}

    constexpr T &unwrap() const { return *ptr; }

    constexpr bool operator==(T &rhs) const { return *ptr == rhs; }
    constexpr bool operator!=(T &rhs) const { return *ptr != rhs; }
    constexpr bool operator!=(const optional_ref &rhs) const {
        return *ptr != rhs.unwrap();
    }
    constexpr    operator T &() const { return *ptr; }
    constexpr T &operator*() const { return *ptr; }
    constexpr T *operator->() const { return ptr; }
};