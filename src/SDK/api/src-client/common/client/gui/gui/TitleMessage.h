#pragma once

#include <string>
#include <optional>

// size: 232 (1.21.50)
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