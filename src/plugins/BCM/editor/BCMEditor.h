#pragma once

#include <memory>
#include "CamTimeline.h"
#include "CameraPath.h"
#include "SDK/api/src/common/world/phys/Vec2.h"

class BedrockCameraMovementsPlugin;
class TimelineWidget;
class KeyframeTableWidget;
class ScreenContext;
class CameraComponent;

enum class SelectionMode {
    None,
    Keyframe,
    ControlPointIn,
    ControlPointOut
};

class BCMEditor {
public:
    explicit BCMEditor(BedrockCameraMovementsPlugin &plugin);
    ~BCMEditor();

    void enableEditor(bool enable);

    void setPlaying(bool playing);
    void togglePlaying() { setPlaying(!mIsPlaying); }

    bool isPlaying() const { return mIsPlaying; }
    bool isScrubbing() const { return mIsScrubbing; }
    void setScrubbing(bool scrubbing) { mIsScrubbing = scrubbing; }

    void setUIFocus(bool focused);
    void toggleUIFocus() { setUIFocus(!mUIHasFocus); }
    bool isUIFocused() const { return mUIHasFocus; }

    void handleInput(ScreenContext &screenCtx);
    void handleClick();
    void handleDrag(ScreenContext &screenCtx);

    // render one frame
    void render();

    // 刷新时间轴（播放时）
    void update(const Timer &timer);

    void addKeyFrame(Keyframe &&newKeyframe);

    BedrockCameraMovementsPlugin &getPlugin() { return mPlugin; }
    CameraPath                   &getPath() { return mPath; }
    const CameraPath             &getPath() const { return mPath; }

    CamTimeline       &getTimeline() { return mTimeline; }
    const CamTimeline &getTimeline() const { return mTimeline; }

    glm::vec3 getScreenPixelDirection(const Vec2 &screenSize, const Vec2 &pos);

private:
    void registryHotkeys();

    BedrockCameraMovementsPlugin &mPlugin;
    CamTimeline                   mTimeline;
    CameraPath                    mPath;

    std::unique_ptr<TimelineWidget>      mTimelineWidget;
    std::unique_ptr<KeyframeTableWidget> mKeyframeTableWidget;

    CameraComponent *mCameraComponent = nullptr;

public:
    // Selection state
    std::optional<size_t> mHoveredKeyframeTick;
    SelectionMode         mHoveredMode = SelectionMode::None;

    std::optional<size_t> mSelectedKeyframeTick;
    SelectionMode         mSelectedMode = SelectionMode::None;

    // Dragging state
    bool mIsDragging = false;
    glm::vec3 mDragInitialPos{};
    glm::vec3 mDragPlaneNormal{};
    float mDragPlaneD = 0.0f;

private:
    bool mEnabled = false;
    bool mIsPlaying = false;
    bool mUIHasFocus = false; // This will be handled by the framework later
    bool mIsScrubbing = false;

public:
    bool mShowEditor = true;
};