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

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\xC7\x41\x00\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\x45\x33\xC0\xC7\x41")
    SDK_API HitDetectSystem *ctor(ClientHitDetectCoordinator &eventCoordinator);

    // vtb+0
    SDK_API virtual ~HitDetectSystem();
#pragma SPHR_LINKER_SYM_ALIAS("??1HitDetectSystem@@UEAA@XZ", "?dtor@HitDetectSystem@@QEAAXXZ")

    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x8B\xF9\x48\x89\x01\xBB")
    SDK_API void dtor() noexcept;

    // vtb+1
    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x8B\x05")
    SDK_API virtual void reset();

    // vtb+2
    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x55\x57\x41\x56\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x8B\xF9\x80\x79")
    SDK_API virtual void tick();

    // vtb+3
    SAPPHIRE_API("1.21.2,1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\x02\x48\x8B\xF9\x0F\x29\x74\x24\x00\x48\x8B\xCA\x0F\x28\xF2\x48\x8B\xDA\x48\x8B\x80")
    SDK_API virtual void evaluate(IClientInstance &client, float a);

    // vtb+4
    SAPPHIRE_API("1.21.2", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x88\x00\x00\x00\x00\x44\x0F\x29\x90\x00\x00\x00\x00\x44\x0F\x29\x98\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xF9\x44\x0F\x28\xDA")
    SAPPHIRE_API("1.21.50,1.21.60", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x48\x00\x44\x0F\x29\x90\x00\x00\x00\x00\x44\x0F\x29\x98\x00\x00\x00\x00\x48\x8B\x05")
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