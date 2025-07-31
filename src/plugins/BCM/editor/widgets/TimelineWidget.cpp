#include "TimelineWidget.h"
#include "../BCMEditor.h"
#include "../CameraPath.h"
#include "../CamTimeline.h"
#include "SDK/api/sapphire/GUI/GUI.h"

TimelineWidget::TimelineWidget(BCMEditor &editor) :
    mEditor(editor) {}

void TimelineWidget::render() {
    ImGui::Separator();

    const auto  &path = mEditor.getPath();
    const size_t max_tick = path.empty() ? 2000 : path.back().tick + 400;
    {
        // The timeline drawing area
        const float timeline_height = 60.0f;
        ImGui::BeginChild("TimelineView", ImVec2(0, timeline_height), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        const ImVec2 p = ImGui::GetCursorScreenPos();
        const ImVec2 avail = ImGui::GetContentRegionAvail();
        ImDrawList  *draw_list = ImGui::GetWindowDrawList();

        // Background
        draw_list->AddRectFilled(p, ImVec2(p.x + avail.x, p.y + avail.y), ImGui::GetColorU32(ImGuiCol_FrameBg));

        // Interaction: Zoom and Pan
        ImGui::SetCursorScreenPos(p);
        ImGui::InvisibleButton("timeline_interaction_area", avail);
        if (ImGui::IsItemHovered()) {
            if (ImGui::GetIO().MouseWheel != 0) {
                if (ImGui::GetIO().KeyCtrl) {
                    // Zoom with Ctrl + mouse wheel
                    const float mouse_x_in_window = ImGui::GetIO().MousePos.x - p.x;
                    const float mouse_tick_before_zoom = this->mOffset + mouse_x_in_window / this->mZoom;

                    this->mZoom *= (ImGui::GetIO().MouseWheel > 0) ? 1.2f : 1.0f / 1.2f;

                    // Clamp zoom level
                    if (avail.x > 0) {
                        const float min_zoom = avail.x / static_cast<float>(std::max<size_t>(1, max_tick));
                        this->mZoom = glm::clamp(this->mZoom, min_zoom, 20.0f);
                    } else {
                        this->mZoom = glm::clamp(this->mZoom, 0.05f, 20.0f);
                    }
                    const float mouse_tick_after_zoom = this->mOffset + mouse_x_in_window / this->mZoom;
                    this->mOffset += mouse_tick_before_zoom - mouse_tick_after_zoom;
                } else {
                    // Scroll with mouse wheel
                    const float scroll_speed_pixels = 50.0f;
                    this->mOffset -= ImGui::GetIO().MouseWheel * scroll_speed_pixels / this->mZoom;
                }
            }
            // Pan with middle mouse button
            if (ImGui::IsMouseDragging(ImGuiMouseButton_Middle)) {
                this->mOffset -= ImGui::GetIO().MouseDelta.x / this->mZoom;
            }
        }

        // Clamp offset
        float max_offset = static_cast<float>(max_tick) - avail.x / this->mZoom;
        this->mOffset = glm::clamp(this->mOffset, 0.0f, max_offset > 0.0f ? max_offset : 0.0f);

        // Auto-scroll during playback
        if (mEditor.isPlaying()) {
            float target_offset = static_cast<float>(mEditor.getTimeline().getTick()) - (avail.x / 2.0f / this->mZoom);
            this->mOffset = glm::clamp(target_offset, 0.0f, max_offset > 0.0f ? max_offset : 0.0f);
        }

        // Draw ruler and ticks
        const ImU32 col_ruler_maj = ImGui::GetColorU32(ImGuiCol_Text);
        const ImU32 col_ruler_min = ImGui::GetColorU32(ImGuiCol_TextDisabled);

        const double min_pixels_per_major_tick = 70.0;
        double       min_ticks_per_major_tick = min_pixels_per_major_tick / this->mZoom;
        double       power_of_10 = pow(10.0, floor(log10(min_ticks_per_major_tick)));
        double       major_tick_interval_d;
        if (power_of_10 * 1.0 >= min_ticks_per_major_tick) {
            major_tick_interval_d = power_of_10 * 1.0;
        } else if (power_of_10 * 2.0 >= min_ticks_per_major_tick) {
            major_tick_interval_d = power_of_10 * 2.0;
        } else if (power_of_10 * 5.0 >= min_ticks_per_major_tick) {
            major_tick_interval_d = power_of_10 * 5.0;
        } else {
            major_tick_interval_d = power_of_10 * 10.0;
        }
        if (major_tick_interval_d < 5.0) { major_tick_interval_d = 5.0; }
        const int major_tick_interval = static_cast<int>(round(major_tick_interval_d));
        const int minor_tick_interval = std::max(1, major_tick_interval / 5);

        const auto start_tick = static_cast<size_t>(this->mOffset);
        const auto end_tick = static_cast<size_t>(this->mOffset + avail.x / this->mZoom) + major_tick_interval;

        for (size_t t = (start_tick / minor_tick_interval) * minor_tick_interval; t <= end_tick; t += minor_tick_interval) {
            const float x = p.x + (static_cast<float>(t) - this->mOffset) * this->mZoom;

            if (t % major_tick_interval == 0) {
                draw_list->AddLine(ImVec2(x, p.y), ImVec2(x, p.y + avail.y), col_ruler_min);
                draw_list->AddText(ImVec2(x + 2.f, p.y), col_ruler_maj, std::to_string(t).c_str());
            } else {
                draw_list->AddLine(ImVec2(x, p.y + avail.y * 0.6f), ImVec2(x, p.y + avail.y), col_ruler_min);
            }
        }

        // Draw keyframes
        const ImU32 col_keyframe = ImGui::GetColorU32(ImGuiCol_ButtonHovered);
        for (const auto &keyframe : path) {
            const float x = p.x + (static_cast<float>(keyframe.tick) - this->mOffset) * this->mZoom;
            if (x >= p.x && x <= p.x + avail.x) {
                draw_list->AddTriangleFilled(ImVec2(x, p.y + 20), ImVec2(x - 4, p.y + 25), ImVec2(x + 4, p.y + 25), col_keyframe);
                draw_list->AddTriangleFilled(ImVec2(x, p.y + 30), ImVec2(x - 4, p.y + 25), ImVec2(x + 4, p.y + 25), col_keyframe);
            }
        }

        // Draw cursor
        const float cursor_x = p.x + (static_cast<float>(mEditor.getTimeline().getTick()) - this->mOffset) * this->mZoom;
        if (cursor_x >= p.x && cursor_x <= p.x + avail.x) {
            draw_list->AddLine(ImVec2(cursor_x, p.y), ImVec2(cursor_x, p.y + avail.y), ImGui::GetColorU32(ImGuiCol_ButtonActive), 2.0f);
        }

        // Scrubbing logic
        if (ImGui::IsItemActive() && ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            mEditor.setScrubbing(true);
            mEditor.setPlaying(false);
            const float mouse_x_on_widget = ImGui::GetMousePos().x - p.x;
            const auto  new_tick = std::min(max_tick, static_cast<size_t>(std::max(0.0f, this->mOffset + mouse_x_on_widget / this->mZoom)));
            mEditor.getTimeline().setTick(new_tick);
        } else if (mEditor.isScrubbing() && !ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
            mEditor.setScrubbing(false);
        }

        ImGui::EndChild();
    }
}