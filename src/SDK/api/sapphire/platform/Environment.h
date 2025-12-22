#pragma once

#include "common/SemanticVersion.hpp"

namespace sapphire::platform {

    class Environment {
    public:
        SPHR_API static Environment &getInstance();

        const SemanticVersion &getSapphireVersion() const { return mSapphireVersion; }
        SPHR_API std::string_view getSapphireVersionStr() const noexcept;

        const SemanticVersion &getGameVersion() const { return mGameVersion; }
        std::string_view       getGameVersionStr() const { return mGameVersionStr; }

        const std::filesystem::path &getSapphireHomePath() const { return mSapphireHomePath; }
        const std::filesystem::path &getSapphireCorePath() const { return mSapphireCorePath; }
        const std::filesystem::path &getSapphireBinPath() const { return mSapphireBinPath; }

        HMODULE getGameModule() const { return mGameModule; }
        HMODULE getSapphireModule() const { return mSapphireModule; }

        HWND getMainWindow() const { return mMainWindow; }

        uintptr_t getImagebase() const { return (uintptr_t)mGameModule; }

        void setMainWindow(HWND hwnd);

    private:
        Environment();
        ~Environment() = default;

        Environment(const Environment &) = delete;
        Environment &operator=(const Environment &) = delete;

        HWND mMainWindow = nullptr;

        SemanticVersion mSapphireVersion;
        SemanticVersion mGameVersion;
        std::string     mGameVersionStr;

        std::filesystem::path mSapphireHomePath;
        std::filesystem::path mSapphireCorePath;
        std::filesystem::path mSapphireBinPath;

        HMODULE mGameModule = nullptr;
        HMODULE mSapphireModule = nullptr;

        MODULEINFO mSapphireModuleInfo;
    };

} // namespace sapphire::platform