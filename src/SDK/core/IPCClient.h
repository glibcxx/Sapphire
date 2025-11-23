#pragma once

namespace sapphire::core {

    class IPCClient {
        HANDLE hPipe = INVALID_HANDLE_VALUE;

    public:
        void connect();
        void disconnect();

        bool isConnected() const {
            return hPipe != INVALID_HANDLE_VALUE;
        }

        void send(const std::string &msg);
    };

} // namespace sapphire::core