#pragma once

#include <string>
#include <map>
#include <memory>
#include <mutex>

// size: 120 (1.21.50)
class Localization {
public:
    bool                               mCommaSeperator; // off+0
    const std::string                  mCode;           // off+8
    std::string                        mUnk48;          // off+40
    std::map<std::string, std::string> mStrings;        // off+72
    std::shared_ptr<std::mutex>        m_lock;          // off+88
    uint64_t                           mUnk104;         // off+104
    bool                               mUnk112;         // off+112

    Localization *ctor(const std::string &code, Localization *fallbackLocale); // \x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x4C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x49\x8B\xE8\x48\x8B\xDA
};
static_assert(sizeof(Localization) == 120);