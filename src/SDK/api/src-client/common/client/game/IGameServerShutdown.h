#pragma once

// size: 8
struct IGameServerShutdown {
    // vtb+0
    virtual ~IGameServerShutdown() = default;

    // vtb+1
    virtual void requestLeaveGame(bool switchScreen, bool sync) = 0;
};
