#pragma once

#include "SDK/core/ApiManager.h"
#include "ToastIconData.h"
#include "SDK/api/src-deps/json/Value.h"

enum class ToastMessageType : int {
    Unknown = 0,
    Achievement = 1,
    Invite = 2,
    RawInvite = 3,
    FocusLost = 4,
    ImportResourcePack = 5,
    ImportExportWorld = 6,
    GlobalResourceCrashRecovery = 7,
    Message = 8,
    PlayerMessaging = 9,
    DialogMessage = 10,
    ChatScreenNotification = 11,
    Debug = 12,
    RecipeUnlocked = 13,
    Snackbar = 14,
};

// size: 328 (1.21.50)
class ToastMessage {
public:
    uint64_t                   mUnk0;          // off+0
    uint8_t                    mUnk8;          // off+8
    bool                       mUnk9;          // off+9
    bool                       mUnk10;         // off+10
    uint8_t                    mUnk11;         // off+11
    ToastMessageType           mType;          // off+12
    std::string                mTitle;         // off+16
    std::string                mSubtitle;      // off+48
    std::string                mUnk104;        // off+80
    Json::Value                mPropertyBag;   // off+112
    std::string                mUnk128;        // off+128
    uint32_t                   mUnk160;        // off+160
    uint8_t                    mUnk164;        // off+164
    float                      mUnk168 = 1.0f; // off+168
    float                      mUnk172 = 0.4f; // off+172
    float                      mUnk176;        // off+176
    float                      mUnk180 = 1.0f; // off+180
    float                      mUnk184;        // off+184
    uint32_t                   mUnk188[12];    // off+188
    std::vector<ToastIconData> mUnk240;        // off+240
    std::string                mUnk264;        // off+264
    std::string                mUnk296;        // off+296

    ToastMessage(
        ToastMessageType   type,
        const std::string &title,
        const std::string &subtitle,
        const Json::Value &propertyBag,
        float              a5
    ) {
        this->ctor(type, title, subtitle, propertyBag, a5);
    }

    SDK_API ToastMessage *ctor(
        ToastMessageType   type,
        const std::string &title,
        const std::string &subtitle,
        const Json::Value &propertyBag,
        float              a5
    );
    MARK_HOOKABLE(&ToastMessage::ctor)

    ToastMessage(const ToastMessage &) = delete;
    ToastMessage(ToastMessage &&) = delete;

    ToastMessage *ctor(ToastMessage &&other);           // \xE8\x00\x00\x00\x00\x48\x81\xC6\x00\x00\x00\x00\x48\x81\xC3\x00\x00\x00\x00\x49\x3B\xDE
    ToastMessage *ctor(const ToastMessage &other);      // \xE8\x00\x00\x00\x00\xC6\x87\x00\x00\x00\x00\x00\x48\x8B\xB7\x00\x00\x00\x00\x4C\x8B\xB7
    ToastMessage &operator=(const ToastMessage &other); // \xE8\x00\x00\x00\x00\xEB\x00\x48\x83\xC1\x00\xE8\x00\x00\x00\x00\xC6\x87\x00\x00\x00\x00\x00\x48\x8B\xB7

    void dtor() noexcept; // \xE8\x00\x00\x00\x00\x4C\x89\x7B\x00\x48\x8B\x7B (1.21.50)
};
static_assert(sizeof(ToastMessage) == 328);