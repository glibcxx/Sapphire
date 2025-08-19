#include "BCMPlugin.h"

#include "SDK/api/sapphire/ui/UIManager.h"
#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/event/EventManager.h"
#include "SDK/api/sapphire/event/events/GameUpdateGraphicEvent.h"
#include "SDK/api/sapphire/event/events/gui/GuiOverlayFrameEvent.h"
#include "SDK/api/sapphire/event/events/RenderLevelEvent.h"
#include "SDK/api/sapphire/input/InputManager.h"
#include "SDK/api/sapphire/service/Service.h"
#include "plugins/FreeCamera/FreeCamera.h"

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

    BCMPlugin &BCMPlugin::getInstance() {
        static BCMPlugin inst{};
        return inst;
    }

    BCMPlugin::BCMPlugin() :
        mFreeCamPlugin(FreeCameraPlugin::getInstance()) {
        mEditor = std::make_unique<Editor>(mFreeCamPlugin);
        mViewModel = std::make_unique<ui::MainUIWindowViewModel>(mFreeCamPlugin, *mEditor);

        UIManager::getInstance().registerView("BCM Editor", &ui::mainWindowView, *mViewModel);

        setupEventListeners();
        setupHotkeys();
        setupSettingsMenu();
    }

    BCMPlugin::~BCMPlugin() = default;

    void BCMPlugin::setupEventListeners() {
        auto &eventMgr = EventManager::getInstance();
        updateGraphicListener = eventMgr.registerAutoListener<GameUpdateGraphicEvent>(
            [this](GameUpdateGraphicEvent &e) {
                if (mViewModel->isPlaying())
                    mEditor->update(e.mTimer);
                mViewModel->update(e.mTimer.mAlpha);
            }
        );
        renderEvent = eventMgr.registerAutoListener<RenderLevelEvent>([this](RenderLevelEvent &e) {
            // render path
        });
    }

    void BCMPlugin::setupHotkeys() {
        GuiOverlay::registerHotkey(
            {.triggerKey = ImGuiKey_F7,
             .description = "Toggle BCM Edit Mode",
             .action = [this]() {
                 if (auto mc = getMinecraft(); mc && mc->mGameSession) {
                     this->mEditor->toggleEditorMode();
                     this->mViewModel->mIsOpen = this->mEditor->isEnabled();
                     this->mFreeCamPlugin.enableFreeCamera(this->mEditor->isEnabled(), false);
                     TextPacket packet = TextPacket::createRaw(std::format("BCM: {}", this->mEditor->isEnabled() ? "ON" : "OFF"));
                     packet.mType = TextPacketType::Tip;
                     mc->mGameSession->mLegacyClientNetworkHandler->handle({}, packet);
                 } else if (this->mEditor->isEnabled()) {
                     this->mEditor->toggleEditorMode();
                     this->mViewModel->mIsOpen = false;
                 }
             }}
        );
    }

    void BCMPlugin::setupSettingsMenu() {
        GuiOverlay::registerPluginSettings(
            {.name = "BCM",
             .description = "A camera path editor based on MVVM.",
             .drawSettings = [this]() {
             }}
        );
    }

    Minecraft *BCMPlugin::getMinecraft() {
        if (!mClientMinecraft) {
            mClientMinecraft = sapphire::getClientMinecraft().access();
        }
        return mClientMinecraft;
    }

} // namespace BCM_V2