#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

// size: 24
class IApp : public Bedrock::EnableNonOwnerReferences {
public:
    // vtb+0
    virtual ~IApp() override;

    // vtb+1
    virtual void init() = 0;

    // vtb+2
    virtual bool isInited() = 0;

    // vtb+3
    virtual void changeContext() = 0;

    // vtb+4
    virtual void quit(const std::string &src, const std::string &reason) = 0;

    // vtb+5
    virtual bool wantToQuit() = 0;

    // vtb+6
    virtual void setRenderingSize(int width, int height) = 0;

    // vtb+7
    virtual void setUISizeAndScale(int width, int height, float forcedGuiScale) = 0;

    // vtb+8
    virtual void muteAudio() = 0;

    // vtb+9
    virtual void unMuteAudio() = 0;

    // vtb+10
    virtual void audioEngineOn() = 0;

    // vtb+11
    virtual void audioEngineOff() = 0;
};