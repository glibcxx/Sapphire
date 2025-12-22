#include "ModInstance.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include <filesystem>

sapphire::core::LibraryModule sapphire::core::LibraryModule::create(
    const std::filesystem::path &libPath
) {
    return LibraryModule{LoadLibrary(libPath.c_str())};
}

sapphire::core::LibraryModule::~LibraryModule() noexcept {
    if (mHandle) FreeLibrary(mHandle);
}

sapphire::IMod *sapphire::core::ModInstance::load() {
    if (mModState != ModStates::ManifestLoaded) {
        return nullptr;
    }

    mModulehandle = LibraryModule::create(mManifest.mMainDllPath);

    if (!mModulehandle) {
        sapphire::error("ModInstance: faild to load '{}'. {}", mManifest.mId, mManifest.mMainDllPath.string());
        return nullptr;
    }

    auto entryPoint = reinterpret_cast<IMod *(*)()>(GetProcAddress(mModulehandle, "sapphireModEntry"));

    if (!entryPoint) {
        sapphire::warn(
            "ModInstance: '{}' loaded but no mod entry point found. It might not be a sapphire mod. {}",
            mManifest.mId,
            mManifest.mMainDllPath.string()
        );
        return nullptr;
    }

    mInstance = entryPoint();

    if (!mInstance) {
        sapphire::error(
            "ModInstance: {} loaded but mod entry point returned invalid IMod ptr. {}",
            mManifest.mId,
            mManifest.mMainDllPath.string()
        );
    }

    return mInstance;
}