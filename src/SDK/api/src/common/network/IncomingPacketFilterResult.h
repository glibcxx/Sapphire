#pragma once

enum class IncomingPacketFilterResult : int {
    Allowed = 0,
    RejectedSilently = 1,
    RejectedWithDisconnect = 2,
};