#include "IPCClient.h"
#include "SDK/api/sapphire/logger/Logger.h"

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
    if (!ipc::Client::connect(L"\\\\.\\pipe\\SapphireSignalPipe")) {
        sapphire::error("IPCClient: 无法连接管道");
        sapphire::alert(L"IPCClient: 无法连接管道");
        throw std::runtime_error{"[Sapphire Core] pipe connection error"};
    }
    ipc::Client::send(ipc::status::Handshake, "Sapphire Core");
    sPipeLogSink = std::make_shared<PipeLogSink>(*this);
    sapphire::LogManager::getInstance().addSink(sPipeLogSink);
    sapphire::info("IPCClient: Connecting Pipe done.");
}

void sapphire::core::IPCClient::disconnect() {
    ipc::Client::send(ipc::status::Handoff, "Sapphire Core");
    ipc::Client::disconnect();
    sapphire::LogManager::getInstance().removeSink(sPipeLogSink);
    sPipeLogSink.reset();
}