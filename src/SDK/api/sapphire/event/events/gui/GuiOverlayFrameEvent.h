#pragma once

#include "../../Event.h"

namespace sapphire::event {

    class GuiOverlayFrameEvent : public Event {
    public:
        bool showingLogWindow;
        bool showingToast;
        bool showingPannel;

        GuiOverlayFrameEvent(bool showingLogWindow, bool showingToast, bool showingPannel) :
            showingLogWindow(showingLogWindow),
            showingToast(showingToast),
            showingPannel(showingPannel) {}
    };

}; // namespace sapphire::event