#pragma once

#include <string>
#include "SDK/core/ApiManager.h"
#include "SDK/api/src-deps/Core/String/StaticOptimizedString.h"

#if MC_VERSION <= v1_21_50
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

    uint16_t    mMajor;
    uint16_t    mMinor;
    uint16_t    mPatch;
    std::string mPreRelease;
    std::string mBuildMeta;
    std::string mFullVersionString;
    bool        mValidVersion;
    bool        mAnyVersion;
};
static_assert(sizeof(SemVersion) == 112);
#elif MC_VERSION >= v1_21_60
class SemVersion {
    uint16_t                       mMajor;
    uint16_t                       mMinor;
    uint16_t                       mPatch;
    Bedrock::StaticOptimizedString mPreRelease;
    Bedrock::StaticOptimizedString mBuildMeta;
};
static_assert(sizeof(SemVersion) == 24);
#endif