#include "GuiOverlay.h"

#include "logger/LogBox.hpp"

#include "../tickrate/AudioSpeed.h"
#include "../tickrate/TickRateTest.h"
#include "../smoothpiston/SmoothPiston.h"

using namespace std::chrono_literals;

ImGuiKey ImGui_ImplWin32_KeyEventToImGuiKey(WPARAM wParam, LPARAM lParam);

void GuiOverlay::initImGui(HWND mainWindow, ID3D12Device *device, ID3D12DescriptorHeap *srvDescHeap, UINT bufferCount) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGui_ImplWin32_Init(mainWindow);
    ImGui_ImplDX12_Init(device, bufferCount,
                        DXGI_FORMAT_R8G8B8A8_UNORM, srvDescHeap,
                        srvDescHeap->GetCPUDescriptorHandleForHeapStart(),
                        srvDescHeap->GetGPUDescriptorHandleForHeapStart());

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

void GuiOverlay::drawMainOverlay() {
    static bool show_demo = true;
    if (show_demo)
        ImGui::ShowDemoWindow(&show_demo);

    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.5f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(100, 45));
    if (ImGui::Begin("TickScale Setting", nullptr,
                     ImGuiWindowFlags_NoResize
                         | ImGuiWindowFlags_NoNav
                         | ImGuiWindowFlags_NoTitleBar)) {
        auto now = std::chrono::steady_clock::now();
        // if (now - sLastShowGuiTimePoint > 2s)
        //     GuiOverlay::sShowOverlay = false;
        ImGui::Text("TickRate: %.1f Tps", sTimeScale * 20.0f);
        ImGui::Text("SmoothPiston: %s", sEnableSmoothPiston ? "ON" : "OFF");
    }
    ImGui::End();
    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void GuiOverlay::handleKeyPress() {
    if (ImGui::IsKeyChordPressed(ImGuiKey::ImGuiKey_ModAlt | ImGuiKey::ImGuiKey_KeypadAdd) && sSelectedTps < 9) {
        GuiOverlay::sShowOverlay = true;
        sLastShowGuiTimePoint = std::chrono::steady_clock::now();
        sTimeScale = TimeScaleList[++sSelectedTps];
        UpdateAudioSpeed(sTimeScale);
    }
    if (ImGui::IsKeyChordPressed(ImGuiKey::ImGuiKey_ModAlt | ImGuiKey::ImGuiKey_KeypadSubtract) && sSelectedTps > 0) {
        GuiOverlay::sShowOverlay = true;
        sLastShowGuiTimePoint = std::chrono::steady_clock::now();
        sTimeScale = TimeScaleList[--sSelectedTps];
        UpdateAudioSpeed(sTimeScale);
    }

    if (ImGui::IsKeyChordPressed(ImGuiKey::ImGuiKey_ModAlt | ImGuiKey::ImGuiKey_KeypadDecimal)) {
        GuiOverlay::sShowOverlay = true;
        sLastShowGuiTimePoint = std::chrono::steady_clock::now();
        sSelectedTps = 6;
        sTimeScale = TimeScaleList[sSelectedTps];
        UpdateAudioSpeed(sTimeScale);
    }

    if (ImGui::IsKeyChordPressed(ImGuiKey::ImGuiKey_ModAlt | ImGuiKey::ImGuiKey_Backslash)) {
        GuiOverlay::sShowOverlay = true;
        GuiOverlay::sEnableSmoothPiston = !GuiOverlay::sEnableSmoothPiston;
        setEnableSmoothPiston(GuiOverlay::sEnableSmoothPiston);
        sLastShowGuiTimePoint = std::chrono::steady_clock::now();
    }
}
