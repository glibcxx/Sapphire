#pragma once

#include <string>

// size: 56
struct ToastIconData {
    struct Unk8 {
        uint64_t    mUnk0;  // off+0
        std::string mUnk8;  // off+8
        uint64_t    mUnk40; // off+40
    };
    uint32_t mUnk0; // off+0
    Unk8     mUnk8; // off+8
};
static_assert(sizeof(ToastIconData) == 56);