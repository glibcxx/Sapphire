#pragma once

#include <functional>
#include <mutex>

namespace Bedrock::Intrusive {

    // size: 1
    template <typename T, typename U>
    class list_standard_operations {
    };

    // size: 16
    template <typename T>
    class list_base_hook : public list_standard_operations<list_base_hook<T>, list_base_hook<T>> {
    public:
        list_base_hook *mNext; // off+0
        list_base_hook *mPrev; // off+8
    };

    // size: 16
    template <typename Val, typename Hook = list_base_hook<void>, typename Traits = list_base_hook<void>>
    class list {
    public:
        using value_type = Val;
        using pointer_type = Val *;
        using const_pointer_type = const Val *;
        using reference_type = Val &;
        using const_reference_type = const Val &;
        using hook_type = Hook;
        using traits_type = Traits;

        // size: 8
        template <bool IsConst>
        class iterator_base {
            using iterator_category = std::bidirectional_iterator_tag;
            using ihook_type = std::conditional_t<IsConst, const hook_type, hook_type>;

            iterator_base(ihook_type &hook) :
                mHook(hook) {}

            ihook_type    &operator*() const { return *mHook; }
            ihook_type    *operator->() const { return mHook; }
            iterator_base &operator++() {
                mHook = mHook->mNext;
                return *this;
            }
            iterator_base  operator++(int) { return mHook->mNext; }
            iterator_base &operator--() {
                mHook = mHook->mPrev;
                return --mHook;
            }
            iterator_base operator--(int) { return mHook->mPrev; }

            ihook_type *mHook; // off+0
        };

        using iterator = iterator_base<false>;
        using const_iterator = iterator_base<true>;

        void push_back(Val &val) {
            val.mPrev = mSentinel.mPrev;
            val.mNext = &mSentinel;
            mSentinel.mPrev->mNext = &val;
            mSentinel.mPrev = &val;
        }

        list_base_hook<void> mSentinel; // off+0
    };

} // namespace Bedrock::Intrusive