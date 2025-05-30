#pragma once

#include <unordered_map>

class NetworkIdentifier {
public:
    enum class Type : int32_t {
        RakNet = 0,
        Address = 1,
        Address6 = 2,
        Generic = 3,
    };

    NetworkIdentifier() = delete;

    // uint64             mNetherNetIdValue;
    // RakNet::RakNetGUID mGuid;
    // sockaddr_storage   mSock;
    // Type               mType;
};

template <>
struct std::hash<NetworkIdentifier> {
public:
    // todo
    std::size_t operator()(const NetworkIdentifier &) const;
};