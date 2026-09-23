#pragma once

#include "Pipe.h"
#include "common/IPC/Protocal.h"
#include "common/coroutine/Task.hpp"

namespace sapphire::ipc {

    class PipeChannel {
        backend::Pipe &mPipe;

    public:
        PipeChannel(backend::Pipe &pipe) noexcept : mPipe(pipe) {}

        bool sendSync(std::string_view status, std::string_view msg) {
            return sendSync({status, msg});
        }

        bool sendSync(const sapphire::ipc::Message &msg) {
            std::string serializedMsg = msg.dump();
            uint32_t    len = static_cast<uint32_t>(serializedMsg.length());

            if (!mPipe.writeSync(reinterpret_cast<const char *>(&len), sizeof(len))) {
                return false;
            }
            if (len > 0) {
                auto res = mPipe.writeSync(serializedMsg.c_str(), len);
                return res.hasValue() && res.value() > 0;
            }
            return true;
        }

        sapphire::coro::Task<bool> send(std::string_view status, std::string_view msg) {
            co_return co_await send({status, msg});
        }

        sapphire::coro::Task<bool> send(const sapphire::ipc::Message &msg) {
            std::string serializedMsg = msg.dump();
            uint32_t    len = static_cast<uint32_t>(serializedMsg.length());

            if (!co_await mPipe.write(reinterpret_cast<const char *>(&len), sizeof(len))) {
                co_return false;
            }
            if (len > 0) {
                auto res = co_await mPipe.write(serializedMsg.c_str(), len);
                co_return res.hasValue() && res.value() > 0;
            }
            co_return true;
        }

        sapphire::coro::Task<bool> recv(sapphire::ipc::Message &msg) {
            uint32_t len = 0;
            if (!co_await mPipe.read(reinterpret_cast<char *>(&len), sizeof(len))) {
                co_return false;
            }

            if (len > 0) {
                std::string buffer(len, '\0');
                if (!co_await mPipe.read(buffer.data(), len)) {
                    co_return false;
                }
                msg = sapphire::ipc::Message::parse(buffer);
            } else {
                msg = sapphire::ipc::Message{};
                co_return false;
            }
            co_return true;
        }
    };

} // namespace sapphire::ipc