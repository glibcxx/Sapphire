#pragma once

enum class UIScalingRules : int {
    Desktop = 0,
    PocketApple = 1,
    PocketAndroid = 2,
    PocketWindows = 3,
    Console = 4,
    HandheldConsole = 5,
};

enum class PlatformType : int {
    Desktop = 0,
    Pocket = 1,
    Console = 2,
    SetTopBox = 3,
};

enum class ARVRPlatform : int {
    ARVR_None = 0,
    ARVR_Rift = 1,
    ARVR_Holographic = 2,
    ARVR_WindowsMR = 3,
    ARVR_OrbisVR = 4,
};

enum class FullscreenMode : int {
    Windowed = 0,
    Fullscreen = 1,
};

enum AppFocusState : int {
    Focused = 0,
    Unfocused = 1,
};

enum class BuildPlatform : int {
    Unknown = -1,
    Google = 1,
    iOS = 2,
    OSX = 3,
    Amazon = 4,
    GearVRDeprecated = 5,
    UWP = 7,
    Win32 = 8,
    Dedicated = 9,
    TvOSDeprecated = 10,
    Sony = 11,
    Nx = 12,
    Xbox = 13,
    WindowsPhoneDeprecated = 14,
    Linux = 15,
};

enum class OperationMode : int {
    Handheld = 0,
    Console = 1,
};

enum class DisplayOrientation : int {
    None = 0,
    Landscape = 1,
    Portrait = 2,
    LandscapeFlipped = 3,
    PortraitFlipped = 4,
};
