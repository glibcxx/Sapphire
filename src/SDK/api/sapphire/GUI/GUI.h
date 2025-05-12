#pragma once

#include <chrono>
#include <functional>

#include <imgui.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include "macros/Macros.h"

#include "../input/InputManager.h"

class GuiOverlay {
public:
    struct PluginSettings {
        std::string           name;
        std::string           description;
        std::function<void()> drawSettings; // 绘制设置界面的函数
    };

    struct Hotkey {
        std::vector<ImGuiKey> keysDown;   // Keys that must be held down (can be ImGuiKey_xxx or ImGuiMod_xxx)
        ImGuiKey              triggerKey; // The single key that must be tapped (ImGuiKey_xxx)
        std::string           description;
        std::function<void()> action;
    };

    SDK_API static void registerPluginSettings(PluginSettings &&settings);

    SDK_API static void registerHotkey(Hotkey &&hotkey);

    SDK_API static void addToast(std::string message, std::chrono::steady_clock::duration duration = std::chrono::seconds(2));

private:
    friend class DX12Hook;

    inline static std::chrono::steady_clock::time_point sLastShowToastTimePoint{};
    inline static std::chrono::steady_clock::duration   sToastShowingDuration{};

    static std::unique_ptr<InputManager> sInputManager;
    static std::vector<PluginSettings>   sPluginSettings;
    static std::vector<Hotkey>           sRegisteredHotkeys;
    static std::vector<std::string>      sToastMessages;

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
