#pragma once

#include <string>
#include "SDK/api/src-deps/Core/String/StaticOptimizedString.h"

#if MC_VERSION <= v1_21_50
// size: 112
class SemVersion {
public:
    enum class ParseOption : int {
        AllowAnyVersion = 0,
        NoAnyVersion = 1,
    };

    struct any_version_constructor {};

    enum class MatchType : int {
        Full = 0,
        Partial = 1,
        None = 2,
    };

    uint16_t    mMajor;             // off+0
    uint16_t    mMinor;             // off+2
    uint16_t    mPatch;             // off+4
    std::string mPreRelease;        // off+8
    std::string mBuildMeta;         // off+40
    std::string mFullVersionString; // off+72
    bool        mValidVersion;      // off+104
    bool        mAnyVersion;        // off+105
};
static_assert(sizeof(SemVersion) == 112);
#elif MC_VERSION >= v1_21_60
// size: 24
class SemVersion {
    uint16_t                       mMajor;      // off+0
    uint16_t                       mMinor;      // off+2
    uint16_t                       mPatch;      // off+4
    Bedrock::StaticOptimizedString mPreRelease; // off+8
    Bedrock::StaticOptimizedString mBuildMeta;  // off+16
};
static_assert(sizeof(SemVersion) == 24);
#endif