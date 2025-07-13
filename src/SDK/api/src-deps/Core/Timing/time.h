#pragma once

#include <optional>
#include <chrono>

// size: 56
class Stopwatch {
public:
    std::optional<std::chrono::steady_clock::time_point> _st;    // off+8
    std::chrono::steady_clock::time_point                _tt;    // off+24
    std::chrono::steady_clock::time_point                _last;  // off+32
    std::chrono::steady_clock::time_point                _max;   // off+40
    int                                                  _count; // off+48

    // vtb+0
    virtual ~Stopwatch() noexcept {}

    // vtb+1
    virtual double stop() = 0;

    // vtb+2
    virtual double stopContinue() = 0;
};
static_assert(sizeof(Stopwatch) == 56);