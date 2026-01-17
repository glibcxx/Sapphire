#pragma once

#include "../ScreenController.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Input/InputMode.h"

class MinecraftScreenModel;
enum class ModalScreenButtonId : int;

enum class ScreenExitBehavior : int {
    LeaveScreen = 0,
    ExitGame = 1,
    GoToStartScreen = 2,
    DoNothing = 3,
};

// https://github.com/LiteLDev/LeviLamina/blob/f787d29ecb3e57b413067b4dbbec17383d591bfe/src/mc/client/gui/screens/controllers/MinecraftScreenController.h#L35

// size: 3104 (1.21.2), 3112 (1.21.50), 3240 (1.21.60)
class MinecraftScreenController : public ScreenController,
                                  public Bedrock::EnableNonOwnerReferences,
                                  public std::enable_shared_from_this<MinecraftScreenController> {
public:
    // size: 40
    struct LeaveScreenInfo {
        bool        mLeaveScreen;
        std::string mLeaveScreenMessage;
    };

    std::shared_ptr<MinecraftScreenModel>              mMinecraftScreenModel;      // off+2760
    ScreenExitBehavior                                 mExitBehavior;              // off+2776
    InputMode                                          mInputMode;                 // off+2780
    HoloUIInputMode                                    mHoloUIInputMode;           // off+2784
    InputMode                                          mPreviousInputMode;         // off+2788
    HoloUIInputMode                                    mPreviousHoloUIInputMode;   // off+2792
    bool                                               mPlayerDied;                // off+2796
    std::function<void(ModalScreenButtonId)>           mModalDialogResultCallback; // off+2800
    std::string                                        mTTSTitle;                  // off+2864
    std::string                                        mTTSDialogMessage;          // off+2896
    LeaveScreenInfo                                    mLeaveScreenInfo;           // off+2928
    bool                                               mRayTracingActive;          // off+2968
    std::unordered_map<std::string, std::vector<bool>> mDropDownActive;            // off+2976
    std::unordered_map<std::string, int>               mStepSliderValues;          // off+3040
    __int8                                             mUnk3104;                   // off+3104

    SDK_API MinecraftScreenController(
        std::shared_ptr<MinecraftScreenModel> model, ScreenExitBehavior exitBehavior, bool usesErrorPopupInfo
    );

    SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "disp:16,call", "\x48\x89\x44\x24\x00\x41\xB1\x00\x45\x33\xC0\x48\x8D\x54\x24\x00\xE8\x00\x00\x00\x00\x90\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x03\x48\x8D\x05\x00\x00\x00\x00\x48\x89\x83\x00\x00\x00\x00\x33\xC0")
    SPHR_CTOR_ALIAS SDK_API MinecraftScreenController *ctor(
        std::shared_ptr<MinecraftScreenModel> model, ScreenExitBehavior exitBehavior, bool usesErrorPopupInfo
    );
};