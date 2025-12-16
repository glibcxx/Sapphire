#pragma once

#include "common/IPC/Client.h"
#include "common/IPC/Protocal.h"

namespace sapphire::core {

    class IPCClient : public ipc::Client {
    public:
        void connect();
        void disconnect();

        bool send(const std::string &msg) {
            if (!msg.empty() && msg.back() == '\n') {
                return ipc::Client::send(ipc::status::Success, std::string_view{msg.data(), msg.size() - 1});
            }
            return ipc::Client::send(ipc::status::Success, msg);
        }

        void requestShutdown(const std::string &msg) {
            ipc::Client::send(ipc::status::Error, msg);
        }
    };

} // namespace sapphire::core