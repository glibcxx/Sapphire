#pragma once

#include "SDK/api/src/common/world/phys/Vec2.h"
#include "SDK/api/src-client/common/client/input/enums/DirectionId.h"
#include "SDK/api/src-client/common/client/input/enums/FocusImpact.h"
#include "SDK/api/src-deps/Core/Utility/NonOwnerPointer.h"
#include "SDK/api/src-deps/Input/InputMode.h"

class ScreenContext;
class FrameRenderObject;
class RectangleArea;
class CachedScenes;
class TaskGroup;
class AbstractSceneProxy;
class MinecraftUIFrameUpdateContext;
struct TextCharEventData;
struct PointerLocationEventData;
enum class EyeRenderingModeBit : uint32_t;
enum class RawInputType : char;
enum class ButtonState : char;
namespace ui {
    enum class SceneType : uint32_t;
}
namespace OreUI {
    struct ViewDebugSettings;
    struct ISceneDebugDataProvider;
} // namespace OreUI

// size: 24
struct ScreenSizeData {
    Vec2 totalScreenSize;    // off+0
    Vec2 clientScreenSize;   // off+8
    Vec2 clientUIScreenSize; // off+16
};

// https://github.com/LiteLDev/LeviLamina/blob/08e88a0c098ce030fa5fbe35c18af45cd46fa794/src-client/mc/client/gui/screens/AbstractScene.h#L36

// size: 16
class AbstractScene {
public:
    __int32 mUnk8; // off+8

    // vtb+0
    virtual ~AbstractScene();

    // vtb+1
    virtual void init(const ScreenSizeData &screenSizeData) = 0;

    // vtb+2
    virtual void setSize(const ScreenSizeData &screenSizeData) = 0;

    // vtb+3
    virtual void onSetKeyboardHeight(float keyboardHeight) = 0;

    // vtb+4
    virtual void onInternetUpdate() = 0;

    // vtb+5
    virtual std::vector<RectangleArea> getInputAreas() const = 0;

    virtual void unk_virtual_6();
    virtual void unk_virtual_7();
    virtual void unk_virtual_8();
    virtual void unk_virtual_9();
    virtual void unk_virtual_10();
    virtual void unk_virtual_11();
    virtual void unk_virtual_12();
    virtual void unk_virtual_13();
    virtual void unk_virtual_14();

    // vtb+15
    virtual void tick(int nTick, int maxTick) = 0;

    // vtb+16
    virtual void updateEvents() = 0;

    // vtb+17
    virtual void applyInput(float a) = 0;

    // vtb+18
    virtual void frameUpdate(MinecraftUIFrameUpdateContext &) = 0;

    // vtb+19
    virtual void preRenderUpdate(ScreenContext &screenContext) = 0;

    // vtb+20
    virtual void prepareFrame(ScreenContext &screenContext) = 0;

    // vtb+21
    virtual void render(ScreenContext &screenContext, const FrameRenderObject &renderObj) = 0;

    // vtb+22
    virtual void postRenderUpdate(ScreenContext &screenContext) = 0;

    // vtb+23
    virtual void setupAndRender(ScreenContext &screenContext) = 0;

    // vtb+24
    virtual void handleInputModeChanged(InputMode inputMode) = 0;

    // vtb+25
    virtual void handleHoloInputModeChanged(HoloUIInputMode inputMode) = 0;

    // vtb+26
    virtual void handleButtonPress(uint32_t buttonId, FocusImpact focusImpact) = 0;

    // vtb+27
    virtual void handleButtonRelease(uint32_t buttonId, FocusImpact focusImpact) = 0;

    // vtb+28
    virtual void handleRawInputEvent(int id, RawInputType keyType, ButtonState state, bool allowRemapping) = 0;

    // vtb+29
    virtual bool handlePointerLocation(const PointerLocationEventData &pointerLocationData, FocusImpact focusImpact) = 0;

    // vtb+30
    virtual void handlePointerPressed(bool pressed) = 0;

    // vtb+31
    virtual void handleDirection(DirectionId directionId, float x, float y, FocusImpact focusImpact) = 0;

    // vtb+32
    virtual bool handleBackEvent(bool isDown) = 0;

    virtual void unk_virtual_33();
    virtual void unk_virtual_34();
    virtual void unk_virtual_35();
    virtual void unk_virtual_36();
    virtual void unk_virtual_37();
    virtual void unk_virtual_38();
    virtual void unk_virtual_39();
    virtual void unk_virtual_40();
    virtual void unk_virtual_41();
    virtual void unk_virtual_42();
    virtual void unk_virtual_43();
    virtual void unk_virtual_44();
    virtual bool unk_virtual_45();
    virtual bool unk_virtual_46();
    virtual bool unk_virtual_47();
    virtual bool unk_virtual_48();
    virtual bool unk_virtual_49();
    virtual bool unk_virtual_50();
    virtual bool unk_virtual_51();

