/*
    Bootloader被注入进游戏进程后，会先使用IPC连接sapphire启动器，以便发送Bootloader的日志，
    然后会检测游戏版本，自动选择匹配的 sapphire_core.dll 和特征码数据库，
    再读入特征码数据库完成特征码数据库扫描，创建一个自动拦截dll修补IAT的RuntimeLinker，
    最后注入 sapphire_core，断开IPC连接
*/

#include "Bootloader.h"
#include "RuntimeLinker.h"
#include "bootloader/SymbolResolver.h"
#include "common/IPC/Pipe.h"
#include "common/IPC/PipeChannel.h"
#include "common/ScopeGuard.hpp"
#include "common/coroutine/IoContext.hpp"
#include "common/coroutine/SyncWait.hpp"
#include "common/coroutine/WhenAll.hpp"
#include "common/sys/MiniWindows.h"

#include <Windows.h>
#include <filesystem>
#include <format>
#include <memory>
#include <regex>
#include <system_error>
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
            return std::format("{}.{}.{}", Major, Minor, Build);
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

        std::regex reg{"sapphire_core\\+mc(\\d+).(\\d+).(\\d+)\\.dll"};
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

    Bootloader::Bootloader(sys::win::hmodule_t hModule) : mModule(hModule) {
        winrt::init_apartment(winrt::apartment_type::multi_threaded);
        auto expected = coro::IoContext::create(1);
        if (!expected) {
            throw std::system_error{expected.error()};
        }
        mIoCtx.emplace(std::move(expected.value()));
    }

    Bootloader::~Bootloader() {
        winrt::uninit_apartment();
    }

    int Bootloader::run() {
        auto pipe = ipc::backend::Pipe::connect(L"\\\\.\\pipe\\SapphireSignalPipe.Bootloader", *mIoCtx);
        if (!pipe) {
            throw std::system_error{std::move(pipe.error()), "Bootloader connect error"};
        }
        mPipeConnection.emplace(std::move(pipe.value()));
        ipc::PipeChannel channel{*mPipeConnection};
        syncWait(
            coro::whenAll(
                channel.send({ipc::status::Handshake, "Bootloader"}),
                bootSapphire(),
                [](sapphire::coro::IoContext &ctx) -> sapphire::coro::Task<int> {
                    ctx.processEvents();
                    co_return 0;
                }(*mIoCtx)
            )
        );
        return true;
    }

    coro::Task<bool> Bootloader::bootSapphire() {
        sapphire::ScopeGuard guard{[&]() { mIoCtx->stop(); }};

        ipc::PipeChannel channel{*mPipeConnection};
        auto             mcVersion = getMinecraftVersion();
        if (!mcVersion) {
            co_await channel.send(ipc::status::Error, "[Bootloader] Failed to get game version!");
            co_return false;
        }
        wchar_t pathBuffer[MAX_PATH];
        GetModuleFileNameW(mModule, pathBuffer, MAX_PATH);
        std::filesystem::path thisModulePath = pathBuffer;

        std::string verStr;
        auto        sapphireDllPath = getBestCompatibleVersion(*mcVersion, thisModulePath.parent_path(), verStr);
        if (sapphireDllPath.empty()) {
            co_await channel.send(ipc::status::Error, "[Bootloader] Failed to find a compatible sapphire core version!");
            co_return false;
        }
        if (!fs::exists(sapphireDllPath)) {
            co_await channel.send({ipc::status::Error, std::format("[Bootloader] Failed to find sapphire Dll at {}!", sapphireDllPath.string())});
            co_return false;
        }

        mSymbolResolver = std::make_unique<SymbolResolver>();

        auto sigDbPath = thisModulePath.parent_path() / std::format("bedrock_sigs+mc{}.sig.db", verStr);
        co_await channel.send(
            ipc::status::Success, std::format("[Bootloader] Loading sig database at {}!", sigDbPath.string())
        );
        if (!mSymbolResolver->loadDatabase(sigDbPath)) {
            co_await channel.send(ipc::status::Error, "[Bootloader] Failed to load signature database!");
            co_return false;
        }

        co_await mSymbolResolver->resolve(*mIoCtx, channel);
        co_await channel.send(
            ipc::status::Success,
            std::format(
                "[Bootloader] {} data symbols, {} func symbols.",
                mSymbolResolver->getResolvedDataSymbols().size(),
                mSymbolResolver->getResolvedFunctionSymbols().size()
            )
        );

        co_await channel.send(ipc::status::Success, "[Bootloader] Initializing RuntimeLinker...");
        mRuntimeLinker = std::make_unique<RuntimeLinker>(*mSymbolResolver, channel);
        co_await channel.send(ipc::status::Success, "[Bootloader] Initializing RuntimeLinker. Done");

        co_await channel.send(
            ipc::status::Success, std::format("[Bootloader] Injecting sapphire Dll at {}!", sapphireDllPath.string())
        );
        HMODULE sapphireDll = LoadLibraryW(sapphireDllPath.wstring().c_str());
        if (!sapphireDll) {
            co_await channel.send(
                ipc::status::Error,
                "[Bootloader] Failed to load sapphire core, some apis may be missing or incompatible."
            );
            co_return false;
        }

        co_await channel.send({ipc::status::Handoff, "Bootloader"});
        mPipeConnection->disconnect();
        co_return true;
    }

} // namespace sapphire::bootloader
