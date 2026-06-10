#pragma once

#include "pch.h" // IWYU pragma: keep

enum class PacketViolationResponse : int {
    None = 0,
    Warning = 1,
    FinalWarning = 2,
    TerminateConnection = 3,
};