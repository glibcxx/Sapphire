#pragma once

#include <gsl/gsl>
#include "EnableNonOwnerReferences.h"

namespace Bedrock {

    // size: 16 (1.21.2), 24 (1.21.50+)
    template <typename T>
    class NonOwnerPointer {
    public:
        std::shared_ptr<EnableNonOwnerReferences::ControlBlock> mControlBlock; // off+0
#if MC_VERSION >= v1_21_50
        T *mPtr = nullptr; // off+16
#endif

        NonOwnerPointer() = default;

        NonOwnerPointer(std::shared_ptr<EnableNonOwnerReferences::ControlBlock> cb, T *ptr) :
            mControlBlock(std::move(cb))
#if MC_VERSION >= v1_21_50
            ,
            mPtr(ptr)
#endif
        {
        }

        bool isValid() const {
#if MC_VERSION == v1_21_2
            return mControlBlock && mControlBlock->mPtr;
#elif MC_VERSION >= v1_21_50
            return mControlBlock && mControlBlock->mIsValid;
#endif
        }

        operator bool() const { return isValid(); }

        T *access() const {
#if MC_VERSION == v1_21_2
            return isValid() ? static_cast<T *>(mControlBlock->mPtr) : nullptr;
#elif MC_VERSION >= v1_21_50
            return isValid() ? mPtr : nullptr;
#endif
        }

        T *operator->() const { return access(); }

        T &operator*() const { return *access(); }

        bool operator==(const NonOwnerPointer &other) const { return access() == other.access(); }

        bool operator==(nullptr_t) const { return isValid(); }
    };

    template <typename T>
    using NotNullNonOwnerPtr = gsl::not_null<NonOwnerPointer<T>>;

} // namespace Bedrock