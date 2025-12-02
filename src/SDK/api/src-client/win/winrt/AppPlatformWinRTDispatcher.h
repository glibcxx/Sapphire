#pragma once

#include <mutex>
#include "HIDControllerWinRT.h"

// size: 9136
class AppPlatformWinRTDispatcher {
public:
    enum class DeviceInputType : uint8_t {
        Mouse = 0,
        Multitouch = 1,
        KeyBoard = 2,
        UNK_TYPE_3 = 3,
    };

    struct IDeviceInput {
        DeviceInputType mType; // off+8

        // 0x146367330
        virtual ~IDeviceInput() = default;
    };
    static_assert(sizeof(IDeviceInput) == 16);

    struct MouseInput : IDeviceInput {
        uint16_t x;              // off+16
        uint16_t y;              // off+18
        uint32_t mUnk20;         // off+20
        char     mMouseButtonId; // off+24
        char     mButtonData;    // off+25

        virtual ~MouseInput() = default;
    };
    static_assert(sizeof(MouseInput) == 32);

    struct MultitouchInput : IDeviceInput {
        uint16_t x;      // off+16
        uint16_t y;      // off+18
        bool     mUnk20; // off+20
        bool     mUnk21; // off+21
        uint8_t  mUnk22; // off+22

        virtual ~MultitouchInput() = default;
    };
    static_assert(sizeof(MultitouchInput) == 24);

    struct KeyBoardInput : IDeviceInput {
        char     keyCode; // off+16
        uint32_t mUnk20;  // off+20

        virtual ~KeyBoardInput() = default;
    };
    static_assert(sizeof(KeyBoardInput) == 24);

    winrt::Windows::UI::Core::CoreDispatcher   mCoreDispatcher; // off+0
    std::mutex                                 mUnk8;           // off+8
    std::vector<std::unique_ptr<IDeviceInput>> mDeviceInputs;   // off+88
    uint8_t                                    mUnk112;         // off+112
    bool                                       mMouseGrabbed;   // off+113
    std::function<void()>                      mUnk120;         // off+120
    std::mutex                                 mUnk184;         // off+184
    float                                      mUnk264;         // off+264
    float                                      mUnk268;         // off+268
    uint64_t                                   mUnk272;         // off+272
    uint64_t                                   mUnk280;         // off+280
    HIDControllerWinRT                         mHIDController;  // off+288

    SPHR_DECL_API("1.21.2", "\x48\x89\x54\x24\x00\x48\x89\x4C\x24\x00\x53\x56\x57\x48\x83\xEC\x00\x48\x8B\xFA\x48\x8B\xD9\x48\x8B\xD1")
    SPHR_DECL_API("1.21.50,1.21.60", "\x4C\x8B\xDC\x49\x89\x5B\x00\x49\x89\x73\x00\x57\x48\x83\xEC\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x44\x24\x00\x48\x8B\xFA\x48\x8B\xD9\x49\x89\x4B\x00\x49\x89\x53")
    AppPlatformWinRTDispatcher *ctor(winrt::Windows::UI::Core::CoreWindow coreWindow);
};
static_assert(sizeof(AppPlatformWinRTDispatcher) == 9136);
