#include "BCM.h"

#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/sapphire/util/DrawUtils.h"
#include "SDK/api/sapphire/event/EventManager.h"
#include "SDK/api/sapphire/event/events/GameUpdateGraphicEvent.h"
#include "SDK/api/sapphire/event/events/gui/GuiOverlayFrameEvent.h"
#include "SDK/api/sapphire/event/events/RenderLevelEvent.h"

#include "SDK/api/src-client/common/client/game/ClientInstance.h"

using namespace sapphire::event;

AutoListener<GameUpdateGraphicEvent> updateGraphicListener;
AutoListener<GuiOverlayFrameEvent>   overlayFrameListener;
AutoListener<RenderLevelEvent>       renderLevelListener;

BedrockCameraMovementsPlugin &BedrockCameraMovementsPlugin::getInstance() {
    static BedrockCameraMovementsPlugin inst{};
    return inst;
}

BedrockCameraMovementsPlugin::BedrockCameraMovementsPlugin() :
    mFreeCam(FreeCameraPlugin::getInstance()) {
    mEditor = std::make_unique<BCMEditor>(*this);
    _setupSettingMenu();
    auto &eventMgr = EventManager::getInstance();
    updateGraphicListener = eventMgr.registerAutoListener<GameUpdateGraphicEvent>(
        [this](GameUpdateGraphicEvent &e) {
            if (!mInEditMode) return;
            mEditor->update(e.mTimer);

            if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !mEditor->isUIFocused()) {
                mEditor->handleClick(); // WIP
            }

            if (ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                mEditor->mIsDragging = false;
            }

            this->update(e.mTimer.mAlpha);
        }
    );
    overlayFrameListener = eventMgr.registerAutoListener<GuiOverlayFrameEvent>(
        [this](GuiOverlayFrameEvent &e) {
            this->renderHUD(e);
        }
    );
    renderLevelListener = eventMgr.registerAutoListener<RenderLevelEvent>(
        [this](RenderLevelEvent &e) {
            mEditor->handleInput(*e.mScreen);
            if (mInEditMode) mEditor->getPath().renderPath(mEditor->mHoveredKeyframeTick, mEditor->mHoveredMode, mEditor->mSelectedKeyframeTick, mEditor->mSelectedMode);
        }
    );
}

BedrockCameraMovementsPlugin::~BedrockCameraMovementsPlugin() = default;

void BedrockCameraMovementsPlugin::update(float alpha) {
    if (mEditor->isPlaying()) {
        auto state = mEditor->getPath().getCameraState(mEditor->getTimeline().getTick(), alpha);
        if (state)
            this->setCameraPos(*state);
        return;
    }
    if (!mEditor->isScrubbing()) return;

    auto &keyframes = mEditor->getPath();
    if (keyframes.size() < 2) return;

    size_t current_tick = mEditor->getTimeline().getTick();
    if (current_tick >= keyframes.front().tick && current_tick <= keyframes.back().tick) {
        auto state = mEditor->getPath().getCameraState(mEditor->getTimeline().getTick(), 0.0f);
        if (state)
            this->setCameraPos(*state);
    }
}

void BedrockCameraMovementsPlugin::setCameraPos(CameraState &cam) {
    mFreeCam.mFreeCamPos = cam.position;
    mFreeCam.setFreeCamOrientation(cam.orientation);
}

void BedrockCameraMovementsPlugin::addKeyFrameAtCurrentCameraPos() {
    if (!mInEditMode) return;
    mEditor->addKeyFrame(Keyframe{
        .tick = mEditor->getTimeline().getTick(),
        .position = mFreeCam.mFreeCamPos,
        .orientation = mFreeCam.getFreeCamOrientation(),
    });
}

void BedrockCameraMovementsPlugin::setEditMode(bool enable) {
    if (mInEditMode == enable) {
        return;
    }
    mInEditMode = enable;
    this->mEditor->enableEditor(enable);
    mFreeCam.enableFreeCamera(enable, !enable);
}

void BedrockCameraMovementsPlugin::setPlaying(bool playing) {
    this->mEditor->setPlaying(playing);
}

void BedrockCameraMovementsPlugin::renderHUD(GuiOverlayFrameEvent &e) {
    mEditor->render();
}

void BedrockCameraMovementsPlugin::_setupSettingMenu() {
    GuiOverlay::registerPluginSettings(
        {.name = "BCM",
         .description = "Bedrock Camera Movements Plugin",
         .drawSettings = [this]() { this->_onRenderSetting(); }}
    );
    GuiOverlay::registerHotkey(
        {.triggerKey = ImGuiKey_F7,
         .description = "Toggle BCM Edit Mode",
         .action = [this]() {
             this->setEditMode(!mInEditMode);
         }}
    );
    GuiOverlay::registerHotkey(
        {.triggerKey = ImGuiKey_K,
         .description = "Add key frame at current camera position",
         .action = [this]() {
             if (!mInEditMode || ClientInstance::primaryClientInstance->isShowingMenu()) return;
             this->addKeyFrameAtCurrentCameraPos();
         }}
    );
}

void BedrockCameraMovementsPlugin::_onRenderSetting() {
    bool currEnableEditorMode = mInEditMode;
    if (ImGui::Checkbox("Enable Edit Mode (F7)", &currEnableEditorMode)) {
        this->setEditMode(currEnableEditorMode);
    }
    if (mInEditMode) {
        ImGui::Checkbox("Show BCM Editor (F8)", &mEditor->mShowEditor);
        ImGui::Separator();
        auto &path = mEditor->getPath();
        ImGui::Checkbox("Show Path Visualization (F6)", &path.mShowPath);
        if (path.mShowPath) {
            ImGui::SliderInt("Path Density", &path.mPathDensity, 1, 20, "%d ticks");
        }
    }
}
