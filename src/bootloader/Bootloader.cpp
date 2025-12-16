#include "Bootloader.h"
#include "RuntimeLinker.h"
#include "bootloader/SymbolResolver.h"

#include <Windows.h>
#include <filesystem>
#include <format>
#include <regex>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.h>

namespace fs = std::filesystem;

namespace {
    struct VersionInfo {
        uint16_t Major;
        uint16_t Minor;
        uint16_t Build;
        uint16_t Revision;

        constexpr VersionInfo(uint16_t ma = 0, uint16_t mi = 0, uint16_t b = 0, uint16_t r = 0) :
            Major{ma}, Minor{mi}, Build{b}, Revision{r} {}
        constexpr VersionInfo(const winrt::Windows::ApplicationModel::PackageVersion &v) :
            Major{v.Major}, Minor{v.Minor}, Build{v.Build}, Revision{v.Revision} {}

        constexpr std::strong_ordering operator<=>(const VersionInfo &rhs) const = default;

        std::string asString() const {
            return std::format("v{}_{}_{}", Major, Minor, Build);
        }
    };

    std::optional<VersionInfo> getMinecraftVersion() {
        try {
            return winrt::Windows::ApplicationModel::Package::Current().Id().Version();
        } catch (const winrt::hresult_error &ex) {
        }
        return std::nullopt;
    }

    std::filesystem::path getBestCompatibleVersion(
        const VersionInfo &uwpInternalVersion, const std::filesystem::path &path, std::string &outVersionString
    ) {
        std::map<VersionInfo, std::filesystem::path, std::greater<VersionInfo>> versions;

        std::regex reg{"sapphire_core_for_v(\\d+)_(\\d+)_(\\d+)\\.dll"};
        for (auto &&entry : fs::directory_iterator{path}) {
            if (!entry.is_regular_file())
                continue;
            auto        filename = entry.path().filename().string();
            std::smatch matched;
            if (!std::regex_match(filename, matched, reg))
                continue;
            VersionInfo ver{
                (uint16_t)std::stoi(matched[1].str()),
                (uint16_t)std::stoi(matched[2].str()),
                (uint16_t)std::stoi(matched[3].str())
            };
            versions.emplace(ver, entry.path());
        }
        if (versions.empty())
            return {};
        VersionInfo serverSideVersion{
            uwpInternalVersion.Major,
            uwpInternalVersion.Minor,
            (uint16_t)(uwpInternalVersion.Build / 100),
            (uint16_t)(uwpInternalVersion.Build % 100)
        };
        auto it = versions.lower_bound(serverSideVersion);
        if (it == versions.end())
            return {};
        outVersionString = it->first.asString();
        return it->second;
    }

} // namespace

namespace sapphire::bootloader {

    Bootloader::Bootloader(HMODULE hModule) : mModule(hModule) {
        winrt::init_apartment(winrt::apartment_type::multi_threaded);
    }

    Bootloader::~Bootloader() {
        winrt::uninit_apartment();
    }

    bool Bootloader::preBoot() {
        return mIPCClient.connect(L"\\\\.\\pipe\\SapphireSignalPipe")
            && mIPCClient.send(ipc::status::Handshake, "Bootloader");
    }

    void Bootloader::postBoot() {
        mIPCClient.send(ipc::status::Handoff, "Bootloader");
        mIPCClient.disconnect();
    }

    void Bootloader::bootSapphire() {
        auto mcVersion = getMinecraftVersion();
        if (!mcVersion) {
            mIPCClient.send(ipc::status::Error, "[Bootloader] Failed to get game version!");
            return;
        }
        wchar_t pathBuffer[MAX_PATH];
        GetModuleFileNameW(mModule, pathBuffer, MAX_PATH);
        std::filesystem::path thisModulePath = pathBuffer;

        std::string verStr;
        auto        sapphireDllPath = getBestCompatibleVersion(*mcVersion, thisModulePath.parent_path(), verStr);
        if (sapphireDllPath.empty()) {
            mIPCClient.send(ipc::status::Error, "[Bootloader] Failed to find a compatible sapphire core version!");
            return;
        }
        if (!fs::exists(sapphireDllPath)) {
            mIPCClient.send(
                ipc::status::Error,
                std::format("[Bootloader] Failed to find sapphire Dll at {}!", sapphireDllPath.string())
            );
            return;
        }

        mSymbolResolver = std::make_unique<SymbolResolver>();

        auto sigDbPath = thisModulePath.parent_path() / std::format("bedrock_sigs.{}.sig.db", verStr);
        mIPCClient.send(
            ipc::status::Success, std::format("[Bootloader] Loading sig database at {}!", sigDbPath.string())
        );
        if (!mSymbolResolver->loadDatabase(sigDbPath)) {
            mIPCClient.send(ipc::status::Error, "[Bootloader] Failed to load signature database!");
            return;
        }

        mSymbolResolver->resolve(mIPCClient);
        mIPCClient.send(
            ipc::status::Success,
            std::format(
                "[Bootloader] {} data symbols, {} func symbols.",
                mSymbolResolver->getResolvedDataSymbols().size(),
                mSymbolResolver->getResolvedFunctionSymbols().size()
            )
        );

        mIPCClient.send(ipc::status::Success, "[Bootloader] Initializing RuntimeLinker...");
        mRuntimeLinker = std::make_unique<RuntimeLinker>(*mSymbolResolver, mIPCClient);
        mIPCClient.send(ipc::status::Success, "[Bootloader] Initializing RuntimeLinker. Done");

        mIPCClient.send(
            ipc::status::Success, std::format("[Bootloader] Injecting sapphire Dll at {}!", sapphireDllPath.string())
        );
        HMODULE sapphireDll = LoadLibraryW(sapphireDllPath.wstring().c_str());
        if (!sapphireDll) {
            mIPCClient.send(
                ipc::status::Error,
                "[Bootloader] Failed to load sapphire core, some apis may be missing or incompatible."
            );
            return;
        }
    }

} // namespace sapphire::bootloader
