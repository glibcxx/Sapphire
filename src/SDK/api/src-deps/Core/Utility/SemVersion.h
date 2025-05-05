#pragma once

#include <string>

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