#pragma once

#include "pch.h" // IWYU pragma: keep

enum class EventResult : int {
    StopProcessing = 0,
    KeepGoing = 1,
};
