#include "BCMEditor.h"
#include "../BCM.h"
#include "../math/Intersection.h"
#include "widgets/KeyframeTableWidget.h"
#include "widgets/TimelineWidget.h"
#include "SDK/api/sapphire/GUI/GUI.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"
#include "SDK/api/src/common/world/phys/AABB.h"
#include "SDK/api/src-client/common/client/gui/screen/ScreenContext.h"

BCMEditor::BCMEditor(BedrockCameraMovementsPlugin &plugin) :
    mPlugin(plugin) {
    mTimelineWidget = std::make_unique<TimelineWidget>(*this);
    mKeyframeTableWidget = std::make_unique<KeyframeTableWidget>(*this);
    registryHotkeys();
}

void BCMEditor::registryHotkeys() {
    GuiOverlay::registerHotkey(
        {.triggerKey = ImGuiKey_F6,
         .description = "Toggle Path Visualization",
         .action = [this]() {
             if (!mEnabled) return;
             mPath.mShowPath = !mPath.mShowPath;
         }}
    );
    GuiOverlay::registerHotkey(
        {.triggerKey = ImGuiKey_F8,
         .description = "Toggle BCM Editor HUD",
         .action = [this]() {
             if (!mEnabled) return;
             mShowEditor = !mShowEditor;
         }}
    );
    GuiOverlay::registerHotkey(
        {.triggerKey = ImGuiKey_Tab,
         .description = "Toggle BCM Editor focus",
         .action = [this]() {
             if (!mEnabled || ClientInstance::primaryClientInstance->isShowingMenu()) return;
             this->setUIFocus(!this->mUIHasFocus);
         }}
    );
    GuiOverlay::registerHotkey(
        {.triggerKey = ImGuiKey_P,
         .description = "Play/Pause camera path",
         .action = [this]() {
             if (!mEnabled || ClientInstance::primaryClientInstance->isShowingMenu()) return;
             this->setPlaying(!mIsPlaying);
         }}
    );
    GuiOverlay::registerHotkey(
        {.triggerKey = ImGuiKey_R,
         .description = "Reset camera timeline",
         .action = [this]() {
             if (!mEnabled || ClientInstance::primaryClientInstance->isShowingMenu()) return;
             mTimeline.reset();
         }}
    );
}

BCMEditor::~BCMEditor() = default;

void BCMEditor::enableEditor(bool enable) {
    if (mEnabled == enable) return;
    mEnabled = enable;
    setUIFocus(mEnabled);
    if (!enable)
        mIsPlaying = false;
}

void BCMEditor::setPlaying(bool playing) {
    if (mIsPlaying == playing || !mEnabled) return;

    if (playing) {
        if (mPath.size() < 2) {
            return;
        }

        if (mTimeline.getTick() >= mPath.back().tick
            || mTimeline.getTick() < mPath.front().tick) {
            mTimeline.setTick(mPath.front().tick);
        }
        setUIFocus(false);
    }

    mIsPlaying = playing;
    if (!mIsPlaying) {
        mIsScrubbing = false;
    }
}

void BCMEditor::setUIFocus(bool focused) {
    if (!mEnabled) return;

    mUIHasFocus = focused;
    if (mUIHasFocus) {
        GuiOverlay::gameReleaseMouse();
    } else {
        GuiOverlay::gameTryGrabMouse();
    }
}

void BCMEditor::handleInput(ScreenContext &screenCtx) {
    if (mIsDragging) {
        handleDrag(screenCtx);
        return;
    }

    if (mUIHasFocus) {
        mHoveredKeyframeTick = std::nullopt;
        mHoveredMode = SelectionMode::None;
        return;
    }

    const auto &freeCam = mPlugin.mFreeCam;
    glm::vec3   rayOrigin = freeCam.mFreeCamPos;
    glm::vec3   rayDir = freeCam.getForwardVector();

    float                 closest_t = std::numeric_limits<float>::max();
    std::optional<size_t> newHoveredTick = std::nullopt;
    SelectionMode         newHoveredMode = SelectionMode::None;

    for (const auto &kf : mPath) {
        float t;

        AABB kfAABB = {kf.position - 0.1f, kf.position + 0.1f};
        if (intersection::rayAABB(rayOrigin, rayDir, kfAABB, t) && t < closest_t) {
            closest_t = t;
            newHoveredTick = kf.tick;
            newHoveredMode = SelectionMode::Keyframe;
        }

        if (kf.interpToNext == InterpolationType::Bezier) {
            AABB inAABB = {kf.controlPointIn - 0.05f, kf.controlPointIn + 0.05f};
            if (intersection::rayAABB(rayOrigin, rayDir, inAABB, t) && t < closest_t) {
                closest_t = t;
                newHoveredTick = kf.tick;
                newHoveredMode = SelectionMode::ControlPointIn;
            }

            AABB outAABB = {kf.controlPointOut - 0.05f, kf.controlPointOut + 0.05f};
            if (intersection::rayAABB(rayOrigin, rayDir, outAABB, t) && t < closest_t) {
                closest_t = t;
                newHoveredTick = kf.tick;
                newHoveredMode = SelectionMode::ControlPointOut;
            }
        }
    }

    mHoveredKeyframeTick = newHoveredTick;
    mHoveredMode = newHoveredMode;
}

