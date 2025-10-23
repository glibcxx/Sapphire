#pragma once

#include "SDK/api/src-deps/Core/Utility/Observer.h"

class PackManifest;

// size: 16
class I18nObserver : public Core::Observer<I18nObserver, Core::SingleThreadedLock> {
public:
    // vtb+0
    virtual ~I18nObserver() override;

    // vtb+2
    virtual void onLanguageChanged(const std::string &code, bool languageSystemInitializing) = 0;

    // vtb+3
    virtual void onLanguageKeywordsLoadedFromPack(const PackManifest &manifest) = 0;

    // vtb+4
    virtual void onLanguagesLoaded() = 0;
};