#pragma once

#include <WinSock2.h>
#include <unordered_map>
#include "SDK/api/src-deps/raknet/raknet/RakNetTypes.h"

class NetworkIdentifier {
public:
    enum class Type : int {
        RakNet = 0,
        Address = 1,
        Address6 = 2,
        Generic = 3,
    };

    size_t             mNetherNetIdValue; // off+0
    RakNet::RakNetGUID mGuid;             // off+8
    sockaddr_storage   mSock;             // off+24
    Type               mType;             // off+152
};

template <>
struct std::hash<NetworkIdentifier> {
public:
    // todo
    std::size_t operator()(const NetworkIdentifier &) const;
};