#pragma once

#include "SDK/api/src/common/world/phys/HitResult.h"
#include "SDK/api/src-deps/Renderer/Matrix.h"

enum class VROutputMode : int {
    REGULAR = 0,
    TWO_DIMENSIONAL = 1,
};

enum class HoloUIToPoseSource : int {
    NonVR = 0,
    MainMenu = 1,
    LivingRoom = 2,
    VRTransition = 3,
    VRHeadlocked = 4,
    VRFloatingHUD = 5,
    VRHandheldHUD = 6,
};

enum class TransformSpace : int {
    Invalid = 0,
    MinecraftAbsoluteSpace = 1,
    MinecraftSteveRelativeSpace = 2,
    MinecraftViewSpace = 3,
    MinecraftProjSpace = 4,
    UIPixelSpace = 5,
    UINormalizedSpace = 6,
    HoloLivingRoomScreenAnchorSpace = 7,
    HoloUIAnchorSpace = 8,
    HoloHeadlockedSpace = 9,
    HoloAugmentedScreenAnchorSpace = 10,
    HoloViewerAnchorSpace = 11,
    HoloRealWorldAbsoluteSpace = 12,
    HoloSRAnchorSpace = 13,
    HoloRealityAnchorSpace = 14,
    HoloPoseSpace = 15,
    HoloHeadGazeSpace = 16,
    HoloLeftHandSpace = 17,
    HoloRightHandSpace = 18,
    HoloAimSpace = 19,
    HoloHeadCameraSpace = 20,
    HoloLeftViewSpace = 21,
    HoloRightViewSpace = 22,
    HoloLeftProjSpace = 23,
    HoloRightProjSpace = 24,
    HoloLeftEarSpace = 25,
    HoloRightEarSpace = 26,
};

// size: 888
class HolographicPlatform {
public:
    enum class MCLocation : int {
        Pose = 0,
        FocusScreen = 1,
        Holoviewer = 2,
        Reality = 3,
        Logo = 4,
        EnvironmentScan = 5,
    };

    enum class GestureMode : int {
        Disabled = 0,
        Pan = 1,
        Rotate = 2,
        Tilt = 3,
        Pivot = 4,
        Zoom = 5,
        GazeOffset = 6,
        Joystick = 7,
        Abstract = 8,
    };

    enum class HolographicFeatures : int {
        LivingRoomFeature = 1,
        HoloViewerFeature = 2,
        HoloScreenFeature = 4,
        VirtualRealityFeature = 8,
        RealityModeFeature = 16,
        TransitionInsideBlocksEnabled = 32,
        HeadInsideBlocksEnabled = 64,
        GazeOverridesMouse = 128,
        MouseEnabled = 256,
        PauseMenuOnFocusLostDisabled = 512,
        FilePickingSupported = 1024,
        ImagePickingSupported = 2048,
        RollTurningSupported = 4096,
        IsRecenterable = 8192,
        HandControllersEnabled = 16384,
    };

    enum class AimFromSpace : int {
        Gaze = 0,
        LeftHand = 1,
        RightHand = 2,
    };

    // size: 504
    struct HoloFrameOfReferenceSetupData {
        Vec3               mMinecraftPlayerPos;                                 // off+0
        float              mUIWidth;                                            // off+12
        float              mUIHeight;                                           // off+16
        float              mUIGuiScale;                                         // off+20
        float              mVerticalFovRadians;                                 // off+24
        float              mHoloUIWidth;                                        // off+28
        float              mHoloUIHeight;                                       // off+32
        float              mHoloHUDWidth;                                       // off+36
        float              mHoloHUDHeight;                                      // off+40
        Matrix             mMinecraftSteveRelativeSpaceToRealitySpaceTransform; // off+44
        float              mRealityFrameModeWorldScale;                         // off+118
        Matrix             mScreenAnchorAugmentationMatrix;                     // off+112
        Matrix             mMinecraftView;                                      // off+176
        Matrix             mMinecraftProj;                                      // off+240
        bool               mDrawUIHeadLocked;                                   // off+304
        bool               mTrueMonoscopicModeEnabled;                          // off+305
        bool               mDrawFloatingHud;                                    // off+306
        bool               mFixHud;                                             // off+307
        Matrix             mHUDMatrixPatch;                                     // off+308
        Matrix             mHUDToLeftHandPatch;                                 // off+372
        HoloUIToPoseSource mUIPoseSource;                                       // off+436
        Matrix             mVRTransitionMatrixPatch;                            // off+440

