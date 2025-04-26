#pragma once

#include <chrono>

#include <imgui.h>
#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>
#include <d3d12.h>
#include <dxgi1_6.h>

class GuiOverlay {
    static constexpr float TimeScaleList[] = {0.00625f, 0.05f, 0.1f, 0.2f, 0.25f, 0.5f, 1.0f, 2.0f, 5.0f, 10.0f};

    inline static bool    LastKeyDown_P = false;
    inline static uint8_t sSelectedTps = 6;

    inline static std::chrono::steady_clock::time_point sLastShowGuiTimePoint{};

public:
    inline static bool sInitialized = false;
    inline static bool sShowOverlay = false;

    inline static float sTimeScale = 1.0f;
    inline static bool  sEnableSmoothPiston = true;

    static void initImGui(HWND mainWindow, ID3D12Device *device, ID3D12DescriptorHeap *srvDescHeap, UINT bufferCount);

    static void shutdownImGui();

    static void drawMainOverlay();

    static void handleKeyPress();
};
