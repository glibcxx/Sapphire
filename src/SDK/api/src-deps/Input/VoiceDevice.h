#pragma once

#include <queue>
#include <string>

// size: 80
class VoiceDevice {
public:
    std::queue<uint32_t> mVoiceEventVector; // off+8
    std::string          mDictation;        // off+48

    // vtb+0
    virtual ~VoiceDevice();
};