#pragma once

#include "IApp.h"
#include "SDK/api/src/common/AppPlatformListener.h"

// size: 48
class App : public IApp, public AppPlatformListener {
public:
    bool _inited;   // off+40
    bool _finished; // off+41

    // vtb+0
    virtual ~App() override;

    // vtb+12
    virtual void initImpl() = 0;
};