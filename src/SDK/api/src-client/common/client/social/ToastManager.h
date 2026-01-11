#pragma once

#include "IToastManager.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src-deps/Core/Threading/EnableQueueForMainThread.h"
#include "SDK/api/src/common/resources/ResourcePackListener.h"
#include "ToastMessage.h"

class IClientInstance;
class RectangleArea;
struct ToastIconData;

// size: 696 (1.21.50)
class ToastManager : public /* off+0 */ IToastManager,
                     public /* off+24 */ Bedrock::Threading::EnableQueueForMainThread,
                     public /* off+40 */ ResourcePackListener,
                     public /* off+48 */ std::enable_shared_from_this<ToastManager> {
public:
    // size: 88
    struct Unk512 {
        ToastManager                         *mToastManager;     // off+0
        std::vector<ToastIconData>            mUnk8;             // off+8
        ToastIconData                        *mUnk32;            // off+32
        std::chrono::steady_clock::time_point mUnk40;            // off+40
        uint64_t                              mUnk48;            // off+48
        uint32_t                              mUnk56 = 800;      // off+56
        uint32_t                              mUnk60 = 400;      // off+60
        uint32_t                              mUnk64 = 1500;     // off+64
        uint32_t                              mUnk68 = 4;        // off+68
        uint32_t                              mUnk72 = 30;       // off+72
        float                                 mUnk76 = 3195.0f;  // off+76
        float                                 mUnk80 = 12000.0f; // off+80
        bool                                  mUnk84 = true;     // off+84

        Unk512 *ctor(ToastManager &toastManager, std::vector<ToastIconData> a2); // \xE8\x00\x00\x00\x00\x4C\x8B\xF8\x48\x8B\x9F
    };
    static_assert(sizeof(Unk512) == 88);

    IClientInstance                      &mClient;                 // off+64
    int                                   mUnk72;                  // off+72
    Bedrock::NotNullNonOwnerPtr<uint64_t> mUnk80;                  // off+80
    std::optional<ToastMessage>           mCurrentToast;           // off+104
    std::vector<ToastMessage>             mToastPopups;            // off+440
    std::vector<int>                      mUnk464;                 // off+464
    std::vector<int>                      mUnk488;                 // off+488
    std::unique_ptr<Unk512>               mUnk512;                 // off+512
    std::function<RectangleArea()>        mAreaBinding;            // off+520
    std::function<bool()>                 mActiveBinding;          // off+584
    bool                                  mToastClicked;           // off+648
    bool                                  mToastsEnabled;          // off+649
    bool                                  mRefreshPendingInvites;  // off+650
    std::chrono::steady_clock::time_point mToastNotificationStart; // off+656
    std::chrono::steady_clock::time_point mToastNotificationEnd;   // off+664
    uint32_t                              mUnk672;                 // off+672
    std::chrono::steady_clock::time_point mUnk680;                 // off+680
    uint64_t                              mUnk688;                 // off+688

    ToastManager *ctor(IClientInstance &client, uint64_t a2, Bedrock::NotNullNonOwnerPtr<uint64_t> a3); // \x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x4D\x8B\xF1\x4D\x8B\xE0

    // off+1
    virtual void pushToast(ToastMessage &&msg); // \x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x4C\x8B\xEA\x4C\x8B\xF1\x45\x33\xFF
};