#pragma once

#include "../UITypes.h"
#include "interfaces/IScreenController.h"

class UIPropertyBag;
class ScreenControllerProxy;
class TaskGroup;
struct ScreenEvent;
struct FlyingItemCommand;
struct SelectNavigationTabCommand;
struct RawInputScreenEventData;
struct FocusMoveScreenEventData;
struct InputModeChangeScreenEventData;
struct HoloInputModeChangeScreenEventData;
struct PointerHeldScreenEventData;
struct TextEditScreenEventData;
struct ToggleChangeEventData;
struct TextEditSelectedStateChangeEventData;
enum class ButtonState : char;

// size: 1408 (1.21.2/1.21.50), 1536 (1.21.60)
struct ScreenViewCommand {
    std::function<void()> mUnk0;    // off+0
    std::function<void()> mUnk64;   // off+64
    std::function<void()> mUnk128;  // off+128
    std::function<void()> mUnk192;  // off+192
    std::function<void()> mUnk256;  // off+256
    std::function<void()> mUnk320;  // off+320
    std::function<void()> mUnk384;  // off+384
    std::function<bool()> mUnk448;  // off+448
    std::function<bool()> mUnk512;  // off+512
    std::function<void()> mUnk576;  // off+576
    std::function<void()> mUnk640;  // off+640
    std::function<void()> mUnk704;  // off+704
    std::function<bool()> mUnk768;  // off+768
    std::function<void()> mUnk832;  // off+832
    std::function<void()> mUnk896;  // off+896
    std::function<void()> mUnk960;  // off+960
    std::function<void()> mUnk1024; // off+1024
    std::function<void()> mUnk1088; // off+1088
    std::function<void()> mUnk1152; // off+1156
    std::function<void()> mUnk1216; // off+1216
    std::function<void()> mUnk1280; // off+1280
    std::function<void()> mUnk1344; // off+1344
#if MC_VERSION == v1_21_60
    std::function<void()> mUnk1408; // off+1408
    std::function<void()> mUnk1472; // off+1472
#endif
};

// size: 2712 (1.21.2), 2720 (1.21.50), 2848 (1.21.60)
class ScreenController : public IScreenController {
public:
    enum class PreviousButtonStateRequirement : int {
        Any = 0,
        Down = 1,
        Up = 2,
        NotDown = 3,
        NotUp = 4,
    };

    using FocusMoveEventHandlerCallback = std::function<ui::ViewRequest(FocusMoveScreenEventData &)>;
    using InputModeChangedEventHandlerCallback = std::function<ui::ViewRequest(InputModeChangeScreenEventData &)>;
    using HoloInputModeChangedEventHandlerCallback = std::function<ui::ViewRequest(HoloInputModeChangeScreenEventData &)>;
    using PointerHeldEventHandlerCallback = std::function<ui::ViewRequest(PointerHeldScreenEventData &)>;
    using ButtonEventHandlerCallback = std::function<ui::ViewRequest(UIPropertyBag *)>;
    using TextEventHandlerCallback = std::function<ui::ViewRequest(TextEditScreenEventData &, int)>;
    using ScreenEventHandlerCallback = std::function<ui::ViewRequest(ScreenEvent &)>;
    using ToggleChangeEventHandlerCallback = std::function<ui::ViewRequest(ToggleChangeEventData &)>;
    using TextEditSelectedStateChangeEventHandlerCallback = std::function<ui::ViewRequest(TextEditSelectedStateChangeEventData &)>;
    using SliderChangeEventHandlerCallback = std::function<ui::ViewRequest(int, float)>;
    using AnimationEventHandlerCallback = std::function<ui::ViewRequest()>;
    using RawInputEventHandlerCallback = std::function<ui::ViewRequest(RawInputScreenEventData &)>;
    using ClippedCollectionEventHandlerCallback = std::function<ui::ViewRequest(int, int, UIPropertyBag &)>;
    using ClipStateChangeEventHandlerCallback = std::function<ui::ViewRequest(bool, UIPropertyBag &)>;
    using CustomRendererEventHandlerCallback = std::function<ui::ViewRequest()>;

    using ButtonEventCallbackKey = std::tuple<uint32_t, ButtonState>;
    using ButtonEventCallbackInfo = std::tuple<PreviousButtonStateRequirement, ButtonEventHandlerCallback>;

    // size: 1
    struct ButtonEventCallbackKeyHasher {
        std::size_t operator()(const ButtonEventCallbackKey &) const;
    };

