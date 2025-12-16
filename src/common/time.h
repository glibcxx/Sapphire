#pragma once

#include <chrono>

namespace util {

    inline double getTimeMs() {
        return std::chrono::steady_clock::now().time_since_epoch().count() / 1'000'000.0;
    }

} // namespace util