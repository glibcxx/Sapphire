#pragma once

#include "SDK/api/src-client/common/client/game/IConfigListener.h"
#include "SDK/api/src/common/AppPlatformListener.h"
#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"
#include "SDK/api/src-deps/Input/RectangleArea.h"
#include "SDK/api/src/common/world/actor/player/PlayerInventory.h"
#include "SDK/api/src-deps/MinecraftRenderer/Renderer/Mesh.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "ScreenSizeData.h"
#include "MenuPointer.h"
#include "GuiMessage.h"
#include "TitleMessage.h"

class IClientInstance;
class DevConsoleLogger;

enum class ContainerID : signed char;

#if MC_VERSION == v1_21_2 || MC_VERSION == v1_21_50
// size: 2504 (1.21.2), 2704 (1.21.50)
class GuiData : public IConfigListener,
                public /*private*/ AppPlatformListener,
                public Bedrock::EnableNonOwnerReferences {
public:
    ScreenSizeData           mScreenSizeData;                    // off+48
    bool                     mScreenSizeDataValid;               // off+72
    float                    mGuiScale /*= 3.0f*/;               // off+76
    float                    mInvGuiScale /*= 0.33333334f*/;     // off+80
    bool                     mIsCurrentlyActive;                 // off+84
    std::set<int>            mPostedErrors;                      // off+88
    MenuPointer              mMenuPointer;                       // off+104
    int16_t                  mPointerX;                          // off+106
    int16_t                  mPointerY;                          // off+108
    bool                     mHasShowPreexistingMessages;        // off+110
    bool                     mToolbarWasRendered;                // off+111
    int32_t                  mPrevSelectedSlot;                  // off+112
    ContainerID              mPrevSelectedInventoryContainer;    // off+116
    int32_t                  mNumSlots;                          // off+120
    int32_t                  mFlashSlotId;                       // off+124
    double                   mFlashSlotStartTime;                // off+128
    IClientInstance         &mClient;                            // off+136
    RectangleArea            mToolbarArea;                       // off+144
    RectangleArea            mToolbarAreaContainer;              // off+160
    std::string              mLastPopupText;                     // off+176
    std::string              mLastPopupSubtitleText;             // off+208
    std::string              mLastJukeboxPopupText;              // off+240
    std::string              mLastJukeboxPopupSubtitleText;      // off+272
    int32_t                  mTickCount /*= 0*/;                 // off+304
    float                    mItemNameOverlayTime /*= 2.0f*/;    // off+308
    float                    mJukeboxNameOverlayTime /*= 3.0f*/; // off+312
    bool                     mPopupNoticeDirty;                  // off+316
    bool                     mJukeboxPopupNoticeDirty;           // off+317
    std::vector<GuiMessage>  mGuiMessages;                       // off+320
    std::vector<std::string> mDevConsoleMessages;                // off+344
    int32_t                  mMaxDevConsoleMessages /*= 100*/;   // off+368
    std::vector<std::string> mContentLogMessages;                // off+376
    std::vector<std::string> mPerfTurtleMessages;                // off+400
    TitleMessage             mTitleMessage;                      // off+424

#    if MC_VERSION == v1_21_2
    uint32_t                                       mUnk536[13];               // off+536
    uint32_t                                       mServerSettingsId;         // off+588
    std::string                                    mServerSettings;           // off+592
    bool                                           mMuteChat;                 // off+624
    float                                          mCurrentDropTicks;         // off+628
    PlayerInventory::SlotData                      mCurrentDropSlot;          // off+632
    PlayerInventory::SlotData                      mLastSelectedSlot;         // off+640
    bool                                           mShowProgress;             // off+648
    std::string                                    mTipMessage;               // off+656
    float                                          mTipMessageLength;         // off+688
    mce::Mesh                                      mRcFeedbackOuter;          // off+696
    mce::Mesh                                      mRcFeedbackInner;          // off+1232
    mce::Mesh                                      mVignette;                 // off+1768
    mce::MaterialPtr                               mInvFillMat;               // off+2304
    mce::MaterialPtr                               mCursorMat;                // off+2320
    Bedrock::NonOwnerPointer<DevConsoleLogger>     mDevConsoleLogger;         // off+2336
    std::chrono::steady_clock::time_point          mLastTickTime;             // off+2352
    std::map<std::string, std::vector<GuiMessage>> mDelayedMessages;          // off+2360
    std::vector<std::string>                       mQueuedDevConsoleMessages; // off+2376
    std::mutex                                     mQueuedDevMessagesMutex;   // off+2400
    uint8_t                                        mUnk2480;                  // off+2480
    uint32_t                                       mUnk2484[2];               // off+2484
    uint32_t                                       mUnk2492[2];               // off+2492
    uint8_t                                        mUnk2500;                  // off+2500

#    elif MC_VERSION == v1_21_50
    uint32_t                                       mUnk656[13];               // off+656
    uint32_t                                       mServerSettingsId;         // off+708
    std::string                                    mServerSettings;           // off+712
    bool                                           mMuteChat;                 // off+744
    float                                          mCurrentDropTicks;         // off+748
    PlayerInventory::SlotData                      mCurrentDropSlot;          // off+752
    PlayerInventory::SlotData                      mLastSelectedSlot;         // off+760
    bool                                           mShowProgress;             // off+768
    std::string                                    mTipMessage;               // off+776
    float                                          mTipMessageLength;         // off+808
    mce::Mesh                                      mRcFeedbackOuter;          // off+816
    mce::Mesh                                      mRcFeedbackInner;          // off+1376
    mce::Mesh                                      mVignette;                 // off+1936
    mce::MaterialPtr                               mInvFillMat;               // off+2496
    mce::MaterialPtr                               mCursorMat;                // off+2512
    Bedrock::NonOwnerPointer<DevConsoleLogger>     mDevConsoleLogger;         // off+2528
    std::chrono::steady_clock::time_point          mLastTickTime;             // off+2552
    std::map<std::string, std::vector<GuiMessage>> mDelayedMessages;          // off+2560
    std::vector<std::string>                       mQueuedDevConsoleMessages; // off+2576
    std::mutex                                     mQueuedDevMessagesMutex;   // off+2600
    uint8_t                                        mUnk2680;                  // off+2680
    uint32_t                                       mUnk2688[2];               // off+2684
    uint32_t                                       mUnk2696[2];               // off+2692
    uint8_t                                        mUnk2700;                  // off+2700
#    endif

    SDK_API GuiData(IClientInstance &client);

    SPHR_DECL_API("1.21.2", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x4C\x8B\xFA\x48\x8B\xF1\x48\x89\x4D\x00\x45\x33\xE4")
    SPHR_DECL_API("1.21.50", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4C\x8B\xE2\x48\x8B\xF9\x48\x89\x4D\x00\x45\x33\xED")
    SPHR_CTOR_ALIAS SDK_API GuiData *ctor(IClientInstance &client);
};

#elif MC_VERSION == v1_21_60

// size: 2728
class GuiData : public IConfigListener,
                public /*private*/ AppPlatformListener,
                public Bedrock::EnableNonOwnerReferences {
public:
    ScreenSizeData           mScreenSizeData;                    // off+64
    bool                     mScreenSizeDataValid;               // off+88
    float                    mGuiScale /*= 3.0f*/;               // off+92
    float                    mInvGuiScale /*= 0.33333334f*/;     // off+96
    bool                     mIsCurrentlyActive;                 // off+100
    std::set<int>            mPostedErrors;                      // off+104
    MenuPointer              mMenuPointer;                       // off+120
    int16_t                  mPointerX;                          // off+122
    int16_t                  mPointerY;                          // off+124
    bool                     mHasShowPreexistingMessages;        // off+126
    bool                     mToolbarWasRendered;                // off+127
    int32_t                  mPrevSelectedSlot;                  // off+128
    ContainerID              mPrevSelectedInventoryContainer;    // off+132
    int32_t                  mNumSlots;                          // off+136
    int32_t                  mFlashSlotId;                       // off+140
    double                   mFlashSlotStartTime;                // off+144
    IClientInstance         &mClient;                            // off+152
    RectangleArea            mToolbarArea;                       // off+160
    RectangleArea            mToolbarAreaContainer;              // off+168
    std::string              mLastPopupText;                     // off+192
    std::string              mLastPopupSubtitleText;             // off+224
    std::string              mLastJukeboxPopupText;              // off+256
    std::string              mLastJukeboxPopupSubtitleText;      // off+288
    int32_t                  mTickCount /*= 0*/;                 // off+320
    float                    mItemNameOverlayTime /*= 2.0f*/;    // off+324
    float                    mJukeboxNameOverlayTime /*= 3.0f*/; // off+328
    bool                     mPopupNoticeDirty;                  // off+332
    bool                     mJukeboxPopupNoticeDirty;           // off+333
    std::vector<GuiMessage>  mGuiMessages;                       // off+336
    std::vector<std::string> mDevConsoleMessages;                // off+360
    int32_t                  mMaxDevConsoleMessages /*= 100*/;   // off+384
    std::vector<std::string> mContentLogMessages;                // off+392
    std::vector<std::string> mPerfTurtleMessages;                // off+416
    uint64_t                 mUnk440;                            // off+440
    TitleMessage             mTitleMessage;                      // off+448

    uint32_t                                       mUnk656[13];               // off+680
    uint32_t                                       mServerSettingsId;         // off+732
    std::string                                    mServerSettings;           // off+736
    bool                                           mMuteChat;                 // off+768
    float                                          mCurrentDropTicks;         // off+772
    PlayerInventory::SlotData                      mCurrentDropSlot;          // off+776
    PlayerInventory::SlotData                      mLastSelectedSlot;         // off+784
    bool                                           mShowProgress;             // off+792
    std::string                                    mTipMessage;               // off+800
    float                                          mTipMessageLength;         // off+832
    mce::Mesh                                      mRcFeedbackOuter;          // off+840
    mce::Mesh                                      mRcFeedbackInner;          // off+1400
    mce::Mesh                                      mVignette;                 // off+1960
    mce::MaterialPtr                               mInvFillMat;               // off+2520
    mce::MaterialPtr                               mCursorMat;                // off+2536
    Bedrock::NonOwnerPointer<DevConsoleLogger>     mDevConsoleLogger;         // off+2552
    std::chrono::steady_clock::time_point          mLastTickTime;             // off+2576
    std::map<std::string, std::vector<GuiMessage>> mDelayedMessages;          // off+2584
    std::vector<std::string>                       mQueuedDevConsoleMessages; // off+2592
    std::mutex                                     mQueuedDevMessagesMutex;   // off+2624
    uint8_t                                        mUnk2680;                  // off+2704
    uint32_t                                       mUnk2688[2];               // off+2708
    uint32_t                                       mUnk2696[2];               // off+2716
    uint8_t                                        mUnk2700;                  // off+2724

    SDK_API GuiData(IClientInstance &client);

    SPHR_DECL_API("1.21.60", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x4C\x8B\xE2\x48\x8B\xF1\x48\x89\x4D\x00\x45\x33\xED")
    SPHR_CTOR_ALIAS SDK_API GuiData *ctor(IClientInstance &client);
};
static_assert(sizeof(GuiData) == 2728);

#endif