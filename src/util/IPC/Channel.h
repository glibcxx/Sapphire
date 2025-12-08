#pragma once

#include "Pipe.h"
#include "Protocal.h"

namespace sapphire::ipc {

    class Channel {
    public:
        Channel(backend::Pipe &pipe) : mPipe(pipe) {}

        bool sendMessage(const Message &msg) {
            std::string serializedMsg = msg.dump();
            uint32_t    len = static_cast<uint32_t>(serializedMsg.length());

            if (!mPipe.write(reinterpret_cast<const char *>(&len), sizeof(len))) {
                return false;
            }
            if (len > 0) {
                return mPipe.write(serializedMsg.c_str(), len);
            }
            return true;
        }

        bool receiveMessage(Message &msg) {
            uint32_t len = 0;
            if (!mPipe.read(reinterpret_cast<char *>(&len), sizeof(len))) {
                return false;
            }

            if (len > 0) {
                std::string buffer(len, '\0');
                if (!mPipe.read(buffer.data(), len)) {
                    return false;
                }
                msg = Message::parse(buffer);
            } else {
                // If length is 0, it's a valid empty message.
                msg = Message();
            }
            return true;
        }

    private:
        backend::Pipe &mPipe;
    };

} // namespace sapphire::ipc