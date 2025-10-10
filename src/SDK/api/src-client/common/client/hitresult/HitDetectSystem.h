#pragma once

#include "SDK/api/src/common/world/phys/HitResult.h"

class ClientHitDetectCoordinator;
class IClientInstance;
class LocalPlayer;

// size: 928
class HitDetectSystem {
public:
    float                       mPickRange;               // off+8
    float                       mInteractRange;           // off+12
    bool                        mEvaluated;               // off+16
    HitResult                   mViewHitResult;           // off+24
    HitResult                   mViewLastHitResult;       // off+136
    HitResult                   mViewLiquidHitResult;     // off+248
    HitResult                   mViewLastLiquidHitResult; // off+360
    HitResult                   mPickHitResult;           // off+472
    HitResult                   mPickLastHitResult;       // off+584
    HitResult                   mPickLiquidHitResult;     // off+696
    HitResult                   mPickLastLiquidHitResult; // off+808
    ClientHitDetectCoordinator &mEventCoordinator;        // off+920

    SDK_API HitDetectSystem(ClientHitDetectCoordinator &eventCoordinator);
#pragma SPHR_LINKER_SYM_ALIAS("??0HitDetectSystem@@QEAA@AEAVClientHitDetectCoordinator@@@Z", "?ctor@HitDetectSystem@@QEAAPEAV1@AEAVClientHitDetectCoordinator@@@Z")

    SDK_API HitDetectSystem *ctor(ClientHitDetectCoordinator &eventCoordinator);

    // vtb+0
    SDK_API virtual ~HitDetectSystem();
#pragma SPHR_LINKER_SYM_ALIAS("??1HitDetectSystem@@UEAA@XZ", "?dtor@HitDetectSystem@@QEAAXXZ")

    SDK_API void dtor() noexcept;

    // vtb+1
    SDK_API virtual void reset();

    // vtb+2
    SDK_API virtual void tick();

    // vtb+3
    SDK_API virtual void evaluate(IClientInstance &client, float a);

    // vtb+4
    SDK_API virtual void _evaluateType(
        IClientInstance &client,
        float            a,
        Actor           &cameraEntity,
        LocalPlayer     &player,
        bool             isPicking,
        HitResult       &hitResult,
        HitResult       &liquidHitResult
    );
};