    // vtb+52
    virtual bool shouldStealMouse() const = 0;

    // vtb+53
    virtual bool screenIsNotFlushable() const = 0;

    // vtb+54
    virtual bool alwaysAcceptsInput() const = 0;

    // vtb+55
    virtual bool screenDrawsLast() const = 0;

    // vtb+56
    virtual bool isPlayScreen() const = 0;

    // vtb+57
    virtual bool renderOnlyWhenTopMost() const = 0;

    // vtb+58
    virtual bool lowFreqRendering() const = 0;

    // vtb+59
    virtual bool ignoreAsTop() const = 0;

    // vtb+60
    virtual bool screenHandlesGamepadMenuButton() const = 0;

    // vtb+61
    virtual bool shouldUpdateWhenSuspended() const = 0;

    // vtb+62
    virtual int getWidth() = 0;

    // vtb+63
    virtual int getHeight() = 0;

    // vtb+64
    virtual void reload() = 0;

    // vtb+65
    virtual const RectangleArea &getRenderingAABB() = 0;

    virtual void unk_virtual_66();

    // vtb+67
    virtual EyeRenderingModeBit getEyeRenderingMode() const = 0;

    // vtb+68
    virtual ui::SceneType getSceneType() const = 0;

    // vtb+69
    virtual std::string getScreenName() const = 0;

    virtual bool unk_virtual_70();

    // vtb+71
    virtual std::string getRoute() const = 0;

    // vtb+72
    virtual std::string getScreenTelemetryName() const = 0;

    // vtb+73
    virtual void addEventProperties(std::unordered_map<std::string, std::string> &properties) const = 0;

    // vtb+74
    virtual int getScreenVersion() const = 0;

    // vtb+75
    virtual void processBufferedTextCharEvents(const std::vector<TextCharEventData> &bufferedEvents) = 0;

    // vtb+76
    virtual bool getShouldSendEvents() = 0;

    // vtb+77
    virtual void setShouldSendEvents(bool sendEvents) = 0;

    // vtb+78
    virtual bool getWantsTextOnly() = 0;

    // vtb+79
    virtual void setWantsTextOnly(bool textOnly) = 0;

    // vtb+80
    virtual void onDelete(CachedScenes &cache, TaskGroup &taskGroup) = 0;

    // vtb+81
    virtual bool isGamepadCursorEnabled() const = 0;

    // vtb+82
    virtual bool isGamepadDeflectionModeEnabled() const = 0;

    // vtb+83
    virtual const glm::tvec2<float> &getGamepadCursorPosition() const = 0;

    // vtb+84
    virtual void cleanInputComponents() = 0;

    // vtb+85
    virtual std::weak_ptr<AbstractSceneProxy> getProxy() = 0;

    // vtb+86
    virtual bool canBePushed() const = 0;

    // vtb+87
    virtual bool canBePopped() const = 0;

    // vtb+88
    virtual bool canBeTransitioned() const = 0;

    // vtb+89
    virtual void onScreenExit(bool isPopping, bool doScreenTransitions, std::shared_ptr<AbstractScene>) = 0;

    // vtb+90
    virtual void onScreenEntrance(bool isRevisiting, bool doScreenTransitions) = 0;

    // vtb+91
    virtual bool isEntering() const = 0;

    // vtb+92
    virtual bool isExiting() const = 0;

    // vtb+93
    virtual void schedulePop() = 0;

    // vtb+94
    virtual bool isTerminating() const = 0;

    // vtb+95
    virtual bool loadScreenImmediately() const = 0;

    // vtb+96
    virtual bool forceUpdateActiveSceneStackWhenPushed() const = 0;

    // vtb+97
    virtual bool hasFinishedLoading() const = 0;

    // vtb+98
    virtual void sendScreenEvent(const std::string &eventName, const std::string &eventData) = 0;

    // vtb+99
    virtual void setScreenState(const std::vector<std::pair<std::string_view, std::string_view>> &routeQueryParameters) = 0;

    // vtb+100
    virtual void setDebugSettings(const OreUI::ViewDebugSettings &) const = 0;

    // vtb+101
    virtual void unk_virtual_101();

#if MC_VERSION == v1_21_2
    virtual void unk_virtual_102();
#endif
};