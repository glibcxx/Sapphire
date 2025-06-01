#pragma once

#include <memory>

template <typename T>
class LinearAllocator {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using void_pointer = void *;
    using const_void_pointer = const void *;
    using reference = T &;
    using const_reference = const T &;

    std::shared_ptr<T> mData;
};