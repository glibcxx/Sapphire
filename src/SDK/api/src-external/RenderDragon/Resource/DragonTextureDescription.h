#pragma once

#include <string>
#include <bitset>
#include "SDK/api/src-deps/Coregraphics/TextureDescription.h"

namespace dragon {

    // size: 4
    class TextureUsage : public std::bitset<4> {
    };

    // size: 64
    class TextureDescription : public cg::TextureDescription {
    public:
        std::string          mIdentifier; // off+24
        dragon::TextureUsage mUsage;      // off+56
        uint8_t              mUnk60;      // off+60
    };
    static_assert(sizeof(TextureDescription) == 64);

} // namespace dragon
