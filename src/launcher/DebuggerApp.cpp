#include "DebuggerApp.h"
#include "LauncherApp.h"
#include "Logger.h"
#include "common/String.hpp"
#include "common/coroutine/IoContext.hpp"
#include "common/coroutine/SyncWait.hpp"
#include "common/coroutine/Task.hpp"
#include "common/coroutine/WhenAll.hpp"
#include "common/IPC/Pipe.h"
#include "common/ScopeGuard.hpp"

#include <AccCtrl.h>
#include <atlcomcli.h>
#include <aclapi.h>
#include <iostream>
#include <libloaderapi.h>
#include <sddl.h>
#include <appmodel.h>
#include <shobjidl.h>
#include <processthreadsapi.h>
#include <winerror.h>
#include <winternl.h>

namespace sapphire::launcher {

    DebuggerApp::DebuggerApp(sys::win::dword_t pid) : mGameProcessId(pid), mLauncherAppPath(MAX_PATH, {}) {
        HRESULT hResult = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
        if (FAILED(hResult)) {
            ErrorBox(L"[launcher] CoInitializeEx 失败 (code: {})", hResult);
            throw std::runtime_error{"[launcher] CoInitializeEx 失败"};
        }

        GetModuleFileNameW(nullptr, mLauncherAppPath.data(), mLauncherAppPath.size());
    }

    DebuggerApp::~DebuggerApp() noexcept {
        CoUninitialize();
    }

    int DebuggerApp::run() {
        auto ctx = coro::IoContext::create(1);
        if (!ctx) {
            ErrorBox(L"[Debugger] 无法创建 IoContext, msg: {}", stringToWString(ctx.error().message()));
            return -1;
        }
        std::cout << "[Debugger] 连接启动器...\n";
        auto pipe = ipc::backend::Pipe::connect(L"\\\\.\\pipe\\SapphireSignalPipe.Debugger", *ctx);
        if (!pipe) {
            ErrorBox(L"[Debugger] 无法创建 Pipe, msg: {}", stringToWString(pipe.error().message()));
            return -1;
        }
        std::cout << "[Debugger] 连接成功！\n";
        auto [ok1, ok2] = syncWait(whenAll(
            [](coro::IoContext &ctx, ipc::backend::Pipe pipe, sys::win::dword_t gameProcessId) -> coro::Task<bool> {
                ScopeGuard guard{[&]() { ctx.stop(); }};

                std::cout << std::format("[Debugger] 发送游戏 pid={}\n", gameProcessId);
                size_t totalSent = 0;
                while (totalSent < sizeof(gameProcessId)) {
                    auto sent = co_await pipe.write((const char *)&gameProcessId + totalSent, sizeof(gameProcessId) - totalSent);
                    if (sent.hasError()) {
                        ErrorBox(L"[Debugger] 无法将游戏 pid 传回启动器 (code: {})", sent.error().value());
                        co_return false;
                    }
                    totalSent += sent.value();
                }
                std::cout << std::format("[Debugger] 已发送游戏 pid\n");
                char b[1]{};
                (void)co_await pipe.read(b, 1);

                std::cout << std::format("[Debugger] 退出...\n");

                co_return true;
            }(*ctx, std::move(pipe.value()), mGameProcessId),
            [](coro::IoContext &ctx) -> coro::Task<bool> {
                ctx.processEvents();
                co_return true;
            }(*ctx)
        ));
        return ok1.result() ? 0 : -1;
    }

} // namespace sapphire::launcher