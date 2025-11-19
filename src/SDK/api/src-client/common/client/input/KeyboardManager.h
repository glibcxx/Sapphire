#pragma once

#include "SDK/api/src/common/world/phys/Vec2.h"
#include "SDK/api/src-deps/Input/RectangleArea.h"

class KeyboardManager {
public:
    // vtb+0
    virtual ~KeyboardManager();

    // vtb+1
    virtual bool tryEnableKeyboard(
        const std::string &currentText,
        int                maxLength,
        bool               limitInput,
        bool               numbersOnly,
        bool               isMultiline,
        const Vec2        &position,
        float              controlHeight
    ) = 0;

    // vtb+2
    virtual bool isFullScreenKeyboard() const = 0;

    // vtb+3
    virtual void disableKeyboard() = 0;

    // vtb+4
    virtual bool canActivateKeyboard() = 0;

    // vtb+5
    virtual bool isKeyboardEnabled() const = 0;

    // vtb+6
    virtual bool isKeyboardActive() const = 0;

    // vtb+7
    virtual bool wasEnabledWithMultiline() const = 0;

    // vtb+8
    virtual bool wasEnabledWithNumbersOnly() const = 0;

    // vtb+9
    virtual int getMaxLength() const = 0;

    // vtb+10
    virtual float getKeyboardHeight() const = 0;

    // vtb+11
    virtual void setForcedHeight(float height) = 0;

    // vtb+12
    virtual bool tryClaimKeyboardOwnership() = 0;

    // vtb+13
    virtual void releaseKeyboardOwnership() = 0;

    // vtb+14
    virtual bool getKeyboardClosedEvent() = 0;

    // vtb+15
    virtual void clearKeyboardClosedEvent() = 0;

    // vtb+16
    virtual void updateTextEditBoxPosition(
        const RectangleArea &controlPosition, const Vec2 &caretPosition, const Vec2 &caretSize
    ) = 0;
};