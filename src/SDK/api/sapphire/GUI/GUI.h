#pragma once

#include <chrono>
#include <functional>

#include <imgui.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include "hook/Macros.h"

#include "../input/InputManager.h"

struct PluginSettings {
    std::string           name;         // 插件名称
    std::string           description;  // 插件描述
    std::function<void()> drawSettings; // 绘制设置界面的函数
};

class GuiOverlay {
public:
    SDK_API static void registerPluginSettings(PluginSettings &&settings);

private:
    friend class DX12Hook;

    inline static std::chrono::steady_clock::time_point sLastShowToastTimePoint{};

    static std::unique_ptr<InputManager> sInputManager;
    static std::vector<PluginSettings>   sPluginSettings;

    inline static int sSelectedPluginIndex = -1;

    inline static int64_t sTime = 0;
    inline static int64_t sTicksPerSecond = 0;

    inline static bool sInitialized = false;

    inline static bool sShowToast = false;
    inline static bool sShowPannel = false;
    inline static bool sShowLogWindow = true;

    static void initImGui(
        HWND                  mainWindow,
        ID3D12Device         *device,
        ID3D12DescriptorHeap *srvDescHeap,
        DXGI_SWAP_CHAIN_DESC &swapChainDesc,
        UINT                  bufferCount
    );

    static void shutdownImGui();

    static void handleHotkey();

    static void drawGUI();

    static void refreshCursorPos();

    static void initInputManager(std::unique_ptr<InputManager> inputManager);

    static void drawToast();
    static void drawPannel();
    static void drawPluginList();
    static void drawPluginDetails();
};
