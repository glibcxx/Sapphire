#include "MainUIWindowView.h"
#include "SDK/api/sapphire/ui/UI.h"

namespace BCM_V2::ui {

    namespace ui = sapphire::ui;

    void mainWindowView(MainUIWindowViewModel &vm) {
        ui::button(vm.playingButtonText(), [&]() { vm.togglePlaying(); });
        ui::sameLine();
        ui::button("Reset", [&]() { vm.resetTime(); });
        ui::sameLine();
        ui::checkbox("Preview with Camera", vm.mCameraPreview);
        ui::textF("Tick: {}", vm.getTimelineTick());
        ui::separator();
        timeline(vm.getTimelineWidget());
    }

    void timeline(TimelineWidgetViewModel &vm) {
        constexpr auto flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse;
        ui::canvas("Timeline Canvas", {vm.getRestAreaSize().x, 60}, flags, [&](const ui::CanvasContext &canvas) {
            canvas.drawRectangle({0, 0}, canvas.size(), vm.backgroundColorU32());
            const auto rulerColor = vm.rulerColorU32();
            // 时间轴标尺
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

            const auto keyframeColor = vm.keyframeColorU32();
            // 关键帧
            for (auto &&data : vm.getKeyframeRenderDatas()) {
                canvas.drawTriangleFilled({data.localPosX, 15}, {data.localPosX + 5, 20}, {data.localPosX - 5, 20}, keyframeColor);
                canvas.drawTriangleFilled({data.localPosX, 25}, {data.localPosX - 5, 20}, {data.localPosX + 5, 20}, keyframeColor);
            }
            // 播放头
            if (auto data = vm.getCursorRenderData(canvas.size().x)) {
                canvas.drawLine({data.localPosX, 0}, {data.localPosX, canvas.size().y}, vm.cursorColorU32(), 2.0f);
            }
        }).onInput([&](const ui::CanvasInputState &input, const ui::CanvasState &canvasState) {
            vm.onCanvasInteracted(input, canvasState);
        });
    }

} // namespace BCM_V2::ui