void BCMEditor::handleClick() {
    if (mUIHasFocus) return;

    if (mHoveredKeyframeTick.has_value()) {
        mSelectedKeyframeTick = mHoveredKeyframeTick;
        mSelectedMode = mHoveredMode;

        mIsDragging = true;
        mDragPlaneNormal = -mPlugin.mFreeCam.getForwardVector();

        auto it = std::find_if(mPath.begin(), mPath.end(), [this](const Keyframe &kf) {
            return kf.tick == *mSelectedKeyframeTick;
        });

        if (it != mPath.end()) {
            switch (mSelectedMode) {
            case SelectionMode::Keyframe: mDragInitialPos = it->position; break;
            case SelectionMode::ControlPointIn: mDragInitialPos = it->controlPointIn; break;
            case SelectionMode::ControlPointOut: mDragInitialPos = it->controlPointOut; break;
            case SelectionMode::None: mIsDragging = false; break;
            }
            mDragPlaneD = -glm::dot(mDragPlaneNormal, mDragInitialPos);
        } else {
            mIsDragging = false;
        }

    } else {
        mSelectedKeyframeTick = std::nullopt;
        mSelectedMode = SelectionMode::None;
    }
}

void BCMEditor::handleDrag(ScreenContext &screenCtx) {
    const auto &freeCam = mPlugin.mFreeCam;
    glm::vec3   rayOrigin = freeCam.mFreeCamPos;
    glm::vec3   rayDir = this->getScreenPixelDirection(
        Vec2(screenCtx.xm, screenCtx.ym), {ImGui::GetMousePos().x, ImGui::GetMousePos().y}
    );

    float t = -(glm::dot(rayOrigin, mDragPlaneNormal) + mDragPlaneD) / glm::dot(rayDir, mDragPlaneNormal);
    if (t > 0) {
        glm::vec3 newPos = rayOrigin + rayDir * t;

        auto it = std::find_if(mPath.begin(), mPath.end(), [this](const Keyframe &kf) {
            return kf.tick == *mSelectedKeyframeTick;
        });

        if (it != mPath.end()) {
            switch (mSelectedMode) {
            case SelectionMode::Keyframe: it->position = newPos; break;
            case SelectionMode::ControlPointIn: it->controlPointIn = newPos; break;
            case SelectionMode::ControlPointOut: it->controlPointOut = newPos; break;
            case SelectionMode::None: break;
            }
            mPath.recalculateAllHandles();
        }
    }
}

void BCMEditor::render() {
    if (!mEnabled || !mShowEditor) return;
    ImGuiWindowFlags flags = 0;
    ImGui::SetNextWindowSize(ImVec2(600, 400), ImGuiCond_FirstUseEver);
    ImGui::Begin("BCM Editor", &mShowEditor, flags);

    bool can_play = mPath.size() >= 2;
    if (!can_play) {
        ImGui::BeginDisabled();
    }
    if (ImGui::Button(mIsPlaying ? "Pause (P)" : "Play (P)")) {
        setPlaying(!mIsPlaying);
    }
    if (!can_play) {
        ImGui::EndDisabled();
    }
    ImGui::SameLine();
    if (ImGui::Button("Reset Timeline (R)")) {
        mTimeline.reset();
    }

    ImGui::Text("Tick: %zu", mTimeline.getTick());

    if (mTimelineWidget) mTimelineWidget->render();
    if (mKeyframeTableWidget) mKeyframeTableWidget->render();

    ImGui::End();
}

void BCMEditor::update(const Timer &timer) {
    if (mIsPlaying) {
        this->mTimeline.update(timer);
        if (!mPath.empty() && mTimeline.getTick() > mPath.back().tick) {
            setPlaying(false);
            mTimeline.setTick(mPath.back().tick);
        }
    }
}

void BCMEditor::addKeyFrame(Keyframe &&newKeyframe) {
    if (!mPath.empty()) {
        newKeyframe.interpToNext = mPath.back().interpToNext;
    }
    mPath.addKeyframe(newKeyframe);
}

glm::vec3 BCMEditor::getScreenPixelDirection(const Vec2 &screenSize, const Vec2 &pos) {
    if (screenSize.x == 0 || screenSize.y == 0) {
        return mPlugin.mFreeCam.getForwardVector();
    }
    if (!mCameraComponent && ClientInstance::primaryClientInstance) {
        auto *renderCam = ClientInstance::primaryClientInstance->getRenderCameraComponent();
        if (renderCam)
            mCameraComponent = &renderCam->mCamera;
        if (!mCameraComponent)
            return mPlugin.mFreeCam.getForwardVector();
    }

    float ndcX = (2.0f * pos.x) / screenSize.x - 1.0f;
    float ndcY = 1.0f - (2.0f * pos.y) / screenSize.y;

    const glm::mat4 &projMatrix = mCameraComponent->mSavedProjection;
    const glm::mat4 &viewMatrix = mCameraComponent->mPostViewTransform;

    glm::mat4 invViewProj = glm::inverse(projMatrix * viewMatrix);
    glm::vec4 worldPos = invViewProj * glm::vec4(ndcX, ndcY, 1.0, 1.0);

    glm::vec3 worldPosCartesian = glm::vec3(worldPos) / worldPos.w;

    return glm::normalize(worldPosCartesian - mPlugin.mFreeCam.mFreeCamPos);
}