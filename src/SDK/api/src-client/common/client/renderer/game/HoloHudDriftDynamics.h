#pragma once

#include "SDK/api/src/common/world/phys/Vec3.h"
#include "SDK/api/src-deps/Renderer/Matrix.h"

class IClientInstance;

// size: 136
class HoloHudDriftDynamics {
public:
    Matrix           mMatrixPatch;          // off+0
    Vec3             mHudDirPoseSpace;      // off+64
    Vec3             mHudDirAVelPoseSpace;  // off+76
    Vec3             mLastHudTargetDir;     // off+88
    Vec3             mLastGazeToPoseDelta;  // off+100
    double           mLastHudDirUpdateTime; // off+112
    bool             mResetLastTargetDir;   // off+120
    IClientInstance &mClient;               // off+128
};