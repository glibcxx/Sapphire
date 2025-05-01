#include "GuiOverlay.h"

#include "logger/Logger.hpp"

#include "../tickrate/AudioSpeed.h"
#include "../tickrate/TickRateTest.h"
#include "../smoothpiston/SmoothPiston.h"

#include "SDK/api/src-client/common/client/game/ClientInstance.h"

using namespace std::chrono_literals;

void GuiOverlay::initImGui(HWND mainWindow, ID3D12Device *device, ID3D12DescriptorHeap *srvDescHeap, UINT bufferCount) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGui_ImplWin32_Init(mainWindow);
    ImGui_ImplDX12_Init(
        device,
        bufferCount,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        srvDescHeap,
        srvDescHeap->GetCPUDescriptorHandleForHeapStart(),
        srvDescHeap->GetGPUDescriptorHandleForHeapStart()
    );

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    [[maybe_unused]] ImFont *font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 27.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());
}

void GuiOverlay::shutdownImGui() {
    GuiOverlay::sInitialized = false;
    ImGui_ImplDX12_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void GuiOverlay::handleKeyPress() {
    if (ImGui::IsKeyChordPressed(ImGuiKey::ImGuiMod_Alt | ImGuiKey::ImGuiKey_P)) {
        GuiOverlay::sShowPannel = !GuiOverlay::sShowPannel;
        if (ClientInstance::primaryClientInstance) {
            if (GuiOverlay::sShowPannel)
                ClientInstance::primaryClientInstance->releaseMouse();
            else if (!ClientInstance::primaryClientInstance->isShowingMenu())
                ClientInstance::primaryClientInstance->grabMouse();
        }
    }

    if (ImGui::IsKeyChordPressed(ImGuiKey::ImGuiMod_Alt | ImGuiKey::ImGuiKey_KeypadAdd) && sSelectedTps < 9) {
        GuiOverlay::sShowToast = true;
        sLastShowToastTimePoint = std::chrono::steady_clock::now();
        sTimeScale = TimeScaleList[++sSelectedTps];
        UpdateAudioSpeed(sTimeScale);
    }

    if (ImGui::IsKeyChordPressed(ImGuiKey::ImGuiMod_Alt | ImGuiKey::ImGuiKey_KeypadSubtract) && sSelectedTps > 0) {
        GuiOverlay::sShowToast = true;
        sLastShowToastTimePoint = std::chrono::steady_clock::now();
        sTimeScale = TimeScaleList[--sSelectedTps];
        UpdateAudioSpeed(sTimeScale);
    }

    if (ImGui::IsKeyChordPressed(ImGuiKey::ImGuiMod_Alt | ImGuiKey::ImGuiKey_KeypadDecimal)) {
        GuiOverlay::sShowToast = true;
        sLastShowToastTimePoint = std::chrono::steady_clock::now();
        sSelectedTps = 6;
        sTimeScale = TimeScaleList[sSelectedTps];
        UpdateAudioSpeed(sTimeScale);
    }

    if (ImGui::IsKeyChordPressed(ImGuiKey::ImGuiMod_Alt | ImGuiKey::ImGuiKey_Backslash)) {
        GuiOverlay::sShowToast = true;
        GuiOverlay::sEnableSmoothPiston = !GuiOverlay::sEnableSmoothPiston;
        setEnableSmoothPiston(GuiOverlay::sEnableSmoothPiston);
        sLastShowToastTimePoint = std::chrono::steady_clock::now();
    }
}

void GuiOverlay::drawGUI() {
    static bool show_demo = true;
    if (show_demo)
        ImGui::ShowDemoWindow(&show_demo);

    if (GuiOverlay::sShowLogWindow)
        GameLogger::getInstance().draw("Log Window", &GuiOverlay::sShowLogWindow);

    if (GuiOverlay::sShowToast)
        GuiOverlay::drawToast();
    if (GuiOverlay::sShowPannel)
        GuiOverlay::drawPannel();
}

void GuiOverlay::drawToast() {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(100, 45));
    if (ImGui::Begin(
            "TickScale Setting",
            nullptr,
            ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoDocking
                | ImGuiWindowFlags_NoNav
                | ImGuiWindowFlags_NoTitleBar
        )) {
        ImGui::Text("TickRate: %.1f Tps", sTimeScale * 20.0f);
        ImGui::Text("SmoothPiston: %s", sEnableSmoothPiston ? "ON" : "OFF");

        auto now = std::chrono::steady_clock::now();
        if (now - sLastShowToastTimePoint > 2s)
            GuiOverlay::sShowToast = false;
    }
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void GuiOverlay::drawPannel() {
    ImGuiViewport *viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->Pos);
    ImGui::SetNextWindowSize(viewport->Size);
    if (ImGui::Begin(
            "Main Panel",
            &GuiOverlay::sShowPannel,
            ImGuiWindowFlags_NoDecoration
                | ImGuiWindowFlags_NoMove
                | ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoSavedSettings
                | ImGuiWindowFlags_NoBringToFrontOnFocus
        )) {
        ImGui::Columns(2, "PluginLayout", true);
        drawPluginList();

        ImGui::NextColumn();
        drawPluginDetails();

        ImGui::Columns(1);
    }
    ImGui::End();
}

void GuiOverlay::drawPluginList() {
    ImGui::BeginChild("Plugin List", ImVec2(0, 0), true);

    if (sPluginSettings.empty()) {
        ImGui::Text("No plugins registered.");
    } else {
        for (size_t i = 0; i < sPluginSettings.size(); ++i) {
            const auto &plugin = sPluginSettings[i];
            bool        isSelected = (sSelectedPluginIndex == i);
            if (ImGui::Selectable(plugin.name.c_str(), isSelected)) {
                sSelectedPluginIndex = (int)i;
            }
        }
    }

    ImGui::EndChild();
}

void GuiOverlay::drawPluginDetails() {
    ImGui::BeginChild("Plugin Details", ImVec2(0, 0), true);

    if (sSelectedPluginIndex >= 0 && sSelectedPluginIndex < sPluginSettings.size()) {
        const auto &plugin = sPluginSettings[sSelectedPluginIndex];

        ImGui::Text("Name: %s", plugin.name.c_str());
        ImGui::TextWrapped("Description: %s", plugin.description.c_str());
        ImGui::Separator();

        if (plugin.drawSettings) {
            plugin.drawSettings();
        } else {
            ImGui::Text("No settings available for this plugin.");
        }
    } else {
        ImGui::Text("Select a plugin from the list.");
    }

    ImGui::EndChild();
}

void GuiOverlay::registerPluginSettings(const PluginSettings &settings) {
    sPluginSettings.push_back(settings);
}