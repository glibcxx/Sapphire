#pragma once

#include "pch.h" // IWYU pragma: keep

#include "common/DowncastImpl.h"
#include "common/SemanticVersion.hpp"
#include "common/sys/MiniWindows.h"

namespace sapphire::platform {

    class EnvironmentImpl;

    class Environment : public DowncastImpl<Environment, EnvironmentImpl> {
    public:
        SPHR_API static Environment &getInstance();

        const SemanticVersion &getSapphireVersion() const { return mSapphireVersion; }
        SPHR_API std::string_view getSapphireVersionStr() const noexcept;

        const SemanticVersion &getGameVersion() const { return mGameVersion; }
        std::string_view       getGameVersionStr() const { return mGameVersionStr; }

        const std::filesystem::path &getSapphireHomePath() const { return mSapphireHomePath; }
        const std::filesystem::path &getSapphireCorePath() const { return mSapphireCorePath; }
        const std::filesystem::path &getSapphireBinPath() const { return mSapphireBinPath; }

        sys::win::hmodule_t getGameModule() const { return mGameModule; }
        sys::win::hmodule_t getSapphireModule() const { return mSapphireModule; }

        sys::win::hwnd_t getMainWindow() const { return mMainWindow; }

        uintptr_t getImagebase() const { return (uintptr_t)mGameModule; }

        void setMainWindow(sys::win::hwnd_t hwnd);

    protected:
        Environment() = default;
        Environment(const Environment &) = delete;
        Environment &operator=(const Environment &) = delete;

        sys::win::hwnd_t mMainWindow = nullptr;

        SemanticVersion mSapphireVersion;
        SemanticVersion mGameVersion;
        std::string     mGameVersionStr;

        std::filesystem::path mSapphireHomePath;
        std::filesystem::path mSapphireCorePath;
        std::filesystem::path mSapphireBinPath;

        sys::win::hmodule_t mGameModule = nullptr;
        sys::win::hmodule_t mSapphireModule = nullptr;
    };

} // namespace sapphire::platform