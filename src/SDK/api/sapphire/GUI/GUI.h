#pragma once

#include <chrono>
#include <functional>

#include <imgui.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include "hook/Macros.h"

#include "../input/InputManager.h"

class GuiOverlay {
public:
    struct PluginSettings {
        std::string           name;
        std::string           description;
        std::function<void()> drawSettings; // 绘制设置界面的函数
    };

    struct Hotkey {
        ImGuiKeyChord         keyChord; // ImGuiKeyChord (e.g., ImGuiMod_Alt | ImGuiKey_P)
        std::string           description;
        std::function<void()> action;
    };

    SDK_API static void registerPluginSettings(PluginSettings &&settings);

    SDK_API static void registerHotkey(Hotkey &&hotkey);

private:
    friend class DX12Hook;

    inline static std::chrono::steady_clock::time_point sLastShowToastTimePoint{};

    static std::unique_ptr<InputManager> sInputManager;
    static std::vector<PluginSettings>   sPluginSettings;
    static std::vector<Hotkey>           sRegisteredHotkeys;

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
    static void drawPanel();
    static void drawPluginList();
    static void drawPluginDetails();
};
