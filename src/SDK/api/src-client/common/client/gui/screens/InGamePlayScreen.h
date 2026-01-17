#pragma once

#include "BaseScreen.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src/common/world/phys/Vec3.h"
#include "macros/Macros.h"

class FrameRenderObject;
class LevelRenderer;
class ScreenContext;
class Actor;
class Frustum;
class Player;
class FullScreenEffectRenderer;
class IMinecraftGame;
class IClientInstance;
namespace mce {
    class Camera;
}

// size: 392 (1.21.2), 400 (1.21.50/1.21.60)
class InGamePlayScreen : public BaseScreen {
public:
    Vec3                                         mPickDirection;               // off+40
    Vec2                                         mTurnDelta;                   // off+52
    Vec2                                         mLastPlayerRotation;          // off+60
    Vec2                                         mLastPlayerAngleVel;          // off+68
    Vec2                                         mLastHandRotation;            // off+76
    Vec2                                         mLastHandVel;                 // off+84
    double                                       mLastTime;                    // off+96
    double                                       mLastVRStepSoundTime;         // off+104
    Vec3                                         mPreviousDirectTurnVector;    // off+112
    float                                        mTapTurnAmountRadians;        // off+124
    float                                        mLastDirectTurnAmountRadians; // off+128
    float                                        mRotXTouch;                   // off+132
    float                                        mRotYTouch;                   // off+136
    float                                        mRotXTouchLast;               // off+140
    float                                        mRotYTouchLast;               // off+144
    float                                        mLastTickT;                   // off+148
    int                                          mTick;                        // off+152
    bool                                         mTapTurnActive;               // off+156
    bool                                         mPrevTapTurnActive;           // off+157
    bool                                         mPrevAtBorder;                // off+158
    bool                                         mRollTurnTempActivation;      // off+159
    float                                        mDXLast;                      // off+160
    float                                        mDYLast;                      // off+164
    Vec3                                         mLastHitLocation;             // off+168
    bool                                         mbHitLocationRecorded;        // off+180
    std::unique_ptr<FullScreenEffectRenderer>    mFullscreenEffectRenderer;    // off+184
    __int64                                      mUnk192[22];                  // off+192
    IMinecraftGame                              &mMinecraft;                   // off+368
    Bedrock::NotNullNonOwnerPtr<IClientInstance> mClient;                      // off+376

    SDK_API InGamePlayScreen(IMinecraftGame &mc, const Bedrock::NotNullNonOwnerPtr<IClientInstance> &client);

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x49\x8B\xF8")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x83\xEC\x00\x49\x8B\xF0\x4C\x8B\xE2\x48\x8B\xD9")
    SPHR_CTOR_ALIAS SDK_API InGamePlayScreen *ctor(
        IMinecraftGame &mc, const Bedrock::NotNullNonOwnerPtr<IClientInstance> &client
    );

    // vtb+15
    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\x41\x3B\xD0")
    SDK_API virtual void tick(int nTick, int maxTick) override;

    // vtb+17
    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x28\xC1")
    SDK_API virtual void applyInput(float a) override;

    // vtb+20
    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x20\xFF\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x89\x54\x24\x00\x4C\x8B")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\xD0\xFE\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x48\x89\x54\x24\x00\x4C\x8B")
    SDK_API virtual void prepareFrame(ScreenContext &screenContext) override;

    // vtb+21
    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x41\x56\x41\x57\x48\x83\xEC\x00\x4D\x8B\xF8\x4C\x8B\xF2\x48\x8B\xD9")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x50\x4D\x8B\xE0")
    SDK_API virtual void render(ScreenContext &screenContext, const FrameRenderObject &renderObj) override;

    // vtb+105
    virtual bool _isPlayerSuspended() const;

    // vtb+106
    SPHR_DECL_API("1.21.2", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x89\x4D")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x89\x4C\x24\x00\x4D\x8B\xE8\x4C\x8B\xE2")
    SDK_API virtual void _renderLevelPrep(
        ScreenContext &screenContext, LevelRenderer &levelRenderer, Actor &cameraEntity
    );

    // vtb+107
    virtual void _preLevelRender(ScreenContext &screenContext);

    // vtb+108
    virtual void _postLevelRender(ScreenContext &screenContext, LevelRenderer &levelRenderer);

    // vtb+109
    virtual bool _shouldRenderFirstPersonObjects(LevelRenderer &levelRenderer) const;

    // vtb+110
    virtual bool _updateFreeformPickDirection(float a, Vec3 &outSrc, Vec3 &outDir);

    // vtb+111
    virtual void _renderLevel(ScreenContext &screenContext, const FrameRenderObject &renderObj);

    // vtb+112
    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xFA\x48\x8B\xD9\x48\x8B\x81")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x70\x00\x55\x57\x41\x56\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x48\x00\x44\x0F\x29\x50\x00\x44\x0F\x29\x58\x00\x44\x0F\x29\xA0\x00\x00\x00\x00\x44\x0F\x29\xA8\x00\x00\x00\x00\x44\x0F\x29\xB0\x00\x00\x00\x00\x44\x0F\x29\xB8\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45")
    SDK_API virtual void _saveMatrices(mce::Camera &camera);

    // vtb+113
    virtual void _renderFirstPerson3DObjects(ScreenContext &screenContext, LevelRenderer &levelRenderer);

    // vtb+114
    virtual void _renderTransparentFirstPerson3DObjects(ScreenContext &screenContext, LevelRenderer &levelRenderer);

    // vtb+115
    virtual void _renderItemInHand(ScreenContext &screenContext, Player &player);

    // vtb+116
    virtual bool _shouldClipLiquids() const;

    // vtb+117
    virtual bool _isHoloViewer() const;

    // vtb+118
    SPHR_DECL_API("1.21.2", "\x40\x55\x53\x56\x57\x41\x54\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4D\x8B\xF9\x4D\x8B\xF0\x48\x8B\xDA\x45\x33\xE4")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x8B\xC4\x55\x53\x56\x57\x41\x56\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x48\x00\x44\x0F\x29\x50\x00\x44\x0F\x29\x98\x00\x00\x00\x00\x44\x0F\x29\xA0")
    SDK_API virtual void _prepareCuller(mce::Camera &camera, Frustum &leftFrustumData, Frustum &rightFrustumData);

    // vtb+119
    virtual void _localPlayerTurned(float yawDegrees);

    // vtb+120
    virtual float _getPickRange();

    // vtb+121
    virtual bool _shouldPushHUD();

    // vtb+122
    virtual void _setHoloMode();

    // vtb+123
    virtual void _updateInGameCursor();

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x48\x8B\xC4\x48\x89\x58\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xA8\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x0F\x29\x78\x00\x44\x0F\x29\x40\x00\x44\x0F\x29\x48\x00\x44\x0F\x29\x90\x00\x00\x00\x00\x44\x0F\x29\x98\x00\x00\x00\x00\x44\x0F\x29\xA0\x00\x00\x00\x00\x44\x0F\x29\xA8\x00\x00\x00\x00\x44\x0F\x29\xB0\x00\x00\x00\x00\x44\x0F\x29\xB8\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x0F\x28\xF1")
    SDK_API void _pick(float a);

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "lea", "\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\x48\xC7\x41\x00\x00\x00\x00\x00\x4C\x89")
    SDK_API static const uintptr_t __vftable[];
};