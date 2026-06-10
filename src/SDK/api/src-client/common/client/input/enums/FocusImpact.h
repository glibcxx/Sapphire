#pragma once

#include "pch.h" // IWYU pragma: keep

enum class FocusImpact : char {
    Neutral = 0,
    ActivateFocus = 1,
    DeactivateFocus = 2,
};