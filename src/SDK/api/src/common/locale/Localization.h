#pragma once

#include <string>
#include <map>
#include <memory>
#include <shared_mutex>

// size: 112 (1.21.2), 120 (1.21.50/1.21.60)
class Localization {
public:
    bool                               mCommaSeperator;      // off+0
    std::string                        mDigitGroupSeparator; // off+8
    const std::string                  mCode;                // off+40
    std::map<std::string, std::string> mStrings;             // off+72
    std::map<std::string, std::string> mUnk88;               // off+88
    std::shared_mutex                  mLock;                // off+104

#if MC_VERSION >= v1_21_50
    bool mUnk112; // off+112
#endif

    Localization *ctor(const std::string &code, Localization *fallbackLocale); // \x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x4C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x49\x8B\xE8\x48\x8B\xDA

    // \xE8\x00\x00\x00\x00\x88\x07\x48\x8D\x4F\x00\x48\x8B\xD3\xE8\x00\x00\x00\x00\x90\x48\x8D\x4F\x00\x0F\x57\xC0\x0F\x11\x01\x33\xF6 (1.21.2)
    // \xE8\x00\x00\x00\x00\x88\x07\x48\x8D\x4F\x00\x48\x8B\xD3 (1.21.50)
    // \xE8\x00\x00\x00\x00\x88\x07\x48\x8D\x4F\x00\x49\x8B\xD7 (1.21.60)
    static bool _isCommaSeperatedLanguage(const std::string &langCode);
};
static_assert(sizeof(Localization) == 120);