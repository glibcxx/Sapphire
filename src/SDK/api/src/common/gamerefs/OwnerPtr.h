#pragma once

#include <memory>
#include "GameRefs.h"

template <typename Traits>
class WeakRefT;

template <typename Traits>
class OwnerPtrT : public Traits::OwnerStorage {
public:
    using OwnerStackRef = typename Traits::OwnerStackRef;

    OwnerPtrT() :
        Traits::OwnerStorage(nullptr) {}

    OwnerPtrT(const OwnerPtrT &other) :
        Traits::OwnerStorage(other) {}

    OwnerPtrT(OwnerPtrT &&other) :
        Traits::OwnerStorage(std::move(other)) {}

    ~OwnerPtrT() {}

    OwnerPtrT &operator=(const OwnerPtrT &other) {
        return Traits::OwnerStorage::operator=(other);
    }

    OwnerPtrT &operator=(OwnerPtrT &&other) {
        return Traits::OwnerStorage::operator=(std::move(other));
    }
};

template <typename T>
using OwnerPtr = OwnerPtrT<GameRefs<T>>;
