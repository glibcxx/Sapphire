#pragma once

#include <bitset>
#include <glm/glm.hpp>
#include "../Materials/Definition/MaterialShared.h"

namespace dragon::materials {
    enum class ShaderCodePlatform : uint8_t;
}

namespace dragon::platform {

    // off+264 (1.21.50)
    struct GraphicsPlatform {
        std::bitset<8>                        mUnk0;                   // off+0
        dragon::materials::ShaderCodePlatform mShaderPlatform;         // off+1
        std::string                           mChipsetName;            // off+8
        std::string                           mChipsetVendorName;      // off+40
        std::string                           mGraphicsVersion;        // off+72
        std::string                           mGraphicsFeatureVersion; // off+104
        std::string                           mUnk136;                 // off+136
        std::string                           mUnk168;                 // off+168
        glm::tvec2<uint32_t>                  mScreenDimensions;       // off+104
        uint32_t                              mVendorId;               // off+112
        uint32_t                              mDeviceId;               // off+116
        uint32_t                              mSubSysId;               // off+120
        uint32_t                              mRevision;               // off+124
        uint64_t                              mDedicatedVideoMemory;   // off+224
        uint64_t                              mDedicatedSystemMemory;  // off+232
        uint64_t                              mSharedSystemMemory;     // off+240
        uint64_t                              mReservedVideoMemory;    // off+248
        uint8_t                               mUnk256;                 // off+256

        SDK_API GraphicsPlatform(GraphicsPlatform &&that);

        SDK_API GraphicsPlatform(const GraphicsPlatform &that);

        SPHR_DECL_API("1.21.50", "call", "\x0F\xB6\x02\x0F\x57\xC0\x88\x01\x0F\xB6\x42\x00\x88\x41\x00\x33\xC0")
        SPHR_CTOR_ALIAS SDK_API GraphicsPlatform *ctor(GraphicsPlatform &&that);

        SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x90\xBB\x00\x00\x00\x00\xEB\x00\x48\x8D\x4C\x24\x00\xE8\x00\x00\x00\x00\x90\xBB\x00\x00\x00\x00\x89\x5C\x24\x00\x48\x8B\xD0\x48\x8B\xCE")
        SPHR_CTOR_ALIAS SDK_API GraphicsPlatform *ctor(const GraphicsPlatform &that);
    };

} // namespace dragon::platform
