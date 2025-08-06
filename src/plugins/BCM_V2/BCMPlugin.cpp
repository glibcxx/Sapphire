#include "BCMPlugin.h"

#include "SDK/api/sapphire/ui/UIManager.h"
#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/event/EventManager.h"
#include "SDK/api/sapphire/event/events/GameUpdateGraphicEvent.h"
#include "SDK/api/sapphire/event/events/gui/GuiOverlayFrameEvent.h"
#include "SDK/api/sapphire/event/events/RenderLevelEvent.h"
#include "SDK/api/sapphire/input/InputManager.h"
#include "plugins/FreeCamera/FreeCamera.h"

#include "ui/MainUIWindowView.h"

namespace BCM_V2 {

    using namespace sapphire::event;
    using namespace sapphire::input;
    using namespace sapphire::ui;

    BCMPlugin &BCMPlugin::getInstance() {
        static BCMPlugin inst{};
        return inst;
    }

    BCMPlugin::BCMPlugin() :
        mFreeCamPlugin(FreeCameraPlugin::getInstance()) {
        mEditor = std::make_unique<Editor>();
        mViewModel = std::make_unique<ui::MainUIWindowViewModel>(mFreeCamPlugin, *mEditor);

        UIManager::getInstance().registerView("BCM Editor", &ui::mainWindowView, *mViewModel);

        setupEventListeners();
        setupHotkeys();
        setupSettingsMenu();
    }

    BCMPlugin::~BCMPlugin() = default;

    void BCMPlugin::setupEventListeners() {
        auto &eventMgr = EventManager::getInstance();
        eventMgr.registerAutoListener<RenderLevelEvent>([this](RenderLevelEvent &e) {
            // render path
        });
    }

    void BCMPlugin::setupHotkeys() {
        GuiOverlay::registerHotkey(
            {.triggerKey = ImGuiKey_F7,
             .description = "Toggle BCM Edit Mode (V2)",
             .action = [this]() {
                this->mEditor->toggleEditorMode();
                this->mViewModel->mIsOpen = this->mEditor->isEnabled();
             }}
        );

        GuiOverlay::registerHotkey(
            {.triggerKey = ImGuiKey_K,
             .description = "Add key frame (BCM V2)",
             .action = [this]() {
             }}
        );

        GuiOverlay::registerHotkey(
            {.triggerKey = ImGuiKey_Tab,
             .description = "Toggle BCM Editor focus (V2)",
             .action = [this]() {
             }}
        );

        // ... other hotkeys like Play/Pause (P), Reset (R) etc.
    }

    void BCMPlugin::setupSettingsMenu() {
        GuiOverlay::registerPluginSettings(
            {.name = "BCM (V2)",
             .description = "A camera path editor based on MVVM.",
             .drawSettings = [this]() {
             }}
        );
    }

} // namespace BCM_V2