#pragma once

#include <memory>

template <typename T>
struct GameRefs {
    using OwnerStorage = std::shared_ptr<T>;
    using StackResultStorage = std::shared_ptr<T>;
    using WeakStorage = std::weak_ptr<T>;
    using StackRef = T;
    using OwnerStackRef = StackRef;
};

// todo: Specialization of EntityContext, EntityId...
