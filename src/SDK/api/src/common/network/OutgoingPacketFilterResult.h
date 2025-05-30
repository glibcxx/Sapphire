#pragma once

enum class OutgoingPacketFilterResult : int {
    Allowed = 0,
    Reject = 1,
};