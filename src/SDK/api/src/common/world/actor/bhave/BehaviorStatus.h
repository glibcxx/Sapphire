#pragma once

#include "pch.h" // IWYU pragma: keep

enum class BehaviorStatus : int {
    Success = 0,
    Running = 1,
    Failure = 2,
    Error = 3,
    Undefined = 4,
};
