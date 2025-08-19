#pragma once

#include "../UIData.h"

namespace sapphire::ui {

    class CanvasContext {
    public:
        // 局部坐标
        const CanvasContext &drawLine(
            const ui::Vec2 &from, const ui::Vec2 &to, uint32_t color, float thickness = 1.0f
        ) const {
            mDrawList->AddLine(mCanvasTopLeftPos + from, mCanvasTopLeftPos + to, color, thickness);
            ImGui::SetCursorScreenPos(mCanvasTopLeftPos);
            return *this;
        }

        // 局部坐标
        const CanvasContext &drawRectangle(
            const ui::Vec2 &from, const ui::Vec2 &to, uint32_t color, float rounding = 0.0f
        ) const {
            mDrawList->AddRectFilled(mCanvasTopLeftPos + from, mCanvasTopLeftPos + to, color, rounding, 0);
            return *this;
        }

        // 局部坐标
        const CanvasContext &drawRectangle(const ui::Rect &box, uint32_t color, float rounding = 0.0f) const {
            mDrawList->AddRectFilled(mCanvasTopLeftPos + box.min, mCanvasTopLeftPos + box.max, color, rounding, 0);
            return *this;
        }

        // 局部坐标
        const CanvasContext &drawText(const ui::Vec2 &pos, uint32_t color, std::string_view text) const {
            mDrawList->AddText(mCanvasTopLeftPos + pos, color, text.data());
            return *this;
        }

        // 局部坐标
        const CanvasContext &drawTriangleFilled(
            const ui::Vec2 &p1, const ui::Vec2 &p2, const ui::Vec2 &p3, uint32_t color
        ) const {
            mDrawList->AddTriangleFilled(mCanvasTopLeftPos + p1, mCanvasTopLeftPos + p2, mCanvasTopLeftPos + p3, color);
            return *this;
        }

        void drawPath(const std::vector<ImVec2> &localPoints, uint32_t color, bool filled, float thickness) {
            if (localPoints.empty()) return;

            std::vector<ImVec2> screenPoints;
            screenPoints.reserve(localPoints.size());
            for (const auto &p : localPoints) {
                screenPoints.push_back({mCanvasTopLeftPos.x + p.x, mCanvasTopLeftPos.y + p.y});
            }
            if (filled)
                mDrawList->AddConvexPolyFilled(screenPoints.data(), screenPoints.size(), color);
            else
                mDrawList->AddPolyline(screenPoints.data(), screenPoints.size(), color, ImDrawFlags_Closed, thickness);
        }

        // 坐标转换
        ui::Vec2 toScreen(const ui::Vec2 &localPos) const { return localPos - mCanvasTopLeftPos; }
        ui::Vec2 toLocal(const ui::Vec2 &screenPos) const { return mCanvasTopLeftPos + screenPos; }

        // 画布尺寸
        ui::Vec2 size() const { return mCanvasSize; }

        CanvasContext(ImDrawList *drawList, const Vec2 &pos, const Vec2 &size) :
            mDrawList(drawList), mCanvasTopLeftPos(pos), mCanvasSize(size) {}

    private:
        ImDrawList *mDrawList;
        ui::Vec2    mCanvasTopLeftPos;
        ui::Vec2    mCanvasSize;
    };

    struct CanvasInputState {
        Vec2  mousePos;
        Vec2  mouseDelta;
        float mouseWheel;
        bool  isHovered;
        bool  isActive;
    };

    struct CanvasState {
        Vec2 canvasPos;
        Vec2 canvasSize;
    };

    template <typename... Fns>
    class CanvasBuilder {
        static_assert(false, "CanvasBuilder template type Requirements: \n"
                             "void Painter(const CanvasContext &);\n"
                             "void OnInputCallback(const CanvasInputState &, const CanvasState &);");
    };

    template <
        std::invocable<const CanvasContext &>                         Painter,
        std::invocable<const CanvasInputState &, const CanvasState &> OnInputCallback>
    class CanvasBuilder<Painter, OnInputCallback> {
        Painter         mPainter;
        OnInputCallback mOnInput;

        std::string_view mId;
        Vec2             mCanvasSize;
        Vec2             mCanvasPos;
        ImGuiChildFlags  mChildFlags;

