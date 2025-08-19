#include "Editor.h"
#include "plugins/freecamera/FreeCamera.h"
#include "SDK/api/sapphire/GUI/GUI.h"

namespace BCM_V2 {

    void Editor::update(const Timer &timer) {
        mTimeline.update(timer);
    }

    void Editor::addKeyFrameAtCurrentFreeCameraPos() {
        if (!mEnabled) return;
        mPath.addKeyframe(Keyframe{
            .tick = mTimeline.getTick(),
            .position = mFreeCam.mFreeCamPos,
            .orientation = mFreeCam.getFreeCamOrientation(),
        });
    }

    Editor::Editor(FreeCameraPlugin &freeCam) :
        mFreeCam(freeCam) {
        setupHotkeys();
    }

    void Editor::setupHotkeys() {
        GuiOverlay::registerHotkey(
            {.triggerKey = ImGuiKey_K,
             .description = "Add key frame",
             .action = [this]() {
                 Editor::addKeyFrameAtCurrentFreeCameraPos();
             }}
        );

        GuiOverlay::registerHotkey(
            {.triggerKey = ImGuiKey_Tab,
             .description = "Toggle BCM Editor focus",
             .action = [this]() {
             }}
        );
    }

} // namespace BCM_V2
