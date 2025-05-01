#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <imgui.h>

class GameLogger {
public:
    std::mutex      logMutex;
    ImGuiTextBuffer logBuffer;
    bool            scrollToBottom = true;

    enum class Type {
        Debug,
        Info,
        Warn,
        Error
    };

    static GameLogger &getInstance() {
        static GameLogger gameLogger;
        return gameLogger;
    }

    void addLog(Type logType, const std::string &str) {
        std::lock_guard lock(logMutex);
        switch (logType) {
        case Type::Debug:
            logBuffer.appendf("[DEBUG] %s\n", str.c_str());
            break;
        case Type::Info:
            logBuffer.appendf("[INFO] %s\n", str.c_str());
            break;
        case Type::Warn:
            logBuffer.appendf("[WARN] %s\n", str.c_str());
            break;
        case Type::Error:
            logBuffer.appendf("[ERROR] %s\n", str.c_str());
        }
        scrollToBottom = true;
    }

    void draw(const char *title, bool *p_open = nullptr) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, 400));
        if (ImGui::Begin(title, p_open, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoNav)) {
            if (ImGui::Button("Clear")) {
                std::lock_guard lock(logMutex);
                logBuffer.clear();
            }
            ImGui::Separator();
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            {
                std::lock_guard lock(logMutex);
                ImGui::TextUnformatted(logBuffer.begin(), logBuffer.end());
            }
            if (scrollToBottom) {
                ImGui::SetScrollHereY(1.0f);
                scrollToBottom = false;
            }
            ImGui::EndChild();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
};
