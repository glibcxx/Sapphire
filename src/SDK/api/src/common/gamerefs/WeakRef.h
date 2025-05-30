#pragma once

#include "StackRefResult.h"

template <typename Traits>
class OwnerPtrT;

template <typename Traits>
class WeakRefT : public Traits::WeakStorage {
public:
    WeakRefT(std::nullptr_t) :
        Traits::WeakStorage(nullptr) {}

    WeakRefT(const WeakRefT &other) :
        Traits::WeakStorage(other) {}

    WeakRefT(WeakRefT &&other) :
        Traits::WeakStorage(std::move(other)) {}

    ~WeakRefT() {}

    WeakRefT &operator=(const WeakRefT &other) {
        return Traits::WeakStorage::operator=(other);
    }

    WeakRefT &operator=(WeakRefT &&other) {
        return Traits::WeakStorage::operator=(std::move<Traits::WeakStorage>(other));
    }

    StackRefResultT<Traits> unwrap() const { return *this; }
};

template <typename T>
using WeakRef = WeakRefT<GameRefs<T>>;