#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-client/common/client/renderer/game/HoloHudDriftDynamics.h"
#include "SDK/api/src-client/common/client/input/HolographicPlatformInterface.h"

class ClientInstance;
namespace mce {
    class RenderStage;
    class Texture;
} // namespace mce

enum class ClientInstanceState : int {
    Idle = 0,
    WaitingForPlayer = 1,
    WaitingToPlay = 2,
    Playing = 3,
};

enum class WYSIWYGState : int {
    NoControlSelected = 0,
    ControlSelected = 1,
    ModifyingOpacity = 2,
    ModifyingScale = 3,
    ResetConfirmation = 4,
    CloseWithoutSavingConfirmation = 5,
    Exiting = 6,
};

// size: 640
struct ClientHMDState {
    // size: 2
    class VRRotationResetState {
        bool mPendingReset;   // off+0
        bool mForceNextReset; // off+1
    };

    ClientInstance               &mClientInstance;                   // off+0
    HoloHudDriftDynamics          mHoloHudDriftDynamics;             // off+8
    mce::RenderStage             *mUIStage;                          // off+144
    mce::RenderStage             *mLevelStage;                       // off+152
    mce::Texture                 *mLevelTexture;                     // off+160
    mce::Texture                 *mUITexture;                        // off+168
    std::shared_ptr<mce::Texture> mUILayersTexture;                  // off+176
    float                         mHoloScreenCursorXCoordNorm;       // off+192
    float                         mHoloScreenCursorYCoordNorm;       // off+196
    Matrix                        mLastLevelViewMatrix;              // off+200
    Matrix                        mLastLevelViewMatrixAbsolute;      // off+264
    Matrix                        mLastLevelProjMatrix;              // off+328
    Matrix                        mLastLevelWorldMatrix;             // off+392
    Matrix                        mHUDMatrixPatch;                   // off+456
    Matrix                        mVRTransitionMatrixPatch;          // off+520
    float                         mLastLevelViewMatrixVerticalShift; // off+584
    float                         mLastVRPitchAngle;                 // off+588
    float                         mVRRotationAdjustment;             // off+592
    float                         mVRRotAdjYawTweak;                 // off+596
    float                         mGazeCursorPitchBoostAngle;        // off+600
    float                         mFadeScreenAlpha;                  // off+604
    float                         mDesiredFadeScreenAlpha;           // off+608
    float                         mRealityFrameModeWorldScale;       // off+612
    float                         mHeadSetDirForSleeping;            // off+616
    int16_t                       mPointerX;                         // off+620
    int16_t                       mPointerY;                         // off+622
    bool                          mHoloCursorOn;                     // off+624
    bool                          mAppJustResumed;                   // off+625
    VRRotationResetState          mVRRotationResetState;             // off+626
    bool                          mHoloTransformsHaveBeenUpdated;    // off+628
    HoloUIToPoseSource            mLastHoloUIToPoseSource;           // off+632
};

// size: 24
class IClientInstance : public Bedrock::EnableNonOwnerReferences {
public:
    // vtb+0
    virtual ~IClientInstance();

    // vtb+1
    virtual void onInitMinecraftGame() = 0;

    // vtb+2
    virtual void onDestroyMinecraftGame() = 0;

    // ...

    SPHR_API static void *const *__vftable;
};