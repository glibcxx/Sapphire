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

        // 坐标转换
        ui::Vec2 toScreen(const ui::Vec2 &localPos) const { return localPos - mCanvasTopLeftPos; }
        ui::Vec2 toLocal(const ui::Vec2 &screenPos) const { return mCanvasTopLeftPos + screenPos; }

        // 画布尺寸
        ui::Vec2 size() const { return mCanvasSize; }

    private:
        template <std::invocable<const CanvasContext &> Fn>
        friend void canvas(std::string_view id, const Vec2 &size, ImGuiChildFlags childFlags, Fn &&painter);
        template <std::invocable<const CanvasContext &> Fn>
        friend void canvas(
            std::string_view id, const Vec2 &size, const Vec2 &pos, ImGuiChildFlags childFlags, Fn &&painter
        );

        CanvasContext(ImDrawList *drawList, const Vec2 &pos, const Vec2 &size) :
            mDrawList(drawList), mCanvasTopLeftPos(pos), mCanvasSize(size) {}

        ImDrawList *mDrawList;
        ui::Vec2    mCanvasTopLeftPos;
        ui::Vec2    mCanvasSize;
    };

    template <std::invocable<const CanvasContext &> Fn>
    void canvas(std::string_view id, const Vec2 &size, ImGuiChildFlags childFlags, Fn &&painter) {
        if (ImGui::BeginChild(id.data(), size, false, childFlags))
            painter(CanvasContext{ImGui::GetWindowDrawList(), (ui::Vec2)ImGui::GetCursorScreenPos(), size});
        ImGui::EndChild();
    }

    template <std::invocable<const CanvasContext &> Fn>
    void canvas(std::string_view id, const Vec2 &size, const Vec2 &pos, ImGuiChildFlags childFlags, Fn &&painter) {
        if (ImGui::BeginChild(id.data(), size, false, childFlags))
            painter(CanvasContext{ImGui::GetWindowDrawList(), pos, size});
        ImGui::EndChild();
    }

} // namespace sapphire::ui