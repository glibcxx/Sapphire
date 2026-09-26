/*
    sapphire::core::Runtime 是 SapphireCore 的中心类型，负责启动 SapphireCore 的各项功能
*/

#include "Runtime.h"

#include <stdexcept>
#include <winrt/base.h>

#include "RenderBackend.h"
#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/hook/Hook.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include "SDK/api/sapphire/event/events/eventImpls/EventHooks.h"
#include "SDK/api/sapphire/event/events/AppTerminateEvent.h"
#include "SDK/api/sapphire/event/EventBus.h"
#include "SDK/api/sapphire/platform/Environment.h"
#include "SDK/api/sapphire/service/Service.h"

#include "common/IPC/PipeChannel.h"
#include "common/ScopedTimer.hpp"
#include "common/String.hpp"

namespace {

    // 把从 Sapphire Log 接口打印的日志传给 Bootloader
    class PipeLogSink : public sapphire::ILogSink {
        sapphire::ipc::PipeChannel mPipe;

    public:
        PipeLogSink(sapphire::ipc::backend::Pipe &p) : mPipe(p) {}

        void emit(const sapphire::LogEvent &event) override {
            auto msg = event.toString();
            mPipe.sendSync(sapphire::ipc::status::Success, !msg.empty() && msg.back() == '\n' ? std::string_view{msg.data(), msg.size() - 1} : msg);
        }
    };

    std::shared_ptr<PipeLogSink> sPipeLogSink;

} // namespace

namespace sapphire::core {

    Runtime &Runtime::getInstance() {
        static Runtime instance{};
        return instance;
    }

    Runtime::Runtime() {}

    Runtime::~Runtime() noexcept {
        this->shutdown();
    }

    void Runtime::init() {
        auto ctx = coro::IoContext::create(1);
        if (!ctx) {
            sapphire::error("Runtime: 无法创建 IoContext, msg: {}", ctx.error().message());
            throw std::runtime_error{"Runtime: 无法创建 IoContext"};
        }
        mIoCtx.emplace(std::move(*ctx));
        auto pipe = mPipeLogger->connect(L"\\\\.\\pipe\\SapphireSignalPipe.Core", *mIoCtx);
        if (!pipe) {
            sapphire::error("Runtime: 无法连接管道, msg: {}", ctx.error().message());
            sapphire::alert(L"Runtime: 无法连接管道, msg: {}", stringToWString(ctx.error().message()));
            throw std::runtime_error{"[Sapphire Core] pipe connection error"};
        }
        mPipeLogger.emplace(std::move(*pipe));
        sPipeLogSink = std::make_shared<PipeLogSink>(*mPipeLogger);
        sapphire::LogManager::getInstance().addSink(sPipeLogSink);
        sapphire::info("IPCClient: Connecting Pipe done.");

        ipc::PipeChannel channel{*mPipeLogger};
        if (!this->_init())
            channel.sendSync(ipc::status::Error, "Fail to init Sapphire Core");
        channel.sendSync(ipc::status::Handoff, "Sapphire Core");
        mPipeLogger->disconnect();
        sapphire::LogManager::getInstance().removeSink(sPipeLogSink);
        sPipeLogSink.reset();
        LogManager::getInstance().flushAll();
    }

    bool Runtime::_init() {
        if (mInitialized) return true;

        event::EventBus::getInstance().registerListener<event::AppTerminateEvent>(
            [this](event::AppTerminateEvent &e) { this->shutdown(); }
        );
        sapphire::TimerToken token;
        {
            sapphire::ScopedTimer timer{token};
            sapphire::info("Runtime: 正在初始化 Sapphire...");
            ui::GuiOverlay::init();

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
            event::impl::EventHooks::init();
            sapphire::service::init();

            sapphire::info("Runtime: 加载插件...");
            if (!this->mModRepo.loadMods())
                return false;

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
        this->mModRepo.unloadMods();
        sapphire::info("Runtime: 卸载完成...");
        sapphire::service::uninit();
        event::impl::EventHooks::uninit();
        mRenderBackend.reset();
        HookManager::getInstance().teardown();
        ui::GuiOverlay::uninit();
        sapphire::LogManager::getInstance().flushAll();
    }

} // namespace sapphire::core
