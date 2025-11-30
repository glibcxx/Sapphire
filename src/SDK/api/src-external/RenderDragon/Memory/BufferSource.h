#pragma once

#include "SDK/api/src-external/RenderDragon/Mesh/BgfxMemoryHelpers.h"

namespace dragon::memory {

    // size: 48
    class BufferSource {
    public:
        // size: 1
        struct LifetimeToken {};

        using LifetimePtr = std::shared_ptr<LifetimeToken>;
        using LifetimeWeakPtr = std::weak_ptr<LifetimeToken>;

        // size: 32
        struct UseTracker {
            const std::shared_ptr<const std::vector<unsigned char>> mOwnedMemory; // off+0
            const std::weak_ptr<LifetimeToken>                      mLifetimePtr; // off+16

            // vtb+0
            ~UseTracker();
        };

        std::variant<
            std::monostate,
            std::shared_ptr<const std::vector<unsigned char>>,
            std::pair<gsl::span<const unsigned char>, LifetimeWeakPtr>>
                 mData; // off+0
        uint64_t mSize; // off+40
    };

    template <>
    struct BgfxDataWrapper<BufferSource::UseTracker> {
        const std::shared_ptr<BufferSource::UseTracker> mData; // off+0

        SAPPHIRE_API("1.21.50", "\x48\x85\xD2\x74\x00\x57\x48\x83\xEC") // 1.21.50
        SDK_API static void _bgfxDeleter(void *a1, void *userData);
    };

} // namespace dragon::memory