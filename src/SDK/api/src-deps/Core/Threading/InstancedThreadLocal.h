#pragma once

#include "SDK/api/src-deps/Core/Container/IntrusiveList.h"
#include <type_traits>
#include <memory>

#if defined(_WIN32)
typedef unsigned long DWORD;
extern "C" {
__declspec(dllimport) PVOID __stdcall FlsGetValue(DWORD dwFlsIndex);
__declspec(dllimport) BOOL __stdcall FlsSetValue(DWORD dwFlsIndex, PVOID lpFlsData);
}
#else
#    error "InstancedThreadLocal currently supports Windows TLS only"
#endif

namespace Bedrock::Threading {

    class InstancedThreadLocalBase {
    };

    // size: 168
    template <typename T, typename Allocator = std::allocator<T>>
    class InstancedThreadLocal : public InstancedThreadLocalBase {
    public:
        struct Item : public Bedrock::Intrusive::list_base_hook<void> {
            std::aligned_storage_t<sizeof(T), alignof(T)> mData; // off+16

            T *data() {
                return std::launder(reinterpret_cast<T *>(&mData));
            }
        };

        using ItemList = Bedrock::Intrusive::list<Item>;

        using ItemIterator = typename ItemList::iterator;

        using MutexType = std::mutex;
        using LockType = std::lock_guard<std::mutex>;
        using AllocatorType = std::allocator<Item>;
        using Constructor = std::function<void(T *)>;
        using IteratorType = ItemIterator;

        Constructor mConstructor; // off+0
        ItemList    mItems;       // off+64
        MutexType   mMutex;       // off+80
        uint32_t    mKey;         // off+160
        bool        mInitialized; // off+164

        T &get() { return *_load(); }

        T &operator*() { return *_load(); }

        T *operator->() { return _load(); }

        T *_load() { // 1.21.50 \xE8\x00\x00\x00\x00\x0F\x57\xC0\x48\x8D\x4D\x00\x33\xD2
            Item *item = static_cast<Item *>(FlsGetValue(this->mKey));
            if (!item) {
                AllocatorType alloc{};
                item = std::allocator_traits<AllocatorType>::allocate(alloc, 1);
                this->mConstructor(item->data());
                FlsSetValue(this->mKey, item);
                LockType lock(this->mMutex);
                this->mItems.push_back(*item);
            }
            return item->data();
        }
    };
} // namespace Bedrock::Threading