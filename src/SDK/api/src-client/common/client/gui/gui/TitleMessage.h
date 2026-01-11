#pragma once

#include <string>
#include <optional>
#include "macros/Version.h"

#if MC_VERSION == v1_21_2
// size: 112 (1.21.2)
class TitleMessage {
public:
    std::string mTitle;            // off+0
    std::string mSubtitle;         // off+72
    int32_t     mFadeInTime;       // off+64
    int32_t     mStayTime;         // off+68
    int32_t     mFadeOutTime;      // off+72
    std::string mActionBarMessage; // off+80
};
static_assert(sizeof(TitleMessage) == 112);
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
// size: 232 (1.21.50/1.21.60)
class TitleMessage {
public:
    std::string                mTitle;            // off+0
    std::optional<std::string> mUnk32;            // off+32
    std::string                mSubtitle;         // off+72
    std::optional<std::string> mUnk104;           // off+104
    int32_t                    mFadeInTime;       // off+144
    int32_t                    mStayTime;         // off+148
    int32_t                    mFadeOutTime;      // off+152
    std::string                mActionBarMessage; // off+160
    std::optional<std::string> mUnk192;           // off+192
};
static_assert(sizeof(TitleMessage) == 232);
#endif