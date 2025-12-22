#pragma once

#include "CameraPath.h"
#include "CamTimeline.h"

class FreeCameraMod;
class ScreenContext;
class Vec2;
namespace sapphire::input {
    class InputManager;
}

namespace BCM_V2 {

    class Editor {
    public:
        void toggleEditorMode() {
            mEnabled = !mEnabled;
            if (!mEnabled) mIsPlaying = false;
        }

        void togglePlaying() {
            if (!mEnabled) return;
            mIsPlaying = !mIsPlaying;
        }

        bool isPlaying() const { return mIsPlaying; }
        bool isEnabled() const { return mEnabled; }

        bool &getCameraPreview() { return mCameraPreview; }
        bool  getCameraPreview() const { return mCameraPreview; }

        CameraPath        &getPath() { return this->mPath; }
        const CameraPath  &getPath() const { return this->mPath; }
        CamTimeline       &getTimeline() { return this->mTimeline; }
        const CamTimeline &getTimeline() const { return this->mTimeline; }

        bool isMirrorEditing() const { return mMirrorEditing; }
        void setMirrorEditing(bool value) { mMirrorEditing = value; }

        void addKeyFrameAtCurrentFreeCameraPos();

        void update(const Timer &timer);

        void preview(const BCM_V2::CameraState &cam);

        void handleInput(ScreenContext &screenCtx);

        glm::vec3 getScreenPixelDirection(const Vec2 &screenSize, const Vec2 &pos);

        Editor(FreeCameraMod &freeCam);

    private:
        void setupHotkeys();

        CameraPath  mPath;
        CamTimeline mTimeline;

        FreeCameraMod              &mFreeCam;
        sapphire::input::InputManager &mInputManager;

        bool mIsPlaying = false;
        bool mEnabled = false;
        bool mCameraPreview = false; // preview with camera

        bool mMirrorEditing = true;

        bool      mIsDragging = false;
        glm::vec3 mDragOffset;
        float     mDragInitialDistance;
    };

} // namespace BCM_V2