/*
    代表一个被载入的 mod 模块，其实就是把 HMODULE 封装了两层
*/

#include "ModInstance.h"
#include <filesystem>
#include <Windows.h>
#include "SDK/api/sapphire/logger/Logger.h"

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

    for (auto &path : mManifest.mBinPath) {
        LibraryModule &module = mModulehandles.emplace_back(LibraryModule::create(path));
        if (!module) {
            mModulehandles.clear();
            sapphire::error("ModInstance: faild to load '{}'. {}", mManifest.mId, path.string());
            return nullptr;
        }
    }

    // 如果是SapphireMod，则第一个dll是入口dll
    if (mManifest.mType == ModManifest::ModType::SapphireMod && mModulehandles.size()) {
        auto entryPoint = reinterpret_cast<IMod *(*)()>(GetProcAddress(mModulehandles[0], "sapphireModEntry"));

        if (!entryPoint) {
            sapphire::warn(
                "ModInstance: '{}' loaded but no mod entry point found. It might not be a sapphire mod. {}",
                mManifest.mId,
                mManifest.mBinPath[0].string()
            );
            return nullptr;
        }

        mInstance = entryPoint();

        if (!mInstance) {
            sapphire::error(
                "ModInstance: {} loaded but mod entry point returned invalid IMod ptr. {}",
                mManifest.mId,
                mManifest.mBinPath[0].string()
            );
        }
    }

    return mInstance;
}