        SDK_API HoloFrameOfReferenceSetupData();
#pragma SPHR_LINKER_SYM_ALIAS("??0HoloFrameOfReferenceSetupData@HolographicPlatform@@QEAA@XZ", "?ctor@HoloFrameOfReferenceSetupData@HolographicPlatform@@QEAAPEAU12@XZ")

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "\x33\xC0\x48\xC7\x41\x00\x00\x00\x00\x00\x48\x89\x01\x48\x89\x41\x00\x89\x41")
        SDK_API HoloFrameOfReferenceSetupData *ctor();
    };

    // size: 176
    struct HoloCursorWorldParams {
        HitResult hit;                        // off+0
        float     pickRange;                  // off+112
        Vec3      pickDirectionMC;            // off+116
        Vec3      lastGazePntMC;              // off+128
        Vec3      lastGazeDirMC;              // off+140
        Vec3      linearizedJumpAdj;          // off+152
        float     brightness;                 // off+164
        float     overallScale;               // off+168
        bool      renderTextureMode;          // off+172
        bool      renderAsACube;              // off+173
        bool      forceFullyVisibleCrosshair; // off+174
    };

    // size: 8
    struct LUID {
        uint32_t LowPart;  // off+0
        int32_t  HighPart; // off+4
    };

    HoloFrameOfReferenceSetupData        mFrameData;                               // off+8
    Vec3                                 mCurrentHMDPosSpring;                     // off+516
    Vec3                                 mHoloRealityModeGazeDirMCSpace;           // off+528
    std::recursive_mutex                 mMutex;                                   // off+544
    std::unordered_map<uint32_t, Matrix> mTransformMap;                            // off+624
    std::vector<HoloCursorWorldParams>   mWorldCursorParamsVec;                    // off+688
    bool                                 mBasicTransformsSet;                      // off+712
    bool                                 mStereoEnabled;                           // off+713
    bool                                 mVRLayeringActive;                        // off+714
    bool                                 mAllowMirrorPresent;                      // off+715
    Matrix                               mHeadlockedUITransform;                   // off+716
    float                                mDynamicMonoscopicEyeConvergenceDistance; // off+780
    float                                mStereoPerformanceBlockPercentage;        // off+784
    float                                mUILayerAlpha;                            // off+788
    int                                  mRecenterHoloUITimeout;                   // off+792
    AimFromSpace                         mAimFromSpace;                            // off+796
    bool                                 mLeftHandAvailable;                       // off+800
    bool                                 mRightHandAvailable;                      // off+801
    std::string                          mLivingRoomHintText;                      // off+808
    uint32_t                             mHolographicFeatures;                     // off+840
    Vec3                                 mHoloviewerGazePointMCSpace;              // off+844
    float                                mRealityFrameScale;                       // off+856
    bool                                 mStuckCursorTriggered;                    // off+860
    float                                mHoloviewerRotY;                          // off+864
    float                                mHoloscreenSize;                          // off+868
    float                                mHoloviewerSize;                          // off+872
    float                                mLastGazeXCoordNorm;                      // off+876
    float                                mLastGazeYCoordNorm;                      // off+880
    float                                mLastGazeXMouseCoordNorm;                 // off+884
    float                                mLastGazeYMouseCoordNorm;                 // off+888
    bool                                 mGazeOnScreen;                            // off+892
    bool                                 mGestureStatePrimaryHandIsHeld;           // off+893
    bool                                 mRecenterGazeActivationOriginPoint;       // off+894

    SDK_API HolographicPlatform();
#pragma SPHR_LINKER_SYM_ALIAS("??0HolographicPlatform@@QEAA@XZ", "?ctor@HolographicPlatform@@QEAAPEAV1@XZ")

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xD9\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\x48\x83\xC1\x00\xE8")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF9\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x01\x48\x83\xC1\x00\xE8")
    SDK_API HolographicPlatform *ctor();

    // vtb+0
    virtual ~HolographicPlatform();

    SPHR_DECL_API("1.21.2", "\x40\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x80\xB9\x00\x00\x00\x00\x00\x45\x8B\xE1")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\xF3\x44\x0F\x10\x65\x00\x0F\x28\xD6")
    SDK_API Matrix _getTransform(TransformSpace spaceA, TransformSpace spaceB);

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x7C\x24\x00\x55\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xF9\x83\xFA")
    SPHR_DECL_API("1.21.50,1.21.60", "\x48\x89\x5C\x24\x00\x55\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x48\x8B\xD9\x83\xFA")
    SDK_API void _makeTransform(TransformSpace spaceA, TransformSpace spaceB);
};