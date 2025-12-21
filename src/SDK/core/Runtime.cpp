#include "Runtime.h"

#include <memory>
#include <stdexcept>
#include <unordered_map>
#include <thread>

#include "CrashLog.h"
#include "RenderBackend.h"
#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include "SDK/api/sapphire/util/DrawUtils.h"
#include "SDK/api/sapphire/config/Config.h"
#include "SDK/api/sapphire/event/events/eventImpls/EventHooks.h"
#include "SDK/api/sapphire/event/events/AppTerminateEvent.h"
#include "SDK/api/sapphire/event/EventBus.h"
#include "SDK/api/sapphire/platform/Environment.h"
#include "SDK/api/sapphire/service/Service.h"

#include "common/IPC/Protocal.h"
#include "common/MemoryScanning.hpp"
#include "common/ScopedTimer.hpp"

namespace sapphire::core {

    Runtime &Runtime::getInstance() {
        static Runtime instance{};
        return instance;
    }

    Runtime::Runtime() : mCrashLogger(std::make_unique<CrashLogger>()) {
    }

    Runtime::~Runtime() noexcept {
        this->shutdown();
        mCrashLogger.reset();
    }

    void Runtime::init() {
        mIPCClient.connect();
        if (!this->_init())
            mIPCClient.requestShutdown("Fail to init Sapphire Core");
        mIPCClient.disconnect();
        LogManager::getInstance().flushAll();
    }

    bool Runtime::_init() {
        if (mInitialized) return true;

        event::EventBus::getInstance().registerListener<event::AppTerminateEvent>(
            [this](event::AppTerminateEvent &e) { this->shutdown(); }
        );
        util::TimerToken token;
        {
            util::ScopedTimer timer{token};
            sapphire::info("Runtime: 正在初始化 Sapphire...");
            GuiOverlay::init();

            winrt::init_apartment(winrt::apartment_type::multi_threaded);
            HWND mainWindow = FindWindow(0, L"Minecraft");
            if (!mainWindow) {
                sapphire::error("Runtime: 未找到 Minecraft 窗口！");
                sapphire::alert(L"Runtime: 未找到 Minecraft 窗口！");
                winrt::uninit_apartment();
                return false;
            }
            platform::Environment::getInstance().setMainWindow(mainWindow);
            mRenderBackend = std::make_unique<RenderBackend>();
            event::EventHooks::init();
            sapphire::service::init();

            sapphire::info("Runtime: 加载插件...");
            if (!this->mPluginManager.loadAllPlugins())
                return false;

            this->mPluginManager.pluginsOnLoaded();
            sapphire::info("Runtime: 加载完毕...");
        }
        mInitialized = true;
        sapphire::info(
            "Runtime: Sapphire 初始化完毕，耗时：{}",
            std::chrono::duration_cast<std::chrono::milliseconds>(token.getDuration())
        );
        return true;
    }

    void sapphire::core::Runtime::shutdown() noexcept {
        if (!mInitialized) return;
        this->_shutdown();
        mInitialized = false;
        sapphire::info("Runtime: Runtime::shutdown - done");
    }

    void Runtime::_shutdown() noexcept {
        sapphire::info("Runtime: 卸载插件...");
        this->mPluginManager.unloadAllPlugins();
        sapphire::info("Runtime: 卸载完成...");
        sapphire::service::uninit();
        event::EventHooks::uninit();
        mRenderBackend.reset();
        HookManager::getInstance().teardown();
        GuiOverlay::uninit();
        sapphire::LogManager::getInstance().flushAll();
    }

} // namespace sapphire::core
