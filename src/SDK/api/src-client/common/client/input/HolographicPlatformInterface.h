#pragma once

enum class VROutputMode : int32_t {
    REGULAR = 0,
    TWO_DIMENSIONAL = 1,
};

enum class HoloUIToPoseSource : int32_t {
    NonVR = 0,
    MainMenu = 1,
    LivingRoom = 2,
    VRTransition = 3,
    VRHeadlocked = 4,
    VRFloatingHUD = 5,
    VRHandheldHUD = 6,
};

enum class TransformSpace : int32_t {
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

class HolographicPlatform {};