#pragma once

#include "pch.h" // IWYU pragma: keep

namespace dragon {

    struct CreateMaterialTransaction {
        std::string                                  mMaterialName; // off+0
        std::unordered_map<std::string, std::string> mUnk32;        // off+32
        uint64_t                                     mUnk96[3];     // off+96
        uint8_t                                      mUnk120;       // off+120
    };

} // namespace dragon