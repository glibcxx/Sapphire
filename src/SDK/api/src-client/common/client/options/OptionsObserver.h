#pragma once

#include "SDK/api/src-deps/Core/Utility/Observer.h"

// size: 16
class OptionsObserver : public Core::Observer<OptionsObserver, Core::SingleThreadedLock> {
public:
    // vtb+0
    virtual ~OptionsObserver() override;

    // vtb+2
    virtual void onForceCloudSave() = 0;

    // vtb+3
    virtual void onOptionsLoadBegin() = 0;

    // vtb+4
    virtual void onOptionsLoadComplete() = 0;
};