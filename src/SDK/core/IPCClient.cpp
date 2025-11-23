#include "IPCClient.h"
#include "SDK/api/sapphire/logger/Logger.h"
#include <memory>

namespace {

    class PipeLogSink : public sapphire::ILogSink {
        sapphire::core::IPCClient &mIPCClient;

    public:
        PipeLogSink(sapphire::core::IPCClient &IPCClient) : mIPCClient(IPCClient) {}

        void emit(const sapphire::LogEvent &event) override {
            if (mIPCClient.isConnected()) {
                mIPCClient.send(event.toString());
            }
        }
    };

    std::shared_ptr<PipeLogSink> sPipeLogSink;
} // namespace

void sapphire::core::IPCClient::connect() {
    sapphire::info("IPCClient: Connecting Pipe...");
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
            sapphire::error("IPCClient: 无法连接管道 (code: {:#X})", code);
            sapphire::alert(L"IPCClient: 无法连接管道 (code: {:#X})", code);
            throw std::runtime_error{"pipe connect error"};
        }
        if (!WaitNamedPipe(L"\\\\.\\pipe\\SapphireSignalPipe", 20000)) {
            CloseHandle(hPipe);
            sapphire::error("IPCClient: 连接管道超时");
            sapphire::alert(L"IPCClient: 连接管道超时");
            throw std::runtime_error{"pipe connect timeout"};
        }
    }
    sPipeLogSink = std::make_shared<PipeLogSink>(*this);
    sapphire::LogManager::getInstance().addSink(sPipeLogSink);
    sapphire::info("IPCClient: Connecting Pipe done.");
}

void sapphire::core::IPCClient::disconnect() {
    sapphire::LogManager::getInstance().removeSink(sPipeLogSink);
    sPipeLogSink.reset();
    CloseHandle(hPipe);
    hPipe = INVALID_HANDLE_VALUE;
}

void sapphire::core::IPCClient::send(const std::string &msg) {
    DWORD bytes_written = 0;
    BOOL  bResult = WriteFile(hPipe, msg.data(), msg.length(), &bytes_written, nullptr);
    if (!bResult) {
        CloseHandle(hPipe);
        sapphire::alert(L"IPCClient: 无法写入管道");
        throw std::runtime_error{"send signal error"};
    }
}