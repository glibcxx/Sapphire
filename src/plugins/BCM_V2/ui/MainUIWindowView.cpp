#include "MainUIWindowView.h"
#include "SDK/api/sapphire/ui/UI.h"

namespace BCM_V2::ui {

    namespace ui = sapphire::ui;

    void mainWindowView(MainUIWindowViewModel &vm) {
        ui::button(vm.playingButtonText(), [&]() { vm.togglePlaying(); });
        ui::sameLine();
        ui::button("Reset (R)", [&]() { vm.resetTime(); });
        ui::textF("Tick: {}", vm.getTimelineTick());
        ui::separator();
        timeline(vm.getTimelineWidget());
    }

    void timeline(TimelineWidgetViewModel &vm) {
        constexpr auto flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        ui::canvas("Timeline", {vm.getRestAreaSize().x, 60}, flags, [&](const ui::CanvasContext &canvas) {
            canvas.drawRectangle({0, 0}, canvas.size(), vm.backgroundColorU32());
            const auto rulerColor = vm.rulerColorU32();
            for (auto &&tick : vm.getRulerRenderDatas(canvas.size().x)) {
                canvas.drawLine(
                    {tick.localPosX, tick.isMajor ? 0.0f : canvas.size().y * 0.6f},
                    {tick.localPosX, canvas.size().y},
                    rulerColor
                );
                if (tick.isMajor) {
                    canvas.drawText(
                        {tick.localPosX + 2.0f, 0.0f},
                        rulerColor,
                        std::to_string(tick.tick)
                    );
                }
            }
        });
    }

} // namespace BCM_V2::ui
