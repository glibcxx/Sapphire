#pragma once

#include "SDK/api/src-external/RenderDragon/Mesh/BgfxMemoryHelpers.h"

namespace dragon::memory {

    class BufferSource {
    public:
        // size: 1
        struct LifetimeToken {};

        using LifetimePtr = std::shared_ptr<LifetimeToken>;
        using LifetimeWeakPtr = std::weak_ptr<LifetimeToken>;

        // size: 32
        struct UseTracker {
            const std::shared_ptr<const std::vector<unsigned char> > mOwnedMemory; // off+0
            const std::weak_ptr<LifetimeToken>                       mLifetimePtr; // off+16

            // vtb+0
            ~UseTracker();
        };
    };

    template <>
    struct BgfxDataWrapper<BufferSource::UseTracker> {
        const std::shared_ptr<BufferSource::UseTracker> mData; // off+0

        SDK_API static void _bgfxDeleter(void *a1, void *userData);
    };

} // namespace dragon::memory