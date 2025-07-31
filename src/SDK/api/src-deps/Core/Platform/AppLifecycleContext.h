#pragma once

// size: 4
class AppLifecycleContext {
public:
    bool mAppliedHasGraphicsContext; // off+0
    bool mAppliedIsCurrentlyResumed; // off+1
    bool mHasGraphicsContext;        // off+2
    bool mIsCurrentlyResumed;        // off+3
};