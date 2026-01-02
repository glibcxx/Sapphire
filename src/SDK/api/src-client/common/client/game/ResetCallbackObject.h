#pragma once

// size: 16
class ResetCallbackObject {
public:
    bool mCallbackReady; // off+8

    // vtb+0
    virtual ~ResetCallbackObject();

    // vtb+1
    virtual void resetCallback() = 0;
};