    using ButtonEventCallbackInfoVector = std::vector<ButtonEventCallbackInfo>;
    using ButtonEventCallbackMap = std::unordered_map<
        ButtonEventCallbackKey,
        ButtonEventCallbackInfoVector,
        ButtonEventCallbackKeyHasher>;
    using ButtonInteractedEventCallbackVector = std::vector<ButtonEventHandlerCallback>;
    using ButtonInteractedEventCallbackMap = std::unordered_map<uint32_t, ButtonInteractedEventCallbackVector>;
    using ToggleChangeEventCallbackVector = std::vector<ToggleChangeEventHandlerCallback>;
    using ToggleChangeEventCallbackMap = std::unordered_map<uint32_t, ToggleChangeEventCallbackVector>;
    using TextEditSelectedStateChangeEventCallbackVector = std::vector<TextEditSelectedStateChangeEventHandlerCallback>;
    using TextEditSelectedStateChangeEventCallbackMap =
        std::unordered_map<uint32_t, TextEditSelectedStateChangeEventCallbackVector>;
    using SliderChangeEventHandlerCallbackInfo = std::tuple<bool, SliderChangeEventHandlerCallback>;
    using SliderChangeEventCallbackVector = std::vector<SliderChangeEventHandlerCallbackInfo>;
    using SliderChangeEventCallbackMap = std::unordered_map<uint32_t, SliderChangeEventCallbackVector>;
    using TextEventHandlerCallbackInfo = std::tuple<bool, TextEventHandlerCallback>;
    using TextEventHandlerCallbackVector = std::vector<TextEventHandlerCallbackInfo>;
    using TextEventHandlerCallbackMap = std::unordered_map<uint32_t, TextEventHandlerCallbackVector>;
    using AnimationEventHandlerCallbackVector = std::vector<AnimationEventHandlerCallback>;
    using AnimationEventHandlerCallbackMap = std::unordered_map<uint32_t, AnimationEventHandlerCallbackVector>;
    using ClippedCollectionEventHandlerCallbackVector = std::vector<ClippedCollectionEventHandlerCallback>;
    using ClippedCollectionEventHandlerCallbackMap =
        std::unordered_map<uint32_t, ClippedCollectionEventHandlerCallbackVector>;
    using ClipStateChangeHandlerCallbackVector = std::vector<ClipStateChangeEventHandlerCallback>;
    using ClipStateChangeEventHandlerCallbackMap = std::unordered_map<uint32_t, ClipStateChangeHandlerCallbackVector>;
    using CustomRendererEventHandlerCallbackVector = std::vector<CustomRendererEventHandlerCallback>;
    using CustomRendererEventHandlerCallbackMap =
        std::unordered_map<uint32_t, CustomRendererEventHandlerCallbackVector>;

    std::function<void(const std::string &, const UIPropertyBag &)> mControlCreateCallback;  // off+8
    std::function<void(const std::string &, const std::string &)>   mControlDestroyCallback; // off+72

    std::function<void(const std::string &)> mControlDestroyAllCallback;              // off+136
    std::function<void(const std::string &)> mScreenViewSendManualInputEventCallback; // off+200
    std::unique_ptr<ScreenControllerProxy>   mProxy;                                  // off+264
    bool                                     mCreateInitialized;                      // off+272
    bool                                     mInitialized;                            // off+273
    bool                                     mUsesErrorPopupInfo;                     // off+274
    ScreenViewCommand                        mScreenViewCommand;                      // off+280
#if MC_VERSION == v1_21_50 || v1_21_60
    __int8 mUnk1688; // off+1688
#endif
    ButtonEventCallbackMap                                mButtonEventCallbackMap;                      // off+1696
    ButtonInteractedEventCallbackMap                      mButtonInteractedEventCallbackMap;            // off+1760
    RawInputEventHandlerCallback                          mRawInputEventHandlerCallback;                // off+1824
    std::vector<FocusMoveEventHandlerCallback>            mFocusMoveEventCallbacks;                     // off+1888
    std::vector<InputModeChangedEventHandlerCallback>     mInputModeChangedEventCallbacks;              // off+1912
    std::vector<HoloInputModeChangedEventHandlerCallback> mHoloInputModeChangedEventCallbacks;          // off+1936
    std::vector<PointerHeldEventHandlerCallback>          mPointerHeldEventCallbacks;                   // off+1960
    TextEventHandlerCallbackMap                           mTextEditEventCallbackMap;                    // off+1984
    ToggleChangeEventCallbackMap                          mToggleChangeEventCallbackMap;                // off+2048
    TextEditSelectedStateChangeEventCallbackMap           mTextEditSelectedStateChangeEventCallbackMap; // off+2112
    std::vector<std::shared_ptr<ScreenController>>        mSubControllers;                              // off+2176
    SliderChangeEventCallbackMap                          mSliderChangeEventCallbackMap;                // off+2200
    AnimationEventHandlerCallbackMap                      mAnimationEventCallbackMap;                   // off+2264
    ClippedCollectionEventHandlerCallbackMap              mClippedCollectionEventCallbackMap;           // off+2328
    ClipStateChangeEventHandlerCallbackMap                mClipStateChangeEventCallbackMap;             // off+2392
    CustomRendererEventHandlerCallbackMap                 mCustomRendererEventHandlerCallbackMap;       // off+2456

    std::unordered_map<uint32_t, std::function<void(const std::string &, UIPropertyBag &)>>
        mBindCallbacks; // off+2520
    std::unordered_map<uint32_t, std::function<void(int, const std::string &, UIPropertyBag &)>>
        mCollectionBindCallbacks; // off+2584
    std::unordered_map<uint32_t, std::function<void(const std::string &, int, const std::string &, UIPropertyBag &)>>
                               mAnyCollectionBindCallbacks; // off+2648
    std::unique_ptr<TaskGroup> mTaskGroup;                  // off+2712

    SDK_API ScreenController(bool useTaskGroup);

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x0F\xB6\xDA")
    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x7C\x24\x00\x55\x41\x56\x41\x57\x48\x8B\xEC\x48\x83\xEC\x00\x0F\xB6\xFA")
    SPHR_DECL_API("1.21.60", "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x56\x48\x83\xEC\x00\x0F\xB6\xFA")
    SPHR_CTOR_ALIAS SDK_API ScreenController *ctor(bool useTaskGroup);
};