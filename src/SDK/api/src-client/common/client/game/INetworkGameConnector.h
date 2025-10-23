#pragma once

#include "SDK/api/src-deps/Core/Threading/Async.h"

namespace Realms {
    class World;
}
namespace Social {
    class GameConnectionInfo;
    enum class MultiplayerServiceIdentifier : int;
} // namespace Social

// size: 8
class INetworkGameConnector {
public:
    // vtb+0
    virtual ~INetworkGameConnector() = default;

    // vtb+1
    virtual void joinRealmsGame(const Realms::World &realm, const Social::GameConnectionInfo &gameConnection, bool) = 0;

    // vtb+2
    virtual void joinRemoteServerWithAddress(Social::GameConnectionInfo gameConnection, const int) = 0;

    // vtb+3
    virtual Bedrock::Threading::IAsyncResult<void>::Handle joinMultiplayerWithService(
        const std::string &, const Social::MultiplayerServiceIdentifier
    ) = 0;
};