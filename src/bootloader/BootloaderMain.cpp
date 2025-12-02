#include <WinSock2.h>
#include <Windows.h>
#include <filesystem>
#include <format>
#include <libloaderapi.h>
#include <objbase.h>
#include <regex>
#include <thread>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Management.Deployment.h>
#include <winrt/Windows.ApplicationModel.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/impl/Windows.Management.Deployment.2.h>
#include "SymbolResolver.h"
#include "RuntimeLinker.h"

namespace fs = std::filesystem;

// These will be managed by the bootloader process.
static std::unique_ptr<sapphire::bootloader::SymbolResolver> g_symbolResolver;
static std::unique_ptr<sapphire::bootloader::RuntimeLinker>  g_runtimeLinker;

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
};

std::optional<VersionInfo> getMinecraftVersion() {
    try {
        winrt::Windows::Management::Deployment::PackageManager packageManager;
        for (auto &&package : packageManager.FindPackagesForUser(L"", L"Microsoft.MinecraftUWP_8wekyb3d8bbwe")) {
            return package.Id().Version();
        }
    } catch (const winrt::hresult_error &ex) {
    }
    return std::nullopt;
}

std::filesystem::path getBestCompatibleVersion(
    const VersionInfo &uwpInternalVersion, const std::filesystem::path &path
) {
    std::map<VersionInfo, std::filesystem::path, std::greater<VersionInfo>> versions;
    for (auto &&entry : fs::directory_iterator{path}) {
        if (!entry.is_regular_file())
            continue;
        auto        filename = entry.path().filename().string();
        std::regex  reg{"sapphire_core_for_v(\\d+)_(\\d+)_(\\d+)\\.dll"};
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
    return it->second;
}

// Placeholder for our core bootloader logic
void bootloader(HMODULE hModule) {
    InfoBox(L"bootloader");
    g_symbolResolver = std::make_unique<sapphire::bootloader::SymbolResolver>();

    wchar_t pathBuffer[MAX_PATH];
    GetModuleFileNameW(hModule, pathBuffer, MAX_PATH);
    std::filesystem::path thisModulePath = pathBuffer;

    if (g_symbolResolver->loadDatabase(thisModulePath.parent_path() / "bedrock_sigs.v1_21_50.sig.db")) {
        g_symbolResolver->resolve();

        // Create the RAII object. The hook will be active as long as this object exists.
        g_runtimeLinker = std::make_unique<sapphire::bootloader::RuntimeLinker>(*g_symbolResolver);
        // auto mcVersion = getMinecraftVersion();
        // if (!mcVersion) {
        //     return;
        // }
        // getBestCompatibleVersion(*mcVersion, modulePath.parent_path());
        auto sapphireDllPath = thisModulePath.parent_path() / "sapphire_core_for_v1_21_50.dll";
        if (!fs::exists(sapphireDllPath)) {
            ErrorBox(L"Failed to find sapphire Dll at {}!", sapphireDllPath.wstring());
            return;
        }
        HMODULE sapphireDll = LoadLibraryW(sapphireDllPath.wstring().c_str());
        if (!sapphireDll) {
            InfoBox(L"LoadLibrary error: {:#x}", GetLastError());
            return;
        }
    } else {
        ErrorBox(L"Failed to load signature database!");
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        std::thread(bootloader, hModule).detach();
    }
    return TRUE;
}