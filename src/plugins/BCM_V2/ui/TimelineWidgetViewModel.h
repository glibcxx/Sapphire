#pragma once

#include "SDK/api/sapphire/ui/UIContext.h"
#include "SDK/api//sapphire/input/InputManager.h"
#include "SDK/api/sapphire/ui/widget/Canvas.h"
#include "SDK/api/sapphire/coroutine/Generator.h"
#include "../editor/Editor.h"

namespace BCM_V2::ui {

    namespace ui = sapphire::ui;

    class TimelineWidgetViewModel {
    public:
        TimelineWidgetViewModel(Editor &editor) :
            mEditorRef(editor), mInputManager(sapphire::input::InputManager::getInstance()) {}

        void update(float alpha) {
            float offset = mEditorRef.getTimeline().getTick() - mCanvasWidth / 2.0f / mZoom + alpha;
            mOffset = std::max(mOffset, offset);
            clampState(mCanvasWidth);
        }

        ui::Vec2 getRestAreaSize() const {
            return context.layout.availableContentRegion();
        }

        uint32_t backgroundColorU32() const {
            return context.style.colorU32(ImGuiCol_FrameBg);
        }

        uint32_t rulerColorU32() const {
            return context.style.colorU32(ImGuiCol_Text);
        }

        uint32_t keyframeColorU32() const {
            return context.style.colorU32(ImGuiCol_ButtonHovered);
        }

        uint32_t cursorColorU32() const {
            return context.style.colorU32(ImGuiCol_ButtonActive);
        }

        void onCanvasInteracted(const ui::CanvasInputState &input, const ui::CanvasState &canvas) {
            const ui::Vec2 mousePosLocal = input.mousePos - canvas.canvasPos;
            if (mInputManager.isKeyPressed(sapphire::input::KeyCode::Mod_Ctrl) && input.mouseWheel != 0.0f) {
                handleZoom(input, mousePosLocal, canvas.canvasSize.x);
            } else if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
                handlePan(input);
            } else if (input.mouseWheel != 0.0f) {
                handleScroll(input);
            } else if (mInputManager.isKeyPressed(sapphire::input::KeyCode::MouseLeft)) {
                handleScrubbing(mousePosLocal);
            }
            if (mInputManager.isLMouseDoubleClicked()) {
                handleAddKeyframe(input, mousePosLocal);
            }
            clampState(canvas.canvasSize.x);
        }

        struct RulerRenderData {
            size_t tick;
            float  localPosX;
            bool   isMajor = false;
        };

        sapphire::coro::Generator<RulerRenderData> getRulerRenderDatas(float canvasWidth) {
            mCanvasWidth = canvasWidth;

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

            mStartTick = mOffset;
            mEndTick = std::min(maxTick, static_cast<size_t>(mOffset + canvasWidth / mZoom));

            size_t tick = (mStartTick / minorTickInterval) * minorTickInterval;

            for (; tick <= mEndTick; tick += minorTickInterval) {
                const bool  isMajor = (tick % majorTickInterval == 0);
                const float localPosX = (tick - mOffset) * mZoom;

                co_yield RulerRenderData{
                    .tick = tick,
                    .localPosX = localPosX,
                    .isMajor = isMajor
                };
            }
        }

        struct KeyframeRenderData {
            size_t tick;
            float  localPosX;
        };

        sapphire::coro::Generator<KeyframeRenderData> getKeyframeRenderDatas() const {
            const auto  &path = mEditorRef.getPath();
            const size_t maxTick = path.empty() ? 2000 : path.back().tick + 400;

            auto iterKeyFrame = std::lower_bound(path.begin(), path.end(), mOffset, [](const Keyframe &kf, float val) {
                return kf.tick < static_cast<size_t>(val);
            });
            for (; iterKeyFrame != path.end() && iterKeyFrame->tick <= mEndTick; ++iterKeyFrame) {
                const float localPosX = (iterKeyFrame->tick - mOffset) * mZoom;
                if (localPosX < 0.0f) continue;
                co_yield KeyframeRenderData{iterKeyFrame->tick, localPosX};
            }
        }

