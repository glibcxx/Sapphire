#pragma once

#include "SDK/api/src/common/world/phys/Vec2.h"

#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
// size: 40
struct MoveInputState {
    bool    mSneakDown;               // off+0
    bool    mSneakToggleDown;         // off+1
    bool    mWantDownSlow;            // off+2
    bool    mWantUpSlow;              // off+3
    bool    mBlockSelectToggleDown;   // off+4
    bool    mAscendScaffolding;       // off+5
    bool    mDescendScaffolding;      // off+6
    bool    mJumpDown;                // off+7
    bool    mSprintDown;              // off+8
    bool    mUpLeft;                  // off+9
    bool    mUpRight;                 // off+10
    bool    mDownLeft;                // off+11
    bool    mDownRight;               // off+12
    bool    mUp;                      // off+13
    bool    mDown;                    // off+14
    bool    mLeft;                    // off+15
    bool    mRight;                   // off+16
    bool    mAscend;                  // off+17
    bool    mDescend;                 // off+18
    bool    mChangeHeight;            // off+19
    Vec2    mAnalogMoveVector;        // off+20
    bool    mLookCenter;              // off+28
    uint8_t mLookSlightDirField;      // off+29
    uint8_t mLookNormalDirField;      // off+30
    uint8_t mLookSmoothDirField;      // off+31
    bool    mSneakInputCurrentlyDown; // off+32
    bool    mSneakInputWasReleased;   // off+33
    bool    mSneakInputWasPressed;    // off+34
    bool    mJumpInputWasReleased;    // off+35
    bool    mJumpInputWasPressed;     // off+36
    bool    mJumpInputCurrentlyDown;  // off+37
};
static_assert(sizeof(MoveInputState) == 40);
#else
// size: 32
struct MoveInputState {
    bool    mSneakDown;          // off+0
    bool    mSneakToggleDown;    // off+1
    bool    mWantDownSlow;       // off+2
    bool    mWantUpSlow;         // off+3
    bool    mAscendScaffolding;  // off+4
    bool    mDescendScaffolding; // off+5
    bool    mJumpDown;           // off+6
    bool    mSprintDown;         // off+7
    bool    mUpLeft;             // off+8
    bool    mUpRight;            // off+9
    bool    mUp;                 // off+10
    bool    mDown;               // off+11
    bool    mLeft;               // off+12
    bool    mRight;              // off+13
    bool    mAscend;             // off+14
    bool    mDescend;            // off+15
    bool    mChangeHeight;       // off+16
    Vec2    mAnalogMoveVector;   // off+20
    bool    mLookCenter;         // off+28
    uint8_t mLookSlightDirField; // off+29
    uint8_t mLookNormalDirField; // off+30
    uint8_t mLookSmoothDirField; // off+31
};
static_assert(sizeof(MoveInputState) == 32);
#endif