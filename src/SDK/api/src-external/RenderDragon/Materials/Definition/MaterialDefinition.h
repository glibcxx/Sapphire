#pragma once

#include <string>
#include "MaterialShared.h"

namespace dragon::materials::definition {

    // size: 184
    class SamplerDefinition {
    public:
        // size: 40
        struct CustomTypeInfo {
            std::string customType;       // off+0
            uint32_t    customTypeStride; // off+32
        };

        uint16_t                      mSlot;                 // off+0
        uint8_t                       mUnk1;                 // off+1
        uint32_t                      mUnk4;                 // off+4
        SamplerAccess                 mAccess;               // off+8
        PrecisionConstraint           mPrecision;            // off+9
        bool                          mAllowUnorderedAccess; // off+10
        SamplerType                   mType;                 // off+11
        std::string                   mFormat;               // off+16
        std::optional<uint8_t>        mUnk48;                // off+48
        std::optional<std::string>    mDefaultTexture;       // off+56
        std::optional<std::string>    mUnk96;                // off+96
        std::optional<CustomTypeInfo> mCustomType;           // off+136
    };

    struct CustomTypeDeclaration {};

} // namespace dragon::materials::definition