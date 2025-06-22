#pragma once

#include <map>
#include <list>
#include <vector>
#include <mutex>
#include "RegistryTraits.h"

namespace dragon::mesh {

    template <typename T1, typename T2>
    class SplitMergeBasedSpanRegistry {
    public:
        using Impl = dragon::mesh::RegistryTraits<T1>;
        using THandle = typename dragon::mesh::RegistryTraits<T1>::THandle;

        struct Page {};
        struct Span {};

        uint64_t            fill[5];              // off+0
        std::mutex          mAccessLock;          // off+40
        std::atomic<size_t> mBytesInUse;          // off+120
        std::atomic<size_t> mTotalBytesAllocated; // off+128

        typename Impl::TContext mContext; // off+136
    };

} // namespace dragon::mesh