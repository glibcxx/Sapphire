#pragma once

#include "pch.h" // IWYU pragma: keep

#include "ModManifest.h"
#include "SDK/api/sapphire/IMod.h"
#include "common/sys/MiniWindows.h"

namespace sapphire::core {

    class LibraryModule {
        sys::win::hmodule_t mHandle;

        constexpr LibraryModule(sys::win::hmodule_t h) : mHandle(h) {}

    public:
        [[nodiscard]] static LibraryModule create(const std::filesystem::path &libPath);

        constexpr LibraryModule(nullptr_t) : mHandle(nullptr) {}

        ~LibraryModule() noexcept;

        constexpr LibraryModule(LibraryModule &&other) noexcept : mHandle(std::exchange(other.mHandle, {})) {}
        constexpr LibraryModule &operator=(LibraryModule &&other) noexcept {
            mHandle = std::exchange(other.mHandle, {});
            return *this;
        }

        LibraryModule(const LibraryModule &) = delete;

        constexpr bool valid() const { return mHandle; }
        constexpr      operator bool() const { return valid(); }

        constexpr sys::win::hmodule_t raw() const { return mHandle; }
        constexpr                     operator sys::win::hmodule_t() const { return raw(); }
    };

    enum class ModStates {
        ManifestLoaded = 0,
        ModLoaded = 1,
        ModInited = 2,
        OtherVersionLoaded = 3,
    };

    class ModInstance {
        ModManifest   mManifest;
        LibraryModule mModulehandle = nullptr;
        IMod         *mInstance = nullptr;
        ModStates     mModState = ModStates::ManifestLoaded;

    public:
        ModInstance(ModManifest &&manifest) :
            mManifest(std::move(manifest)) {}

        ~ModInstance() = default;

        ModInstance(ModInstance &&other) noexcept = default;
        ModInstance &operator=(ModInstance &&other) noexcept = default;

        ModInstance(const ModInstance &) = delete;

        void markIgnored() { mModState = ModStates::OtherVersionLoaded; }
        void markLoaded() { mModState = ModStates::ModLoaded; }
        void markInited() { mModState = ModStates::ModInited; }

        IMod *load();

        IMod &getIMod() const {
            assert(mInstance && "mInstance is nullptr");
            return *mInstance;
        }

        const ModManifest &getManifest() const { return mManifest; }
    };

} // namespace sapphire::core