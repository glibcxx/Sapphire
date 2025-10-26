#include "Core.h"

#include <unordered_map>
#include <thread>

#include "SDK/api/sapphire/hook/Hook.h"
#include "util/MemoryScanning.hpp"
#include "util/time.h"

#include "IatPatcher.h"
#include "DX12Hook.h"
#include "SDK/api/sapphire/util/DrawUtils.h"
#include "SDK/api/sapphire/config/Config.h"
#include "SDK/api/sapphire/event/events/eventImpls/EventHooks.h"
#include "SDK/api/sapphire/service/Service.h"
#include "SDK/api/sapphire/event/events/AppTerminateEvent.h"
#include "SDK/api/sapphire/event/EventManager.h"

#include "util/threading/ThreadPool.h"
#include "util/ScopedTimer.hpp"

namespace fs = std::filesystem;

namespace moduleInfo {
    HWND     gMainWindow = nullptr;
    uint64_t gStartTime = 0;
} // namespace moduleInfo

namespace sapphire {

    SapphireModuleInfo::SapphireModuleInfo() {
        GetModuleHandleEx(
            GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCWSTR)&SapphireModuleInfo::get,
            &sapphireModuleHandle
        );
        GetModuleInformation(GetCurrentProcess(), this->sapphireModuleHandle, &sapphireModuleInfo, sizeof(MODULEINFO));
        wchar_t modulePathBuf[MAX_PATH] = {0};
        if (GetModuleFileName(sapphireModuleHandle, modulePathBuf, MAX_PATH) != 0) {
            sapphireHome = modulePathBuf;
            sapphireHome = sapphireHome.parent_path() / HOME_FOLDER_NAME;
        }
    }

    SapphireModuleInfo &SapphireModuleInfo::get() {
        static SapphireModuleInfo instance;
        return instance;
    }

    Core &Core::getInstance() {
        static Core instance{};
        return instance;
    }

    Core::~Core() noexcept {
        this->_uninit();
    }

    void Core::init() {
        Logger::Info("[Core] Connecting Pipe...");
        HANDLE hPipe = INVALID_HANDLE_VALUE;
        while (true) {
            hPipe = CreateFile(
                L"\\\\.\\pipe\\SapphireSignalPipe",
                GENERIC_WRITE,
                0,
                nullptr,
                OPEN_EXISTING,
                0,
                nullptr
            );

            if (hPipe != INVALID_HANDLE_VALUE) {
                break;
            }
            auto code = GetLastError();
            if (code != ERROR_PIPE_BUSY) {
                CloseHandle(hPipe);
                Logger::Error("[Core] 无法连接管道 (code: {:#X})", code);
                Logger::ErrorBox(L"[Core] 无法连接管道 (code: {:#X})", code);
                throw std::runtime_error{"pipe connect error"};
            }
            if (!WaitNamedPipe(L"\\\\.\\pipe\\SapphireSignalPipe", 20000)) {
                CloseHandle(hPipe);
                Logger::Error("[Core] 连接管道超时");
                Logger::ErrorBox(L"[Core] 连接管道超时");
                throw std::runtime_error{"pipe connect timeout"};
            }
        }
        Logger::Info("[Core] Connecting Pipe done.");
        bool res = this->_init();

        std::string_view msg = res ? "READY" : "ERROR";
        DWORD            bytes_written = 0;
        BOOL             bResult = WriteFile(hPipe, msg.data(), msg.size(), &bytes_written, nullptr);
        if (!bResult) {
            CloseHandle(hPipe);
            Logger::Error("[Core] 无法写入管道");
            Logger::ErrorBox(L"[Core] 无法写入管道");
            throw std::runtime_error{"send signal error"};
        }
        CloseHandle(hPipe);
    }

    bool Core::_init() {
        if (mInitialized) return true;

        event::EventManager::getInstance().registerListener<event::AppTerminateEvent>(
            [](event::AppTerminateEvent &e) {
                sapphire::Core::getInstance()._uninit();
            }
        );
        util::TimerToken token;
        {
            util::ScopedTimer timer{token};
            Logger::Info("[Core] 正在初始化 Sapphire...");
            auto &apiManager = ApiManager::getInstance();
            apiManager.startThreadPool();
            apiManager.waitAllFinished();

            winrt::init_apartment(winrt::apartment_type::multi_threaded);
            moduleInfo::gStartTime = util::getTimeMs();
            moduleInfo::gMainWindow = FindWindow(0, L"Minecraft");
            if (!moduleInfo::gMainWindow) {
                Logger::Error("[Core] 未找到 Minecraft 窗口！");
                Logger::ErrorBox(L"[Core] 未找到 Minecraft 窗口！");
                winrt::uninit_apartment();
                return false;
            }
            DX12Hook::installAsync();
            DrawUtils::getInstance();
            event::EventHooks::init();
            sapphire::service::init();

            Logger::Info("[Core] 加载插件...");
            if (!this->loadAllPlugins())
                return false;
            apiManager.waitAllFinished();

            this->mPluginManager.pluginsOnLoaded();
            Logger::Info("[Core] 加载完毕...");

            apiManager.stopThreadPool();
        }
        mInitialized = true;
        Logger::Info(
            "[Core] Sapphire初始化完毕，耗时：{}",
            std::chrono::duration_cast<std::chrono::milliseconds>(token.getDuration())
        );
        return true;
    }

    void Core::_uninit() noexcept {
        if (!mInitialized) return;
        Logger::Info("[Core] 卸载插件...");
        this->mPluginManager.unloadAllPlugins();
        Logger::Info("[Core] 卸载完成...");
        sapphire::service::uninit();
        event::EventHooks::uninit();
        DX12Hook::uninstall();
        HookManager::getInstance().teardown();
        Logger::Loggers::getInstance().flush();
        mInitialized = false;
    }

    bool Core::loadAllPlugins() {
        DWORD    dwProcessId;
        fs::path modsDir = SapphireModuleInfo::get().sapphireHome / "mods";
        if (!fs::exists(modsDir)) {
            fs::create_directory(modsDir);
        }
        if (!fs::is_directory(modsDir)) {
            Logger::Error("[Core] 未找到插件目录！sapphire/mods/");
            return false;
        }

        auto  &apiMap = ApiManager::getInstance().getApiDecoratedName2TargetAddr();
        auto  &iatPatcher = IatPatcher::getInstance();
        auto  &plugins = this->mPluginManager.mLoadedPlugins;
        size_t loaded = plugins.size();
        for (auto &&entry : fs::directory_iterator{modsDir}) {
            auto filename = entry.path().filename().string();
            if (!entry.is_regular_file()
                || entry.path().extension() != ".dll"
                || filename == "sapphire_core.dll")
                continue;
            Logger::Info("[Core] 注入文件 {}...", filename);
            HMODULE handle = LoadLibrary(entry.path().c_str());
            if (!handle) {
                Logger::Error("[Core] {} 注入失败！", filename);
                continue;
            }
            size_t current = plugins.size();
            if (current > loaded + 1) {
                Logger::Error(
                    "[Core] 不得在一个dll插件注入期间注入另一个dll插件或者一个dll创建多个IPlugin实例！({})",
                    filename
                );
                plugins.erase(
                    plugins.begin() + loaded,
                    plugins.end()
                );
            } else if (current == loaded) {
                Logger::Warn(
                    "[Core] {} 未向sapphire注册，可能不是标准sapphire插件，sapphire将无法管理其生命周期！",
                    filename
                );
            } else if (current < loaded) {
                Logger::Error("[Core] 加载 {} 后出现意外数据丢失，终止进程！", filename);
                return false;
            } else {
                plugins.back().handle = handle;
                loaded++;
                iatPatcher.patchModule(handle, apiMap);
                Logger::Info("[Core] {} 注入成功！", filename);
            }
        }
        return true;
    }

} // namespace sapphire

// 整个库的入口
BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID reserved) {
    switch (reason) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        std::thread{[]() {
            sapphire::Core::getInstance().init();
        }}.detach();
        break;
    }
    case DLL_PROCESS_DETACH:
        break;
    default:
        break;
    }
    return TRUE;
}
