#pragma once

#include <gsl/gsl>
#include "EnableNonOwnerReferences.h"

namespace Bedrock {

    template <typename T>
    class NonOwnerPointer {
    public:
        std::shared_ptr<EnableNonOwnerReferences::ControlBlock> mControlBlock; // off+0
#if MC_VERSION >= v1_21_50
        T *mPtr; // off+16
#endif

        operator bool() const {
            return access();
        }

        T *access() const {
#if MC_VERSION == v1_21_2
            return mControlBlock ? static_cast<T *>(mControlBlock->mPtr) : nullptr;
#elif MC_VERSION >= v1_21_50
            return mControlBlock && mControlBlock->mIsValid ? mPtr : nullptr;
#endif
        }

        T *operator->() const {
            return access();
        }

        T &operator*() const {
            return *access();
        }

        bool operator==(const NonOwnerPointer &other) const {
            return access() == other.access();
        }

        bool operator==(nullptr_t) const {
            return access() == nullptr;
        }
    };

    template <typename T>
    using NotNullNonOwnerPtr = gsl::not_null<NonOwnerPointer<T>>;

} // namespace Bedrock