#pragma once

#include "CameraPath.h"
#include "CamTimeline.h"

class FreeCameraPlugin;

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

        CameraPath        &getPath() { return this->mPath; }
        const CameraPath  &getPath() const { return this->mPath; }
        CamTimeline       &getTimeline() { return this->mTimeline; }
        const CamTimeline &getTimeline() const { return this->mTimeline; }

        void addKeyFrameAtCurrentFreeCameraPos();

        void update(const Timer &timer);

        Editor(FreeCameraPlugin &freeCam);

    private:
        void setupHotkeys();

        CameraPath  mPath;
        CamTimeline mTimeline;

        FreeCameraPlugin &mFreeCam;

        bool mIsPlaying = false;
        bool mEnabled = false;
    };

} // namespace BCM_V2