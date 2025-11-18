#pragma once

class ClientNetworkSystem;
class ServerNetworkSystem;

// size: 16
class ClientOrServerNetworkSystemRef {
public:
    std::variant<
        std::reference_wrapper<ClientNetworkSystem>,
        std::reference_wrapper<ServerNetworkSystem>>
        mNetworkSystem; // off+0
};