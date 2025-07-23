#pragma once

#include <memory>

// size: 24
class NetworkPeer {
public:
    enum class Reliability : int {
        Reliable = 0,
        ReliableOrdered = 1,
        Unreliable = 2,
        UnreliableSequenced = 3,
    };

    enum class DataStatus : int {
        HasData = 0,
        NoData = 1,
        BrokenData = 2,
    };

    enum class NetworkLoad : int {
        Unrestricted = 0,
        Low = 1,
        Medium = 2,
        High = 3,
    };

    // vtb+0
    virtual ~NetworkPeer() = default;

    std::shared_ptr<NetworkPeer> mPeer; // off+8
};