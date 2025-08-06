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
        if (ImGui::BeginChild(id, size, false, 0))
            content();
        ImGui::EndChild();
    }

    template <std::invocable Fn>
    void childWindow(std::string_view id, const Vec2 &size, ImGuiChildFlags childFlags, Fn &&content, const Style &style = {}) {
        if (ImGui::BeginChild(id, size, false, childFlags))
            content();
        ImGui::EndChild();
    }

    void sameLine(float offsetFromStart_x = 0.0f, float spacing = -1.0f) {
        ImGui::SameLine(offsetFromStart_x, spacing);
    }

    void separator() {
        ImGui::Separator();
    }

    template <std::invocable Fn>
    bool tabBar(std::string_view id, Fn &&tabsContent);

    template <std::invocable Fn>
    bool tabItem(std::string_view label, Fn &&content);

    /// vvv Layout & Container | Text & Display ^^^

    void text(std::string_view text, const Style &style = {}) {
        ImGui::Text(text.data());
    }

    template <typename... Args>
    void textF(const std::format_string<Args...> &fmt, Args &&...args) {
        ImGui::Text(std::format(fmt, std::forward<Args>(args)...).data());
    }

    void textDisabled(std::string_view text, const Style &style = {});

    // void image(TextureID texture, const Vec2 &size, const Style &style = {});

    void progressBar(float fraction, const Vec2 &size = {-1, 0}, const std::string &overlay = "");

    ///  vvv Controls & Inputs | Layout & Container ^^^

    template <std::invocable Fn>
    void button(std::string_view label, Fn &&onClick, const Style &style = {}) {
        if (ImGui::Button(label.data()))
            onClick();
    }

    template <std::invocable Fn>
    void invisibleButton(std::string_view label, Fn &&onClick, const Style &style = {}) {
        if (ImGui::InvisibleButton(label.data()))
            onClick();
    }

    void checkbox(std::string_view label, bool &boundValue);

    void radioButton(std::string_view label, int &boundValue, int buttonValue);

    void sliderFloat(std::string_view label, float &boundValue, float min, float max);

    void sliderInt(std::string_view label, int &boundValue, int min, int max);

    void inputText(std::string_view label, std::string &boundValue, const Style &style = {});

    void inputTextMultiline(std::string_view label, std::string &boundValue, const Vec2 &size = {0, 0});

    void comboBox(std::string_view label, int &bound_selected_index, const std::vector<std::string> &items);

    void colorEdit4(std::string_view label, float color[4], const Style &style = {});

    /// vvv Popups & Modals | Controls & Inputs ^^^

    template <std::invocable Fn>
    void modal(std::string_view name, bool &is_open, Fn &&content, const Style &style = {});

    template <std::invocable Fn>
    void tooltip(Fn &&content);

    template <std::invocable Fn>
    void contextMenu(std::string_view id, Fn &&content);

    void applyTheme(const Theme &theme);

    template <std::invocable Fn>
    void styleScope(const Style &style, Fn &&content);

} // namespace sapphire::ui