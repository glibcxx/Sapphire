#pragma once

#include <string>
#include <format>
#include <vector>
#include "style/Style.h"
#include "style/Theme.h"
#include "widget/Canvas.h"

namespace sapphire::ui {

    /// vvv Layout & Container vvv

    template <std::invocable Fn>
    void group(Fn &&content, const Style &style = {});

    template <std::invocable Fn>
    void childWindow(std::string_view id, const Vec2 &size, Fn &&content, const Style &style = {}) {
        if (ImGui::BeginChild(id.data(), size, false, 0))
            content();
        ImGui::EndChild();
    }

    template <std::invocable Fn>
    void childWindow(std::string_view id, const Vec2 &size, ImGuiChildFlags childFlags, Fn &&content, const Style &style = {}) {
        if (ImGui::BeginChild(id.data(), size, false, childFlags))
            content();
        ImGui::EndChild();
    }

    inline void sameLine(float offsetFromStart_x = 0.0f, float spacing = -1.0f) {
        ImGui::SameLine(offsetFromStart_x, spacing);
    }

    inline void separator() {
        ImGui::Separator();
    }

    template <std::invocable Fn>
    bool tabBar(std::string_view id, Fn &&tabsContent) {
        if (ImGui::BeginTabBar(id.data())) {
            tabsContent();
            ImGui::EndTabBar();
            return true;
        }
        return false;
    }

    template <std::invocable Fn>
    bool tabItem(std::string_view label, Fn &&content) {
        if (ImGui::BeginTabItem(label.data())) {
            content();
            ImGui::EndTabItem();
            return true;
        }
        return false;
    }

    /// vvv Layout & Container | Text & Display ^^^

    inline void text(std::string_view text, const Style &style = {}) {
        ImGui::Text("%s", text.data());
    }

    template <typename... Args>
    void textF(const std::format_string<Args...> &fmt, Args &&...args) {
        ImGui::Text("%s", std::format(fmt, std::forward<Args>(args)...).data());
    }

    inline void textDisabled(std::string_view text, const Style &style = {}) {
        ImGui::TextDisabled("%s", text.data());
    }

    template <typename... Args>
    void textDisabledF(const std::format_string<Args...> &fmt, Args &&...args) {
        ImGui::TextDisabled("%s", std::format(fmt, std::forward<Args>(args)...).data());
    }

    // void image(TextureID texture, const Vec2 &size, const Style &style = {});

    inline void progressBar(float fraction, const Vec2 &size = {-std::numeric_limits<float>::min(), 0}, std::string_view overlay = {}) {
        ImGui::ProgressBar(fraction, size, overlay.data());
    }

    ///  vvv Controls & Inputs | Layout & Container ^^^

    template <std::invocable Fn>
    void button(std::string_view label, Fn &&onClick, const Style &style = {}) {
        if (ImGui::Button(label.data()))
            onClick();
    }

    template <std::invocable Fn>
    void invisibleButton(std::string_view label, const Vec2 &size, Fn &&onClick, const Style &style = {}) {
        if (ImGui::InvisibleButton(label.data(), size))
            onClick();
    }

    inline void checkbox(std::string_view label, bool &boundValue, const Style &style = {}) {
        ImGui::Checkbox(label.data(), &boundValue);
    }

    template <std::invocable Fn>
    inline void checkbox(std::string_view label, bool &boundValue, Fn &&onChanged, const Style &style = {}) {
        if (ImGui::Checkbox(label.data(), &boundValue))
            onChanged();
    }

    inline void radioButton(std::string_view label, int &boundValue, int buttonValue) {
        ImGui::RadioButton(label.data(), &boundValue, buttonValue);
    }

    inline void sliderFloat(std::string_view label, float &boundValue, float min, float max) {
        ImGui::SliderFloat(label.data(), &boundValue, min, max);
    }

    inline void sliderInt(std::string_view label, int &boundValue, int min, int max) {
        ImGui::SliderInt(label.data(), &boundValue, min, max);
    }

    template <std::size_t N>
    inline void inputText(std::string_view label, char (&textBuffer)[N], const Style &style = {}) {
        ImGui::InputText(label.data(), &textBuffer, N);
    }

    template <std::size_t N>
    inline void inputTextMultiline(std::string_view label, char (&textBuffer)[N], const Vec2 &size = {0, 0}) {
        ImGui::InputTextMultiline(label.data(), &textBuffer, N, size);
    }

    inline void inputFloat(
        std::string_view label, float &v, float step = 0.0f, float stepFast = 0.0f, const Style &style = {}
    ) {
        ImGui::InputFloat(label.data(), &v, step, stepFast);
    }

    template <std::size_t N>
    inline void inputFloat(
        std::string_view label, float (&v)[N], float step = 0.0f, float stepFast = 0.0f, const Style &style = {}
    ) {
        if constexpr (N == 1)
            ImGui::InputFloat(label.data(), &v, step, stepFast);
        else if constexpr (N == 2)
            ImGui::InputFloat2(label.data(), &v, step, stepFast);
        else if constexpr (N == 3)
            ImGui::InputFloat3(label.data(), &v, step, stepFast);
        else if constexpr (N == 4)
            ImGui::InputFloat4(label.data(), &v, step, stepFast);
        else
            static_assert(false, "Invalid range");
    }

    inline void inputInt(std::string_view label, int &v, int step = 0, int stepFast = 0, const Style &style = {}) {
        ImGui::InputInt(label.data(), &v, step, stepFast);
    }

    template <std::size_t N>
    inline void inputInt(std::string_view label, int (&v)[N], int step = 0, int stepFast = 0, const Style &style = {}) {
        if constexpr (N == 1)
            ImGui::InputInt(label.data(), &v, step, stepFast);
        else if constexpr (N == 2)
            ImGui::InputInt2(label.data(), &v, step, stepFast);
        else if constexpr (N == 3)
            ImGui::InputInt3(label.data(), &v, step, stepFast);
        else if constexpr (N == 4)
            ImGui::InputInt4(label.data(), &v, step, stepFast);
        else
            static_assert(false, "Invalid range");
    }

    inline void comboBox(std::string_view label, int &bound_selected_index, std::span<std::string> &items) {
        ImGui::Combo(
            label.data(),
            &bound_selected_index,
            reinterpret_cast<const char *const *>(items.data()),
            static_cast<int>(items.size())
        );
    }

    inline void colorEdit4(std::string_view label, Color color, const Style &style = {}) {
        ImGui::ColorEdit4(label.data(), color);
    }

    /// vvv Popups & Modals | Controls & Inputs ^^^

    template <std::invocable Fn>
    void modal(std::string_view name, bool &is_open, Fn &&content, const Style &style = {}) {
        if (ImGui::Begin(name.data(), &is_open, 0))
            content();
        ImGui::End();
    }

    template <std::invocable Fn>
    void tooltip(Fn &&content) {
        if (ImGui::BeginTooltip()) {
            content();
            ImGui::EndTooltip();
        }
    }

    template <std::invocable Fn>
    void contextMenu(std::string_view id, Fn &&content) {
        if (ImGui::BeginPopup(id.data())) {
            content();
            ImGui::EndPopup();
        }
    }

    inline void applyTheme(const Theme &theme) {
    }

    template <std::invocable Fn>
    void styleScope(const Style &style, Fn &&content) {
        content();
    }

} // namespace sapphire::ui