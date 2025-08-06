#pragma once

#include "CameraPath.h"
#include "CamTimeline.h"

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

    private:
        CameraPath  mPath;
        CamTimeline mTimeline;

        bool mIsPlaying = false;
        bool mEnabled = false;
    };

} // namespace BCM_V2