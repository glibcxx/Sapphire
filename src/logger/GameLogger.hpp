#pragma once

#include <vector>
#include <string>
#include <mutex>
#include <imgui.h>

#include "LogBox.hpp"

class GameLogger {
    std::mutex      logMutex;
    ImGuiTextBuffer logBuffer;
    ImVector<int>   lineOffsets; // Index to lines offset.
    bool            scrollToBottom = true;

public:
    enum class Type {
        Debug,
        Info,
        Warn,
        Error
    };

    GameLogger() {
        lineOffsets.push_back(0);
    }

    SDK_API static GameLogger &getInstance() {
        static GameLogger gameLogger;
        return gameLogger;
    }

    void addLog(Type logType, const std::string &str) {
        std::lock_guard lock(logMutex);
        switch (logType) {
        default:
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
            break;
        }
        updateLineOffsets();
        scrollToBottom = true;
    }

    void clearLogs() {
        std::lock_guard lock(logMutex);
        logBuffer.clear();
        lineOffsets.clear();
        lineOffsets.push_back(0);
    }

    void draw(const char *title, bool *p_open = nullptr) {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(500, 400));
        if (ImGui::Begin(title, p_open, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoNav)) {
            if (ImGui::Button("Clear")) {
                clearLogs();
            }
            ImGui::Separator();
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
            {
                std::lock_guard  lock(logMutex);
                const char      *buf = logBuffer.begin();
                const char      *buf_end = logBuffer.end();
                ImGuiListClipper clipper;
                clipper.Begin(lineOffsets.Size - 1); // -1 because LineOffsets has an extra trailing item for the end of buffer
                while (clipper.Step()) {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++) {
                        const char *line_start = buf + lineOffsets[line_no];
                        const char *line_end = (line_no + 1 < lineOffsets.Size) ? (buf + lineOffsets[line_no + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
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

private:
    void updateLineOffsets() {
        int         old_size = lineOffsets.empty() ? 0 : lineOffsets.back();
        const char *buf_begin = logBuffer.begin();
        for (int new_size = logBuffer.size(); old_size < new_size; old_size++)
            if (buf_begin[old_size] == '\n')
                lineOffsets.push_back(old_size + 1);
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
