#pragma once

#include "pch.h" // IWYU pragma: keep

enum class Rotation : uint8_t {
    None = 0,
    Rotate90 = 1,
    Rotate180 = 2,
    Rotate270 = 3,
    Total = 4,
};
