#pragma once

#include <mutex>
#include "macros/Macros.h"
#include "imgui.h"

namespace Logger {

    class GameLogger {
        friend class Loggers;

        std::mutex      logMutex;
        ImGuiTextBuffer logBuffer;
        ImVector<int>   lineOffsets; // Index to lines offset.
        bool            scrollToBottom = true;

        GameLogger() {
            lineOffsets.push_back(0);
        }

    public:
        void log(std::string_view str) {
            std::lock_guard lock(logMutex);
            logBuffer.append(str.data(), &str.back() + 1);
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

} // namespace Logger