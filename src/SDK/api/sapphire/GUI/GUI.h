#pragma once

#include <chrono>
#include <functional>

#include "macros/Macros.h"

#include <imgui.h>
#include <d3d12.h>
#include <d3d11.h>
#include <dxgi1_6.h>

#include "SDK/api/sapphire/config/Config.h"

class GuiOverlay {
public:
    struct PluginSettings {
        std::string           name;
        std::string           description;
        std::function<void()> drawSettings;
    };

    struct Hotkey {
        std::vector<ImGuiKey> keysDown;   // [optinal] Keys that must be held down (can be ImGuiKey_xxx or ImGuiMod_xxx)
        ImGuiKey              triggerKey; // The single key that must be tapped (ImGuiKey_xxx)
        std::string           description;
        std::function<void()> action;
    };

    SDK_API static void registerPluginSettings(PluginSettings &&settings);

    static void registerPluginSettings(const std::string &name, const std::string &description, std::function<void()> drawSettings) {
        return registerPluginSettings({name, description, drawSettings});
    }

    SDK_API static void registerHotkey(Hotkey &&hotkey);

    SDK_API static void addToast(std::string message, std::chrono::steady_clock::duration duration = std::chrono::seconds(2));

private:
    friend class DX12Hook;

    inline static std::chrono::steady_clock::time_point sLastShowToastTimePoint{};
    inline static std::chrono::steady_clock::duration   sToastShowingDuration{};

    static std::vector<PluginSettings> sPluginSettings;
    static std::vector<Hotkey>         sRegisteredHotkeys;
    static std::vector<std::string>    sToastMessages;

    static std::shared_ptr<sapphire::config::Config> sConfig;

    inline static int sSelectedPluginIndex = -1;

    inline static int64_t sTime = 0;
    inline static int64_t sTicksPerSecond = 0;

    inline static bool sInitialized = false;

    inline static bool sShowToast = false;
    inline static bool sShowPannel = false;
    inline static bool sShowLogWindow = true;

    // Style settings for the main panel
    inline static ImVec2 sPanelPadding = ImVec2(15.0f, 15.0f);
    inline static float  sPanelRounding = 10.0f;
    inline static ImVec4 sPanelBgColor = ImVec4(0.08f, 0.08f, 0.10f, 0.92f);
    inline static float  sPanelDefaultWidthRatio = 0.7f;
    inline static float  sPanelDefaultHeightRatio = 0.8f;

    static void initImGui(
        HWND                  mainWindow,
        ID3D11Device         *device,
        ID3D11DeviceContext  *deviceContext,
        DXGI_SWAP_CHAIN_DESC &swapChainDesc
    );

    static void shutdownImGui();

    static void handleHotkey();

    static void frame();

    static void refreshCursorPos();

    static void saveConfig();
    static void loadConfig();

    static void drawToast();
    static void drawPanel();
    static void drawPluginList();
    static void drawPluginDetails();
};
