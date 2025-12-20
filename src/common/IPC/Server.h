#pragma once

#include <thread>
#include <atomic>
#include <functional>
#include <string>
#include <vector>

#include "Channel.h"
#include "Pipe.h"
#include "Protocal.h"

namespace sapphire::ipc {

    class Server {
    public:
        ~Server() {
            stop();
        }

        void start(const std::wstring &pipeName) {
            if (mIsRunning) return;

            mPipeName = pipeName;
            mIsRunning = true;
            mThread = std::thread(&Server::serverLoop, this);
        }

        void stop() {
            if (!mIsRunning) return;

            mIsRunning = false;

            backend::Pipe dummyClient;
            dummyClient.connect(mPipeName, 10);
            dummyClient.disconnect();

            if (mThread.joinable()) {
                mThread.join();
            }
        }

        std::function<void()>                onClientConnected;
        std::function<void(const Message &)> onMessage;
        std::function<void()>                onClientDisconnected;

    private:
        void serverLoop() {
            while (mIsRunning) {
                backend::Pipe connection;
                if (connection.create(mPipeName) && connection.listen()) {
                    if (mIsRunning && onClientConnected) onClientConnected();

                    Channel channel(connection);
                    Message msg;
                    while (mIsRunning && channel.receiveMessage(msg)) {
                        if (onMessage) onMessage(msg);
                    }

                    if (mIsRunning && onClientDisconnected) onClientDisconnected();
                }
                if (!mIsRunning) std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
        }

        std::atomic<bool> mIsRunning = false;
        std::thread       mThread;
        std::wstring      mPipeName;
    };

} // namespace sapphire::ipc