#include <WinSock2.h>
#include <Windows.h>
#include <filesystem>
#include <format>
#include <libloaderapi.h>
#include <objbase.h>
#include <regex>
#include <thread>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.ApplicationModel.h>
#include "SymbolResolver.h"
#include "RuntimeLinker.h"

namespace fs = std::filesystem;

// These will be managed by the bootloader process.
static std::unique_ptr<sapphire::bootloader::SymbolResolver> gSymbolResolver;
static std::unique_ptr<sapphire::bootloader::RuntimeLinker>  gRuntimeLinker;

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
        // Log the error for debugging if needed.
        // For example: ErrorBox(L"Failed to get package version: %s", ex.message().c_str());
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

void bootloader(HMODULE hModule) {
    winrt::init_apartment(winrt::apartment_type::multi_threaded);
    struct ApartmentUninit {
        ~ApartmentUninit() {
            winrt::uninit_apartment();
        }
    };
    auto mcVersion = getMinecraftVersion();
    if (!mcVersion) {
        ErrorBox(L"Failed to get game version!");
        return;
    }
    wchar_t pathBuffer[MAX_PATH];
    GetModuleFileNameW(hModule, pathBuffer, MAX_PATH);
    std::filesystem::path thisModulePath = pathBuffer;

    std::string verStr;
    auto        sapphireDllPath = getBestCompatibleVersion(*mcVersion, thisModulePath.parent_path(), verStr);
    if (sapphireDllPath.empty()) {
        ErrorBox(L"Failed to find a compatible sapphire core version!");
        return;
    }
    if (!fs::exists(sapphireDllPath)) {
        ErrorBox(L"Failed to find sapphire Dll at {}!", sapphireDllPath.wstring());
        return;
    }

    ApartmentUninit uninit;
    gSymbolResolver = std::make_unique<sapphire::bootloader::SymbolResolver>();

    if (gSymbolResolver->loadDatabase(thisModulePath.parent_path() / std::format("bedrock_sigs.{}.sig.db", verStr))) {
        gSymbolResolver->resolve();

        gRuntimeLinker = std::make_unique<sapphire::bootloader::RuntimeLinker>(*gSymbolResolver);

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