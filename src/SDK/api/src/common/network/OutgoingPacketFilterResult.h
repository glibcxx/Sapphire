#pragma once

#include "pch.h" // IWYU pragma: keep

enum class OutgoingPacketFilterResult : int {
    Allowed = 0,
    Reject = 1,
};