        struct CursorRenderData {
            float localPosX;
            bool  render;

            operator bool() const { return render; }
        };

        CursorRenderData getCursorRenderData(float canvasWidth) {
            float x = (mEditorRef.getTimeline().getTick() - mOffset + mEditorRef.getTimeline().getAlpha()) * mZoom;
            return {x, x >= 0.0f && x <= canvasWidth};
        }

    private:
        void handleZoom(const ui::CanvasInputState &input, const ui::Vec2 &mousePosLocal, float canvasWidth) {
            const float mouseTickBeforeZoom = screenToTick(mousePosLocal.x);
            mZoom *= (input.mouseWheel > 0) ? 1.2f : 1.0f / 1.2f;
            clampState(canvasWidth);
            const float mouseTickAfterZoom = screenToTick(mousePosLocal.x);
            // 调整offset，使得鼠标下的tick位置在缩放前后保持不变，实现中心缩放
            mOffset += mouseTickBeforeZoom - mouseTickAfterZoom;
        }

        void handlePan(const ui::CanvasInputState &input) {
            if (mZoom > 0.0f) mOffset -= input.mouseDelta.x / mZoom;
        }

        void handleScroll(const ui::CanvasInputState &input) {
            const float scrollSpeedTicks = 50.0f;
            if (mZoom > 0.0f) mOffset -= input.mouseWheel * scrollSpeedTicks / mZoom;
        }

        void handleScrubbing(const ui::Vec2 &mousePosLocal) {
            if (mEditorRef.isPlaying())
                mEditorRef.togglePlaying();
            if (mZoom > 0.0f) {
                const auto       &path = mEditorRef.getPath();
                auto             &timeline = mEditorRef.getTimeline();
                const std::size_t maxTick = path.empty() ? 2000 : path.back().tick + 800;
                const size_t      newTick = std::max(0.0f, this->mOffset + mousePosLocal.x / this->mZoom);
                timeline.setTick(std::min(newTick, maxTick));
                if (newTick > mEndTick) {
                    const float scrollOffset = newTick - mEndTick;
                    mOffset += scrollOffset / mZoom * 0.5f;
                } else if (newTick < mOffset) {
                    const float scrollOffset = mOffset - newTick;
                    mOffset -= scrollOffset / mZoom * 0.5f;
                }
                auto cam = path.getCameraState(timeline.getTick(), timeline.getAlpha());
                if (cam) mEditorRef.preview(*cam);
            }
        }

        void handleAddKeyframe(const ui::CanvasInputState &input, const ui::Vec2 &mousePosLocal) {
            auto  &path = mEditorRef.getPath();
            size_t tick = mEditorRef.getTimeline().getTick();
            auto   found = std::find(path.begin(), path.end(), tick);
            if (found == path.end())
                mEditorRef.addKeyFrameAtCurrentFreeCameraPos();
            else
                path.removeKeyframe(tick);
        }

        void clampState(float canvasWidth) {
            if (canvasWidth <= 0.0f) return;
            const auto  &path = mEditorRef.getPath();
            const size_t maxTick = path.empty() ? 2000 : path.back().tick + 400;
            const float  minZoom = canvasWidth / std::max<size_t>(1, maxTick);
            mZoom = std::clamp(mZoom, minZoom, 20.0f);
            const float maxOffset = maxTick - canvasWidth / mZoom;
            mOffset = std::clamp(mOffset, 0.0f, maxOffset > 0.0f ? maxOffset : 0.0f);
        }

        float screenToTick(float screenX) const {
            if (mZoom <= 0.0f) return 0.0f;
            return mOffset + screenX / mZoom;
        }

        float mOffset = 0.0f; // ticks
        float mZoom = 5.0f;   // pixels/tick

        float mCanvasWidth = 0.0f;

        size_t mStartTick = 0;
        size_t mEndTick = 0;

        Editor &mEditorRef;

        sapphire::input::InputManager &mInputManager;

        ui::UIContext context;
    };

} // namespace BCM_V2::ui