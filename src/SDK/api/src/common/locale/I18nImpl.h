#pragma once

#include <string>
#include <unordered_map>
#include <memory>
#include <shared_mutex>
#include "SDK/api/src-deps/Core/Utility/Subject.h"
#include "I18n.h"

class Localization;
class ResourcePackManager;
class I18nObserver;

// size: 448
class I18nImpl : public I18n {
public:
    static void *const *__vftable; // \x48\x8D\x05\x00\x00\x00\x00\x48\x89\x05\x00\x00\x00\x00\x48\x8D\x4D

    struct Unk24 {
        std::shared_mutex mMutex;   // off+0
        uint64_t          _fill[4]; // off+8
    };

    std::shared_ptr<Localization>                             mEmptyLanguage;                     // off+8
    Unk24                                                     mUnk24;                             // off+24
    std::vector<std::shared_ptr<Localization>>                mLanguages;                         // off+64
    std::vector<std::string>                                  mLanguageCodes;                     // off+88
    std::unordered_map<std::string, std::string>              mLanguageNames;                     // off+112
    std::vector<std::shared_ptr<Localization>>                mPackKeywordLanguages;              // off+176
    std::vector<std::string>                                  mPackReservedKeys;                  // off+200
    std::unordered_map<std::string, std::vector<std::string>> mFormatDictionary;                  // off+224
    bool                                                      mLanguageSupportsHypenSplitting;    // off+288
    ResourcePackManager                                      *mResourcePackManager;               // off+296
    std::shared_mutex                                         mAdditionalTranslationsBackupMutex; // off+304
    std::vector<Localization>                                 mAdditionalTranslationsBackup;      // off+312
    std::shared_mutex                                         mUnk336;                            // off+336
    Core::Subject<I18nObserver, std::mutex>                   mSubject;                           // off+344
};
static_assert(sizeof(I18nImpl) == 448);