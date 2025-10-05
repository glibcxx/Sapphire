#include "Editor.h"
#include "plugins/freecamera/FreeCamera.h"
#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/src-client/common/client/gui/screen/ScreenContext.h"
#include "SDK/api/sapphire/input/InputManager.h"

#include <limits>
#include "../math/Intersection.h"

namespace BCM_V2 {

    using namespace sapphire::input;

    void Editor::update(const Timer &timer) {
        mTimeline.update(timer);
        if (!mPath.empty() && mTimeline.getTick() >= mPath.back().tick) {
            togglePlaying();
            mTimeline.setTick(mPath.back().tick);
        }
        auto cam = mPath.getCameraState(mTimeline.getTick(), mTimeline.getAlpha());
        if (cam) preview(*cam);
    }

    void BCM_V2::Editor::preview(const BCM_V2::CameraState &cam) {
        if (mCameraPreview) {
            mFreeCam.mFreeCamPos = cam.position;
            mFreeCam.setFreeCamOrientation(cam.orientation);
        } else {
            // todo
        }
    }

    void Editor::addKeyFrameAtCurrentFreeCameraPos() {
        if (!mEnabled) return;
        mPath.addKeyframe(Keyframe{
            .tick = mTimeline.getTick(),
            .position = mFreeCam.mFreeCamPos,
            .orientation = mFreeCam.getFreeCamOrientation(),
        });
    }

    void Editor::handleInput(ScreenContext &screenCtx) {
        if (!mEnabled) return;

        glm::vec3 &rayOrigin = mFreeCam.mFreeCamPos;
        glm::vec3  rayDir = mFreeCam.getForwardVector();
        const bool lmouseDowm = mInputManager.isKeyDown(KeyCode::MouseLeft);
        const bool lmousePressed = mInputManager.isKeyPressed(KeyCode::MouseLeft);

        if (mIsDragging && lmouseDowm) {
            // Dragging
            glm::vec3 rayPoint = rayOrigin + rayDir * mDragInitialDistance;
            glm::vec3 newObjectCenter = rayPoint + mDragOffset;

            if (mPath.mSelectedKeyframeTick.has_value()) {
                auto it = mPath.findKeyframe(*mPath.mSelectedKeyframeTick);
                if (it != mPath.end()) {
                    glm::vec3 oldObjectCenter;
                    if (mPath.mSelectedMode == SelectionMode::Keyframe)
                        oldObjectCenter = it->position;
                    else if (mPath.mSelectedMode == SelectionMode::ControlPointIn)
                        oldObjectCenter = it->controlPointIn;
                    else
                        oldObjectCenter = it->controlPointOut;

                    glm::vec3 offset = newObjectCenter - oldObjectCenter;

                    switch (mPath.mSelectedMode) {
                    case SelectionMode::Keyframe:
                        it->position += offset;
                        it->controlPointIn += offset;
                        it->controlPointOut += offset;
                        break;
                    case SelectionMode::ControlPointIn:
                        it->controlPointIn = newObjectCenter;
                        it->controlPointInIsUserModified = true;
                        if (mMirrorEditing) {
                            it->controlPointOut = it->position - (newObjectCenter - it->position);
                            it->controlPointOutIsUserModified = true;
                        }
                        break;
                    case SelectionMode::ControlPointOut:
                        it->controlPointOut = newObjectCenter;
                        it->controlPointOutIsUserModified = true;
                        if (mMirrorEditing) {
                            it->controlPointIn = it->position - (newObjectCenter - it->position);
                            it->controlPointInIsUserModified = true;
                        }
                        break;
                    default:
                        break;
                    }
                }
            }
        } else {
            mIsDragging = false;

            // Hovering
            mPath.mHoveredKeyframeTick.reset();
            mPath.mHoveredMode = SelectionMode::None;
            float closest_t = std::numeric_limits<float>::max();

            for (auto &kf : mPath) {
                float t = 0;

                AABB kf_aabb = {kf.position - 0.1f, kf.position + 0.1f};
                if (intersection::rayAABB(rayOrigin, rayDir, kf_aabb, t) && t < closest_t) {
                    closest_t = t;
                    mPath.mHoveredKeyframeTick = kf.tick;
                    mPath.mHoveredMode = SelectionMode::Keyframe;
                }

                if (kf.interpToNext == InterpolationType::Bezier) {
                    AABB cp_in_aabb = {kf.controlPointIn - 0.05f, kf.controlPointIn + 0.05f};
                    if (intersection::rayAABB(rayOrigin, rayDir, cp_in_aabb, t) && t < closest_t) {
                        closest_t = t;
                        mPath.mHoveredKeyframeTick = kf.tick;
                        mPath.mHoveredMode = SelectionMode::ControlPointIn;
                    }

                    AABB cp_out_aabb = {kf.controlPointOut - 0.05f, kf.controlPointOut + 0.05f};
                    if (intersection::rayAABB(rayOrigin, rayDir, cp_out_aabb, t) && t < closest_t) {
                        closest_t = t;
                        mPath.mHoveredKeyframeTick = kf.tick;
                        mPath.mHoveredMode = SelectionMode::ControlPointOut;
                    }
                }
            }
        }

        if (lmousePressed) {
            if (mPath.mHoveredKeyframeTick.has_value()) {
                mPath.mSelectedKeyframeTick = mPath.mHoveredKeyframeTick;
                mPath.mSelectedMode = mPath.mHoveredMode;

                // Start dragging
                mIsDragging = true;
                auto it = mPath.findKeyframe(*mPath.mSelectedKeyframeTick);
                if (it != mPath.end()) {
                    glm::vec3 objectCenter;
                    switch (mPath.mSelectedMode) {
                    case SelectionMode::Keyframe:
                        objectCenter = it->position;
                        break;
                    case SelectionMode::ControlPointIn:
                        objectCenter = it->controlPointIn;
                        break;
                    case SelectionMode::ControlPointOut:
                        objectCenter = it->controlPointOut;
                        break;
                    default:
                        mIsDragging = false;
                        break;
                    }

                    if (mIsDragging) {
                        float t;
                        AABB  aabb;
                        if (mPath.mSelectedMode == SelectionMode::Keyframe)
                            aabb = {objectCenter - 0.1f, objectCenter + 0.1f};
                        else
                            aabb = {objectCenter - 0.05f, objectCenter + 0.05f};

                        if (intersection::rayAABB(rayOrigin, rayDir, aabb, t)) {
                            glm::vec3 intersectionPoint = rayOrigin + rayDir * t;
                            mDragOffset = objectCenter - intersectionPoint;
                            mDragInitialDistance = glm::distance(intersectionPoint, rayOrigin);
                        } else {
                            mIsDragging = false;
                        }
                    }
                }

            } else {
                mPath.mSelectedKeyframeTick.reset();
                mPath.mSelectedMode = SelectionMode::None;
                mIsDragging = false;
            }
        }
    }

    glm::vec3 Editor::getScreenPixelDirection(const Vec2 &screenSize, const Vec2 &pos) {
        return {};
    }

    Editor::Editor(FreeCameraPlugin &freeCam) :
        mFreeCam(freeCam), mInputManager(InputManager::getInstance()) {
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
