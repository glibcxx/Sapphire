#pragma once

namespace A {
    class B;
}

namespace sapphire::platform {

    class Environment {
    public:
        static Environment &getInstance();

        const std::filesystem::path &getSapphireHomePath() const { return mSapphireHomePath; }
        const std::filesystem::path &getSapphireCorePath() const { return mSapphireCorePath; }
        const std::filesystem::path &getSapphireBinPath() const { return mSapphireBinPath; }

        HMODULE getGameModule() const { return mGameModule; }
        HMODULE getSapphireModule() const { return mSapphireModule; }

        HWND getMainWindow() const { return mMainWindow; }

        void setMainWindow(HWND hwnd) { mMainWindow = hwnd; }

        uintptr_t getImagebase() const { return (uintptr_t)mGameModule; }

    private:
        Environment();
        ~Environment() = default;

        Environment(const Environment &) = delete;
        Environment &operator=(const Environment &) = delete;

        HWND mMainWindow = nullptr;

        std::filesystem::path mSapphireHomePath;
        std::filesystem::path mSapphireCorePath;
        std::filesystem::path mSapphireBinPath;

        HMODULE mGameModule = nullptr;
        HMODULE mSapphireModule = nullptr;

        MODULEINFO mSapphireModuleInfo;
    };

} // namespace sapphire::platform