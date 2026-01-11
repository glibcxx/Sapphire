#pragma once

#include <winrt/windows.gaming.input.h>
#include "GameControllerHandler.h"
#include "AppPlatformListener.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Core/Threading/TaskGroup.h"
#include "SDK/api/src-deps/Core/Threading/EnableQueueForMainThread.h"

// size: 496 (1.21.2/1.21.50), 512 (1.21.60)
class GameControllerHandler_winrt : public Bedrock::EnableNonOwnerReferences,
                                    public GameControllerHandler,
                                    public AppPlatformListener {
public:
    // size: 384
    class WGIGamepad : public Bedrock::Threading::EnableQueueForMainThread {
    public:
        uint64_t                               mUnk16;      // off+16
        uint64_t                               mUnk24;      // off+24
        winrt::Windows::Gaming::Input::Gamepad mWGIGamepad; // off+32
        uint64_t                               _fill120[9]; // off+40
        uint32_t                               mUnk112;     // off+112

        std::function<void(winrt::Windows::Gaming::Input::Gamepad, winrt::Windows::System::User)> mUnk120; // off+120

        std::mutex     mUnk184;            // off+184
        winrt::hstring mUserNonRoamableId; // off+264
        uint8_t        mUnk272;            // off+272
        std::mutex     mUnk280;            // off+280
        uint64_t       _fill360[3];        // off+360

        SPHR_CTOR_ALIAS SDK_API WGIGamepad(winrt::Windows::Gaming::Input::Gamepad gamepad);

        SPHR_DECL_API("1.21.2,1.21.50,1.21.60", "disp:15,call", "\xFF\x15\x00\x00\x00\x00\x48\x8D\x54\x24\x00\x48\x8D\x00\x00\xE8\x00\x00\x00\x00\xEB\x00\x48")
        SPHR_CTOR_ALIAS SDK_API WGIGamepad *ctor(winrt::Windows::Gaming::Input::Gamepad gamepad);
    };
    static_assert(sizeof(WGIGamepad) == 384);

    ;                                                                       // offset: (1.21.2/1.21.50), (1.21.60 = 1.21.50 + 16)
    std::map<uint32_t, std::shared_ptr<WGIGamepad>> mWGIGamepads;           // off+288
    std::unordered_map<std::string, uint64_t>       mUnk304;                // off+304
    winrt::event_token                              mOnGamepadAdded;        // off+368
    winrt::event_token                              mOnGamepadRemoved;      // off+376
    GameControllerHandler_winrt                    *mGameControllerHandler; // off+384
    std::mutex                                      mMutex;                 // off+392
    uint64_t                                        mUnk372;                // off+472
    uint64_t                                        mUnk380;                // off+480
    std::unique_ptr<TaskGroup>                      mTaskGroup;             // off+488

    static const uintptr_t __vftable0[]; // ??_7GameControllerHandler_winrt@@6BEnableNonOwnerReferences@Bedrock@@@
};
#if MC_VERSION == v1_21_2 || MC_VERSION == v1_21_50
static_assert(sizeof(GameControllerHandler_winrt) == 496);
#elif MC_VERSION == v1_21_60
static_assert(sizeof(GameControllerHandler_winrt) == 512);
#endif