    public:
        constexpr CanvasBuilder(
            Painter         &&painter,
            OnInputCallback &&onInput,
            std::string_view  id,
            const Vec2       &size,
            const Vec2       &pos,
            ImGuiChildFlags   childFlags
        ) :
            mPainter(std::move(painter)),
            mOnInput(std::move(onInput)),
            mId(id),
            mCanvasSize(size),
            mCanvasPos(pos),
            mChildFlags(childFlags) {}

        ~CanvasBuilder() {
            childWindow(mId, {mCanvasSize.x, mCanvasSize.y}, mChildFlags, [this]() {
                constexpr auto enabledButton = ImGuiButtonFlags_MouseButtonLeft
                                             | ImGuiButtonFlags_MouseButtonRight
                                             | ImGuiButtonFlags_MouseButtonMiddle;
                ImGui::InvisibleButton(mId.data(), {mCanvasSize.x, mCanvasSize.y}, enabledButton);

                const bool isHovered = ImGui::IsItemHovered();
                const bool isActive = ImGui::IsItemActive();
                if (isHovered || isActive) {
                    auto            &io = ImGui::GetIO();
                    CanvasInputState state;
                    state.mousePos = io.MousePos;
                    state.mouseDelta = io.MouseDelta;
                    state.mouseWheel = io.MouseWheel;
                    state.isHovered = isHovered;
                    state.isActive = isActive;
                    mOnInput(state, {mCanvasPos, mCanvasSize});
                }
                mPainter(CanvasContext{ImGui::GetWindowDrawList(), mCanvasPos, mCanvasSize});
            });
        }

        CanvasBuilder(const CanvasBuilder &) = delete;
        CanvasBuilder(CanvasBuilder &&) = delete;
    };

    template <typename Fn1, typename Fn2>
    CanvasBuilder(Fn1, Fn2, std::string_view, const Vec2 &, const Vec2 &, ImGuiChildFlags) -> CanvasBuilder<Fn1, Fn2>;

    template <std::invocable<const CanvasContext &> Painter>
    class CanvasBuilder<Painter> {
        Painter mPainter;

        std::string_view mId;
        Vec2             mCanvasSize;
        Vec2             mCanvasPos;
        ImGuiChildFlags  mChildFlags;

        bool mValid = true;

    public:
        constexpr CanvasBuilder(
            Painter &&painter, std::string_view id, const Vec2 &size, const Vec2 &pos, ImGuiChildFlags childFlags
        ) :
            mPainter(std::move(painter)), mId(id), mCanvasSize(size), mCanvasPos(pos), mChildFlags(childFlags) {}

        ~CanvasBuilder() {
            if (mValid) {
                childWindow(mId, {mCanvasSize.x, mCanvasSize.y}, mChildFlags, [this]() {
                    mPainter(CanvasContext{ImGui::GetWindowDrawList(), mCanvasPos, mCanvasSize});
                });
            }
        }

        CanvasBuilder(const CanvasBuilder &) = delete;
        CanvasBuilder(CanvasBuilder &&) = delete;

        template <std::invocable<const CanvasInputState &, const CanvasState &> OnInputCallback>
        constexpr CanvasBuilder<Painter, OnInputCallback> onInput(OnInputCallback &&onInput) {
            mValid = false;
            return {std::move(mPainter), std::forward<OnInputCallback>(onInput), mId, mCanvasSize, mCanvasPos, mChildFlags};
        }
    };

    template <typename Fn1>
    CanvasBuilder(Fn1, std::string_view, const Vec2 &, const Vec2 &, ImGuiChildFlags) -> CanvasBuilder<Fn1>;

    template <std::invocable<const CanvasContext &> Fn>
    auto canvas(std::string_view id, const Vec2 &size, ImGuiChildFlags childFlags, Fn &&painter) {
        return CanvasBuilder{std::move(painter), id, size, ImGui::GetCursorScreenPos(), childFlags};
    }

    template <std::invocable<const CanvasContext &> Fn>
    auto canvas(std::string_view id, const Vec2 &size, const Vec2 &pos, ImGuiChildFlags childFlags, Fn &&painter) {
        return CanvasBuilder{std::move(painter), id, size, pos, childFlags};
    }

} // namespace sapphire::ui