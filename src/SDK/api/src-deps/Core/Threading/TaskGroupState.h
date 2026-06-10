#pragma once

#include "pch.h" // IWYU pragma: keep

enum class TaskGroupState : int {
    Running = 0,
    Paused = 1,
    Flush = 2,
    Sync = 3,
};
