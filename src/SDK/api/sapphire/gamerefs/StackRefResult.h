#pragma once

#include <utility>
#include "GameRefs.h"

template <typename Traits>
class WeakRefT;

template <typename Traits>
class StackRefResultT : public Traits::StackResultStorage {
public:
    using StackRef = Traits::StackRef;

    StackRefResultT(std::nullptr_t) :
        Traits::StackResultStorage(nullptr) {}

    StackRefResultT(const WeakRefT<Traits> &weakRef) :
        Traits::StackResultStorage(weakRef) {}

    StackRefResultT(StackRef &value) :
        Traits::StackResultStorage(value) {}

    StackRefResultT(const StackRefResultT &rhs) :
        Traits::StackResultStorage(rhs) {}

    StackRefResultT(StackRefResultT &&rhs) :
        Traits::StackResultStorage(std::move(rhs)) {}

    ~StackRefResultT() {}

    StackRefResultT &operator=(const StackRefResultT &other) {
        return Traits::StackResultStorage::operator=(other);
    }

    StackRefResultT &operator=(StackRefResultT &&other) {
        return Traits::StackResultStorage::operator=(std::move(other));
    }
};

template <typename T>
using StackRefResult = StackRefResultT<GameRefs<T>>;