#pragma once

#include <cstdint>

class Config {
public:
    int32_t mWidth;  // off+0
    int32_t mHeight; // off+4

    // ...
};

// size: 8
class IConfigListener {
public:
    // vtb+0
    virtual ~IConfigListener() noexcept {}

    // vtb+1
    virtual void onConfigChanged(const Config &) = 0;
};