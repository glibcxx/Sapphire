#pragma once

#include "pch.h" // IWYU pragma: keep

class VoiceDevice;

// size: 16
class VoiceSystem {
public:
    std::unique_ptr<VoiceDevice> mVoiceDevice; // off+8

    // vtb+0
    virtual ~VoiceSystem();
};