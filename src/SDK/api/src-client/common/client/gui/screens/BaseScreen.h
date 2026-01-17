#pragma once

#include "AbstractScene.h"

class AbstractScreenSetupCleanupStrategy;

// size: 40
class BaseScreen : public AbstractScene {
public:
    int                                                 mWidth;              // off+16
    int                                                 mHeight;             // off+20
    bool                                                mShouldSendEvents;   // off+24
    bool                                                mWantsTextOnly;      // off+25
    bool                                                mIsPopped;           // off+26
    std::unique_ptr<AbstractScreenSetupCleanupStrategy> mScreenSetupCleanup; // off+32

    // vtb+0
    virtual ~BaseScreen() override;

    // vtb+102 (1.21.50/1.21.60)
    virtual void setupForRendering(ScreenContext &screenContext);

    // vtb+103 (1.21.50/1.21.60)
    virtual void cleanupForRendering(ScreenContext &screenContext);

    // vtb+104 (1.21.50/1.21.60)
    virtual void setScreenSetupCleanup(std::unique_ptr<AbstractScreenSetupCleanupStrategy> screenSetupCleanup);
};