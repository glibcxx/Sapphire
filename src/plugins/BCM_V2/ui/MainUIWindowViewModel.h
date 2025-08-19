#pragma once

#include "SDK/api/sapphire/ui/WindowViewModel.h"
#include "../editor/CamTimeline.h"
#include "TimelineWidgetViewModel.h"

class FreeCameraPlugin;

namespace BCM_V2::ui {

    struct MainUIWindowViewModel : public sapphire::ui::WindowViewModel {
        MainUIWindowViewModel(FreeCameraPlugin &freeCam, Editor &editor) :
            mFreeCam(freeCam), mTimelineWidget(editor), mEditor(editor) {}

        void update(float alpha) {
            if (isPlaying())
                mTimelineWidget.update(alpha);
        }

        bool isPlaying() const { return mEditor.isPlaying(); }

        size_t getTimelineTick() const {
            return mEditor.getTimeline().getTick();
        }

        float getTimelineAlpha() const {
            return mEditor.getTimeline().getAlpha();
        }

        void togglePlaying() {
            mEditor.togglePlaying();
        }

        void resetTime() {
            mEditor.getTimeline().reset();
        }

        std::string_view playingButtonText() const {
            return mEditor.isPlaying() ? "Pause (P)" : "Play (P)";
        }

        TimelineWidgetViewModel &getTimelineWidget() { return mTimelineWidget; }

    private:
        TimelineWidgetViewModel mTimelineWidget;
        FreeCameraPlugin       &mFreeCam;
        Editor                 &mEditor;
    };

} // namespace BCM_V2::ui