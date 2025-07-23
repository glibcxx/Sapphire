#pragma once

#include "SDK/api/src-deps/Core/Utility/EnableNonOwnerReferences.h"

class ToastMessage;

// size: 24
class IToastManager : public Bedrock::EnableNonOwnerReferences {
public:
    // vtb+0
    virtual ~IToastManager() = default;

    // vtb+1
    virtual void pushToast(ToastMessage &&msg) = 0;

    // vtb+2
    virtual bool isEditorModeEnabled() const = 0;
};