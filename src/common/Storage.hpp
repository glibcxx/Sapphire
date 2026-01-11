#pragma once

#include <cstddef>
#include <memory>
#include <new>
#include <type_traits>
#include <utility>

namespace sapphire {

    template <typename T, std::size_t Size = sizeof(T), std::size_t Align = alignof(T)>
    class Storage {
        alignas(Align) std::byte mStorage[Size];

    public:
        Storage() = default;

        template <typename... Args>
        Storage(Args &&...args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
            new (&mStorage) T{std::forward<Args>(args)...};
        }

        template <typename... Args>
        T &emplace(Args &&...args) noexcept(std::is_nothrow_constructible_v<T, Args...>) {
            new (&mStorage) T{std::forward<Args>(args)...};
        }

        T       *get() noexcept { return std::launder(reinterpret_cast<T *>(&mStorage)); }
        const T *get() const noexcept { return std::launder(reinterpret_cast<T *>(&mStorage)); }

        T       &value() noexcept { return *std::launder(reinterpret_cast<T *>(&mStorage)); }
        const T &value() const noexcept { return *std::launder(reinterpret_cast<T *>(&mStorage)); }

        operator T &() noexcept { return value(); }
        operator const T &() const noexcept { return value(); }

        T       &operator*() noexcept { return value(); }
        const T &operator*() const noexcept { return value(); }

        T       *operator->() noexcept { return get(); }
        const T *operator->() const noexcept { return get(); }
    };

} // namespace sapphire