#pragma once

#include <functional>

// size: 80
class BasicTimer {
public:
    double                  mTimeDelay;              // off+0
    double                  mStartTime;              // off+8
    std::function<double()> mGetCurrentTimeCallback; // off+16
};