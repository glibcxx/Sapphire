#pragma once

#include <cstdint>

enum class BlockActorType : int;

namespace dragon {

    // size: 24 (1.21.2), 64 (1.21.50/1.21.60)
    struct RenderMetadata {
        const int64_t mUnk0;  // off+0
        uint32_t      mUnk8;  // off+8
        uint32_t      mUnk12; // off+12
        bool          mUnk16; // off+16
#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        std::aligned_storage_t<40, 8> mUnk24; // off+24, std::varaint<std::monostate, {sz: 32, al: 8}, {sz: 32, al: 8}, {sz: 32, al: 8}>
#endif
    };

} // namespace dragon