#pragma once

#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"

class WorldTransferAgent;
struct ExternalWorldTransferActionFunc;
struct LocalWorldTransferActionFunc;

// size: 8
struct IWorldTransfer {
    // vtb+0
    virtual ~IWorldTransfer() = default;

    // vtb+1
    virtual void setWorldTransferAction(
        std::variant<LocalWorldTransferActionFunc, ExternalWorldTransferActionFunc, std::monostate>
    ) = 0;

    // vtb+2
    virtual Bedrock::NonOwnerPointer<WorldTransferAgent> const getWorldTransferAgent() const = 0;

    // vtb+3
    virtual void setIsWorldTransferInProgress(bool) = 0;

    // vtb+4
    virtual bool isWorldTransferInProgress() const = 0;
};
