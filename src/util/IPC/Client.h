#pragma once

#include "Channel.h"
#include "Pipe.h"
#include "Protocal.h"

namespace sapphire::ipc {

    class Client {
    public:
        bool connect(const std::wstring &pipeName, int timeout_ms = 5000) {
            return mPipe.connect(pipeName, timeout_ms);
        }

        void disconnect() {
            mPipe.disconnect();
        }

        bool send(const Message &msg) {
            if (!mPipe.isOpen()) {
                return false;
            }
            Channel channel(mPipe);
            return channel.sendMessage(msg);
        }

        bool send(std::string_view status, std::string_view msg) {
            if (!mPipe.isOpen()) {
                return false;
            }
            Channel channel(mPipe);
            return channel.sendMessage(Message{status, msg});
        }

        bool isConnected() const {
            return mPipe.isOpen();
        }

    private:
        backend::Pipe mPipe;
    };

} // namespace sapphire::ipc