#pragma once

#include "MainUIWindowViewModel.h"
#include "TimelineWidgetViewModel.h"

namespace BCM_V2::ui {

    // 编辑器窗口view
    void mainWindowView(MainUIWindowViewModel &vm);

    void timeline(TimelineWidgetViewModel &vm);

} // namespace BCM_V2::ui