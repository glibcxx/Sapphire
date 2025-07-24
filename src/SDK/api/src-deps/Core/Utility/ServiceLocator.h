#pragma once

#include <shared_mutex>
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src-deps/Platform/Threading/PrioritizeSharedOwnership.h"

template <typename Service>
class ServiceReference {
public:
    std::shared_lock<Bedrock::Threading::PrioritizeSharedOwnership> mLock;    // off+0
    Bedrock::NonOwnerPointer<Service>                               mService; // off+16
};

template <typename Service>
class ServiceLocator {
public:
    static ServiceReference<Service> get() {
        static_assert(false, "defualt ServiceLocator::get() is not implemented!");
    }
};