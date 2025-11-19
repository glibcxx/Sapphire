#pragma once

#include "KeyboardManager.h"
class IClientInstance;

// size: 160
class MinecraftKeyboardManager : public KeyboardManager {
public:
    bool                     mDelayedInputResume;                 // off+8
    bool                     mIsKeyboardActive    : 1;            // off+9
    bool                     mIsKeyboardEnabled   : 1;            // off+9
    bool                     mKeyboardClosedEvent : 1;            // off+9
    float                    mKeyboardForcedHeight;               // off+12
    std::function<void()>    mResumeInputCallback;                // off+16
    std::function<void(int)> mDisableInputToOtherClientsCallback; // off+80
    IClientInstance         &mClientInstance;                     // off+144
    bool                     mWasEnabledWithMultiline;            // off+152
    bool                     mWasEnabledWithNumbersOnly;          // off+153
    int                      mMaxLength;                          // off+156

    MinecraftKeyboardManager(IClientInstance &client) = delete;

    // vtb+0
    virtual ~MinecraftKeyboardManager() override;

    // vtb+17
    virtual void setResumeInputCallback(std::function<void()> callback);

    // vtb+18
    virtual void setDisableInputToOtherClientsCallback(std::function<void(int)> callback);
};