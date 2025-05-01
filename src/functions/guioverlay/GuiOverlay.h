#pragma once

#include <chrono>
#include <functional>

#include <imgui.h>
#include <backends/imgui_impl_dx12.h>
#include <backends/imgui_impl_win32.h>
#include <d3d12.h>
#include <dxgi1_6.h>

struct PluginSettings {
    std::string           name;         // 插件名称
    std::string           description;  // 插件描述
    std::function<void()> drawSettings; // 绘制设置界面的函数
};

class GuiOverlay {
    static constexpr float TimeScaleList[] = {0.00625f, 0.05f, 0.1f, 0.2f, 0.25f, 0.5f, 1.0f, 2.0f, 5.0f, 10.0f};

    inline static bool    LastKeyDown_P = false;
    inline static uint8_t sSelectedTps = 6;

    inline static std::chrono::steady_clock::time_point sLastShowToastTimePoint{};

public:
    inline static bool sInitialized = false;

    inline static bool sShowToast = false;
    inline static bool sShowPannel = false;
    inline static bool sShowLogWindow = true;

    inline static float sTimeScale = 1.0f;
    inline static bool  sEnableSmoothPiston = true;

    static void registerPluginSettings(const PluginSettings &settings);

    static void initImGui(HWND mainWindow, ID3D12Device *device, ID3D12DescriptorHeap *srvDescHeap, UINT bufferCount);

    static void shutdownImGui();

    static void handleKeyPress();

    static void drawGUI();

private:
    inline static std::vector<PluginSettings> sPluginSettings;
    inline static int                         sSelectedPluginIndex = -1;

    static void drawToast();

    static void drawPannel();

    static void drawPluginList();
    static void drawPluginDetails();
};
