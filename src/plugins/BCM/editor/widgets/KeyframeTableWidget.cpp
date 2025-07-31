#include "KeyframeTableWidget.h"
#include "../BCMEditor.h"
#include "../../BCM.h"

KeyframeTableWidget::KeyframeTableWidget(BCMEditor &editor) :
    mEditor(editor) {}

void KeyframeTableWidget::render() {
    auto &path = mEditor.getPath();
    ImGui::Text("Keyframes: %zu", path.size());

    if (ImGui::BeginTable("keyframes_table", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableSetupColumn("Tick");
        ImGui::TableSetupColumn("Position");
        ImGui::TableSetupColumn("Orientation");
        ImGui::TableSetupColumn("Interpolation");
        ImGui::TableSetupColumn("Actions");
        ImGui::TableHeadersRow();

        std::optional<size_t> tick_to_delete;
        for (auto &keyframe : path) {
            ImGui::PushID(static_cast<int>(keyframe.tick));
            ImGui::TableNextRow();

            ImGui::TableSetColumnIndex(0);
            ImGui::Text("%zu", keyframe.tick);

            ImGui::TableSetColumnIndex(1);
            ImGui::Text("%.2f, %.2f, %.2f", keyframe.position.x, keyframe.position.y, keyframe.position.z);

            ImGui::TableSetColumnIndex(2);
            glm::vec3 euler = glm::degrees(glm::eulerAngles(keyframe.orientation));
            ImGui::Text("%.2f, %.2f, %.2f", euler.y, euler.x, euler.z);

            ImGui::TableSetColumnIndex(3);
            const char *items[] = {"Linear", "Catmull-Rom", "Bezier"};
            int         current_item = static_cast<int>(keyframe.interpToNext);
            ImGui::SetNextItemWidth(-1.0f);
            if (ImGui::Combo("##interp", &current_item, items, IM_ARRAYSIZE(items))) {
                keyframe.interpToNext = static_cast<InterpolationType>(current_item);
            }

            ImGui::TableSetColumnIndex(4);
            if (ImGui::Button("Go To")) {
                mEditor.getPlugin().mFreeCam.mFreeCamPos = keyframe.position;
                mEditor.getPlugin().mFreeCam.setFreeCamOrientation(keyframe.orientation);
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                tick_to_delete = keyframe.tick;
            }
            ImGui::PopID();
        }
        ImGui::EndTable();

        if (tick_to_delete) {
            path.removeKeyframe(*tick_to_delete);
        }
    }
}