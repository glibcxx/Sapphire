#pragma once

#include "SDK/api/sapphire/ui/UIContext.h"
#include "SDK/api/sapphire/coroutine/Generator.h"
#include "../editor/Editor.h"

namespace BCM_V2::ui {

    namespace ui = sapphire::ui;

    class TimelineWidgetViewModel {
    public:
        TimelineWidgetViewModel(Editor &editor) :
            mEditorRef(editor) {}

        ui::Vec2 getRestAreaSize() const {
            return context.layout.availableContentRegion();
        }

        uint32_t backgroundColorU32() const {
            return context.style.colorU32(ImGuiCol_FrameBg);
        }

        uint32_t rulerColorU32() const {
            return context.style.colorU32(ImGuiCol_Text);
        }

        struct RulerRenderData {
            size_t tick;
            float  localPosX;
            bool   isMajor;
        };

        sapphire::coro::Generator<RulerRenderData> getRulerRenderDatas(float canvasWidth) const {
            const auto  &path = mEditorRef.getPath();
            const size_t maxTick = path.empty() ? 2000 : path.back().tick + 400;

            const double minPixelsPerMajorTick = 70.0;
            const double minTicksPerMajorTick = minPixelsPerMajorTick / mZoom;
            const double powerOf10 = pow(10.0, floor(log10(minTicksPerMajorTick)));

            double majorTickIntervalDouble;
            if (powerOf10 * 1.0 >= minTicksPerMajorTick) {
                majorTickIntervalDouble = powerOf10 * 1.0;
            } else if (powerOf10 * 2.0 >= minTicksPerMajorTick) {
                majorTickIntervalDouble = powerOf10 * 2.0;
            } else if (powerOf10 * 5.0 >= minTicksPerMajorTick) {
                majorTickIntervalDouble = powerOf10 * 5.0;
            } else {
                majorTickIntervalDouble = powerOf10 * 10.0;
            }
            if (majorTickIntervalDouble < 5.0) { majorTickIntervalDouble = 5.0; }

            const int majorTickInterval = static_cast<int>(round(majorTickIntervalDouble));
            const int minorTickInterval = std::max(1, majorTickInterval / 5);

            const auto startTick = static_cast<size_t>(mOffset);
            const auto endTick = static_cast<size_t>(mOffset + canvasWidth / mZoom) + majorTickInterval;

            for (size_t tick = (startTick / minorTickInterval) * minorTickInterval; tick <= endTick && tick <= maxTick; tick += minorTickInterval) {
                const bool  isMajor = (tick % majorTickInterval == 0);
                const float localPosX = (static_cast<float>(tick) - mOffset) * mZoom;

                co_yield RulerRenderData{
                    .tick = tick,
                    .localPosX = localPosX,
                    .isMajor = isMajor
                };
            }
        }

    private:
        float   mOffset = 0.0f;
        float   mZoom = 5.0f;
        Editor &mEditorRef;

        ui::UIContext context;
    };

} // namespace BCM_V2::ui