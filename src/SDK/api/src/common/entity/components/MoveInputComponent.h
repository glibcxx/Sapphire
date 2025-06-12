#pragma once

#include "SDK/api/src-client/common/client/input/MoveInputState.h"
#include "SDK/api/src/common/world/phys/Vec3.h"

struct MoveInputComponent {
    MoveInputState mInputState;    // off+0
    MoveInputState mRawInputState; // off+40

#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    uint64_t            mHoldAutoJumpInWaterUntil; // off+80
    Vec2                mMove;                     // off+88
    Vec2                mLookDelta;                // off+86
    Vec2                mInteractDir;              // off+104
    Vec3                mDisplacement;             // off+112
    Vec3                mDisplacementDelta;        // off+124
    Vec3                mCameraOrientation;        // off+136
    bool                mSneaking;
    bool                mSprinting;
    bool                mWantUp;
    bool                mWantDown;
    bool                mJumping;
    bool                mAutoJumpingInWater;
    bool                mMoveLocked;
    bool                mPersistSneak;
    bool                mAutoJumpEnabled;
    bool                mIsCameraRelativeMovementEnabled;
    bool                mIsRotControlledByMoveDirection;
    std::array<bool, 2> mIsPaddling;
#else
    // ...
#endif

    MoveInputComponent() = default;
};
#if MC_VERSION == v1_21_50
static_assert(sizeof(MoveInputComponent) == 168);
#endif