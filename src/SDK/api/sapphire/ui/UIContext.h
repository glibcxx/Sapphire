#pragma once

#include "UIData.h"

namespace sapphire::ui {

    struct InteractionContext {
        bool isItemHovered() const { return ImGui::IsItemHovered(); }
        bool isItemActive() const { return ImGui::IsItemActive(); }
        Vec2 getMouseDelta() const { return ImGui::GetIO().MouseDelta; }
        Vec2 getMouseDragDelta() const { return ImGui::GetMouseDragDelta(); }
        // ...
    };

    struct LayoutContext {
        // cursor 指当前渲染到哪了
        Vec2 cursorPos() const { return ImGui::GetCursorScreenPos(); }
        Vec2 cursorPosLocal() const { return ImGui::GetCursorPos(); }
        void setCursorScreenPos(const Vec2 &pos) const { ImGui::SetCursorScreenPos(pos); }
        Vec2 availableContentRegion() const { return ImGui::GetContentRegionAvail(); }
        // ...
    };

    struct StyleContext {
        // ImFont *font() const;
        // float   fontSize() const;
        uint32_t colorU32(ImGuiCol idx, float alpha_mul = 1.0f) const {
            return ImGui::GetColorU32(idx, alpha_mul);
        }
        // ...
    };

    struct UIContext {
        [[msvc::no_unique_address]] InteractionContext interaction;
        [[msvc::no_unique_address]] LayoutContext      layout;
        [[msvc::no_unique_address]] StyleContext       style;
    };

    constexpr UIContext getUIContext() { return {}; }

} // namespace sapphire::ui