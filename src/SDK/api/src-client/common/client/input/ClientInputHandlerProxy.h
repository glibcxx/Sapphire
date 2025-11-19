#pragma once

// size: 128
struct ClientInputHandlerProxyCallbacks {
    std::function<uint32_t(const std::string &)> mGetNameId;              // off+0
    std::function<std::string()>                 mGetCurrentInputMapping; // off+8
};

// size: 128
class ClientInputHandlerProxy {
public:
    const ClientInputHandlerProxyCallbacks mCallbacks; // off+0
};