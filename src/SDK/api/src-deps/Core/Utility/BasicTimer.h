#pragma once

#include "pch.h" // IWYU pragma: keep

// size: 80
class BasicTimer {
public:
    double                  mTimeDelay;              // off+0
    double                  mStartTime;              // off+8
    std::function<double()> mGetCurrentTimeCallback; // off+16
};