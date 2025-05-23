#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <imgui.h>

#include "LogBox.hpp"

class GameLogger {
    std::mutex      logMutex;
    ImGuiTextBuffer logBuffer;
    bool            scrollToBottom = true;

public:
    enum class Type {
        Debug,
        Info,
        Warn,
        Error
    };

    SDK_API static GameLogger &getInstance() {
        static GameLogger gameLogger;
        return gameLogger;
    }

    void addLog(Type logType, const std::string &str) {
        std::lock_guard lock(logMutex);
        switch (logType) {
        case Type::Debug:
#ifndef NDEBUG
            logBuffer.appendf("[DEBUG] %s\n", str.c_str());
#endif
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

namespace Logger {
    template <typename... Args>
    inline void Debug(std::format_string<Args...> fmt, Args &&...args) {
#ifndef NDEBUG
        GameLogger::getInstance().addLog(GameLogger::Type::Debug, std::format(fmt, std::forward<Args>(args)...));
#endif
    }

    template <typename... Args>
    inline void Info(std::format_string<Args...> fmt, Args &&...args) {
        GameLogger::getInstance().addLog(GameLogger::Type::Info, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void Error(std::format_string<Args...> fmt, Args &&...args) {
        GameLogger::getInstance().addLog(GameLogger::Type::Error, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args>
    inline void Warn(std::format_string<Args...> fmt, Args &&...args) {
        GameLogger::getInstance().addLog(GameLogger::Type::Warn, std::format(fmt, std::forward<Args>(args)...));
    }
} // namespace Logger
