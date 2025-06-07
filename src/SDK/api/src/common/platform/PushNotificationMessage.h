#pragma once

#include <string>

enum class PushNotificationType : int {
    Achievement = 0,
    MultiplayerInvite = 1,
    MultiplayerInviteRaw = 2,
    FocusLost = 3,
    BrazeModalDialog = 4,
    BrazeToast = 5,
    Toast = 6,
    DeepLink = 7,
    RemoveRequest = 8,
    Unknown = 99,
}; // PushNotificationType

// size: 88
class PushNotificationMessage {
public:
    PushNotificationType m_Type;           // off+0
    std::string          m_Title;          // off+8
    std::string          m_Description;    // off+40
    uint64_t             m_PropertyBag[2]; // off+72, Json::Value
};
static_assert(sizeof(PushNotificationMessage) == 88);
