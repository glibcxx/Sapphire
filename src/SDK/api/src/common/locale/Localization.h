#pragma once

#include <string>
#include <map>
#include <memory>
#include <shared_mutex>

// size: 112 (1.21.2), 120 (1.21.50/1.21.60)
class Localization {
public:
    bool                               mCommaSeperator;          // off+0
    std::string                        mDigitGroupSeparator;     // off+8
    const std::string                  mCode;                    // off+40
    std::map<std::string, std::string> mStrings;                 // off+72
    std::map<std::string, std::string> mCaseSensitiveCache;      // off+88
    std::shared_mutex                  mCaseSensitiveCacheMutex; // off+104

#if MC_VERSION >= v1_21_50
    bool mMayContainIdentifier; // off+112
#endif

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x88\x07\x48\x8D\x4F\x00\x48\x8B\xD3\xE8\x00\x00\x00\x00\x90\x48\x8D\x4F\x00\x0F\x57\xC0\x0F\x11\x01\x33\xF6")
    SPHR_DECL_API("1.21.50", "call", "\xE8\x00\x00\x00\x00\x88\x07\x48\x8D\x4F\x00\x48\x8B\xD3")
    SPHR_DECL_API("1.21.60", "call", "\xE8\x00\x00\x00\x00\x88\x07\x48\x8D\x4F\x00\x49\x8B\xD7")
    SDK_API static bool _isCommaSeperatedLanguage(const std::string &langCode);
};
