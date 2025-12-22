#include "BCMMod.h"

#include "SDK/api/sapphire/ui/UIManager.h"
#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/event/EventBus.h"
#include "SDK/api/sapphire/event/events/GameUpdateGraphicEvent.h"
#include "SDK/api/sapphire/event/events/gui/GuiOverlayFrameEvent.h"
#include "SDK/api/sapphire/event/events/RenderLevelEvent.h"
#include "SDK/api/sapphire/input/InputManager.h"
#include "SDK/api/sapphire/service/Service.h"
#include "../freeCamera/FreeCamera.h"

#include "SDK/api/src/common/world/Minecraft.h"
#include "SDK/api/src/common/world/GameSession.h"
#include "SDK/api/src/common/network/packet/TextPacket.h"

#include "ui/MainUIWindowView.h"

namespace BCM_V2 {

    using namespace sapphire::event;
    using namespace sapphire::input;
    using namespace sapphire::ui;

    static AutoListener<RenderLevelEvent>       renderEvent;
    static AutoListener<GameUpdateGraphicEvent> updateGraphicListener;

    BCMMod &BCMMod::getInstance() {
        static BCMMod inst{};
        return inst;
    }

    BCMMod::BCMMod() :
        mFreeCamMod(FreeCameraMod::getInstance()) {
        mEditor = std::make_unique<Editor>(mFreeCamMod);
        mViewModel = std::make_unique<ui::MainUIWindowViewModel>(mFreeCamMod, *mEditor);

        UIManager::getInstance().registerView("BCM Editor", &ui::mainWindowView, *mViewModel);

        setupEventListeners();
        setupHotkeys();
        setupSettingsMenu();
    }

    BCMMod::~BCMMod() = default;

    void BCMMod::setupEventListeners() {
        auto &eventMgr = EventBus::getInstance();
        updateGraphicListener = eventMgr.registerAutoListener<GameUpdateGraphicEvent>(
            [this](GameUpdateGraphicEvent &e) {
                if (mViewModel->isPlaying())
                    mEditor->update(e.mTimer);
                mViewModel->update(e.mTimer.mAlpha);
            }
        );
        renderEvent = eventMgr.registerAutoListener<RenderLevelEvent>([this](RenderLevelEvent &e) {
            if (!mEditor->isEnabled())
                return;
            auto &path = mEditor->getPath();
            path.renderPath();
            if (!this->mViewModel->mIsOnFocus)
                mEditor->handleInput(e.mScreen);
        });
    }

    void BCMMod::setupHotkeys() {
        GuiOverlay::registerHotkey(
            {.triggerKey = ImGuiKey_F7,
             .description = "Toggle BCM Edit Mode",
             .action = [this]() {
                 if (auto mc = getMinecraft(); mc && mc->mGameSession) {
                     this->mEditor->toggleEditorMode();
                     this->mViewModel->mIsOpen = this->mEditor->isEnabled();
                     this->mFreeCamMod.enableFreeCamera(this->mEditor->isEnabled(), false);
                     TextPacket packet = TextPacket::createRaw(std::format("BCM: {}", this->mEditor->isEnabled() ? "ON" : "OFF"));
                     packet.mType = TextPacketType::Tip;
                     mc->mGameSession->mLegacyClientNetworkHandler->handle({}, packet);
                 } else if (this->mEditor->isEnabled()) {
                     this->mEditor->toggleEditorMode();
                     this->mViewModel->mIsOpen = false;
                 }
             }}
        );

        GuiOverlay::registerHotkey(
            {.triggerKey = ImGuiKey_Tab,
             .description = "Toggle Focus",
             .action = [this]() {
                 if (!this->mViewModel->mIsOpen)
                     return;
                 this->mViewModel->mIsOnFocus = !this->mViewModel->mIsOnFocus;
             }}
        );
    }

    void BCMMod::setupSettingsMenu() {
        GuiOverlay::registerModSettings(
            {.name = "BCM",
             .description = "BCM",
             .drawSettings = [this]() {
             }}
        );
    }

    Minecraft *BCMMod::getMinecraft() {
        if (!mClientMinecraft) {
            mClientMinecraft = sapphire::getClientMinecraft().access();
        }
        return mClientMinecraft;
    }

} // namespace BCM_V2