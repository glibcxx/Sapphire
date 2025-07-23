#include "GUI.h"

#include "SDK/core/Core.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

using namespace std::chrono_literals;

std::unique_ptr<InputManager>             GuiOverlay::sInputManager = nullptr;
std::vector<GuiOverlay::PluginSettings>   GuiOverlay::sPluginSettings{};
std::vector<GuiOverlay::Hotkey>           GuiOverlay::sRegisteredHotkeys{};
std::vector<std::string>                  GuiOverlay::sToastMessages{};
std::shared_ptr<sapphire::config::Config> GuiOverlay::sConfig{};

void GuiOverlay::registerPluginSettings(PluginSettings &&settings) {
    sPluginSettings.push_back(std::move(settings));
}

void GuiOverlay::registerHotkey(Hotkey &&hotkey) {
    sRegisteredHotkeys.push_back(std::move(hotkey));
}

void GuiOverlay::addToast(std::string message, std::chrono::steady_clock::duration duration) {
    GuiOverlay::sShowToast = true;
    GuiOverlay::sLastShowToastTimePoint = std::chrono::steady_clock::now();
    if (sToastShowingDuration < duration) sToastShowingDuration = duration;
    sToastMessages.emplace_back(std::move(message));
}

InputManager &GuiOverlay::getInputManager() {
    return *sInputManager;
}

void GuiOverlay::initImGui(
    HWND                  mainWindow,
    ID3D11Device         *device,
    ID3D11DeviceContext  *deviceContext,
    DXGI_SWAP_CHAIN_DESC &swapChainDesc
) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGui_ImplWin32_Init(mainWindow);
    ImGui_ImplDX11_Init(device, deviceContext);

    ImGuiIO &io = ImGui::GetIO();
    io.IniFilename = nullptr;
    sConfig = sapphire::ConfigManager::getInstance()["imgui.json"].shared_from_this();
    GuiOverlay::loadConfig();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    [[maybe_unused]] ImFont *font = io.Fonts->AddFontFromFileTTF("C:/Windows/Fonts/msyh.ttc", 27.0f, nullptr, io.Fonts->GetGlyphRangesChineseFull());

    io.DisplaySize = ImVec2((float)swapChainDesc.BufferDesc.Width, (float)swapChainDesc.BufferDesc.Height);
    if (!QueryPerformanceFrequency((LARGE_INTEGER *)&GuiOverlay::sTicksPerSecond))
        GuiOverlay::sTicksPerSecond = 1000;
    if (!QueryPerformanceCounter((LARGE_INTEGER *)&GuiOverlay::sTime))
        GuiOverlay::sTime = GetTickCount64();

    GuiOverlay::registerHotkey({
        {ImGuiMod_Alt},
        ImGuiKey_P,
        "Toggle Main Panel",
        []() {
            GuiOverlay::sShowPannel = !GuiOverlay::sShowPannel;
            if (ClientInstance::primaryClientInstance) {
                if (GuiOverlay::sShowPannel) {
                    ClientInstance::primaryClientInstance->releaseMouse();
                } else if (!ClientInstance::primaryClientInstance->isShowingMenu()) {
                    ClientInstance::primaryClientInstance->grabMouse();
                    ImGui::SetWindowFocus(NULL);
                }
            } else {
                Logger::Warn("ClientInstance not found!");
            } },
    });

    GuiOverlay::registerHotkey({
        {},
        ImGuiKey_Escape,
        "Close Main Panel",
        []() {
            GuiOverlay::sShowPannel = false;
            if (!ClientInstance::primaryClientInstance->isShowingMenu())
                ClientInstance::primaryClientInstance->grabMouse();
            ImGui::SetWindowFocus(NULL);
        },
    });

    GuiOverlay::registerPluginSettings(
        {"Appearance",
         "Customize the style of the GUI.",
         []() {
             ImGui::TextUnformatted("Panel Style");
             ImGui::SliderFloat2("Window Padding", reinterpret_cast<float *>(&GuiOverlay::sPanelPadding), 5.0f, 30.0f, "%.0f px");
             ImGui::SliderFloat("Window Rounding", &GuiOverlay::sPanelRounding, 0.0f, 20.0f, "%.0f px");
             ImGui::ColorEdit4("Background Color", reinterpret_cast<float *>(&GuiOverlay::sPanelBgColor), ImGuiColorEditFlags_AlphaBar);

             ImGui::Separator();
             ImGui::TextUnformatted("Panel Size (relative to main window)");
             ImGui::SliderFloat("Window Width", &GuiOverlay::sPanelDefaultWidthRatio, 0.3f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);
             ImGui::SliderFloat("Window Height", &GuiOverlay::sPanelDefaultHeightRatio, 0.3f, 1.0f, "%.2f", ImGuiSliderFlags_AlwaysClamp);

             ImGui::Separator();
             ImGui::TextUnformatted("Note: Panel size and position are remembered after the first adjustment.");
         }}
    );
}

void GuiOverlay::shutdownImGui() {
    GuiOverlay::sInitialized = false;
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void GuiOverlay::handleHotkey() {
    ImGuiIO &io = ImGui::GetIO();
    for (const auto &hotkey : sRegisteredHotkeys) {
        // Check if the trigger key was just pressed (not held)
        if (ImGui::IsKeyPressed(hotkey.triggerKey, false)) {
            bool allRequiredDown = true;
            // Check if all other required keys are down
            ImGuiKeyChord mods = ImGuiMod_None;
            for (ImGuiKey key : hotkey.keysDown) {
                if (key & ImGuiMod_Mask_) { // It's a modifier flag (ImGuiMod_xxx)
                    mods |= (key & ImGuiMod_Mask_);
                } else if (!ImGui::IsKeyDown(key)) { // It's a key code (ImGuiKey_xxx), Check if the normal key is held down
                    allRequiredDown = false;
                    break;
                }
            }

            if (mods == io.KeyMods && allRequiredDown) {
                hotkey.action();
            }
        }
    }
}

void GuiOverlay::drawGUI() {
#ifndef NDEBUG
    static bool show_demo = true;
    if (show_demo)
        ImGui::ShowDemoWindow(&show_demo);
#endif

    if (GuiOverlay::sShowLogWindow)
        Logger::Loggers::getInstance().getGameLogger().draw("Log Window", &GuiOverlay::sShowLogWindow);

    if (GuiOverlay::sShowToast)
        GuiOverlay::drawToast();
    if (GuiOverlay::sShowPannel)
        GuiOverlay::drawPanel();
}

void GuiOverlay::refreshCursorPos() {
    ImGuiIO &io = ImGui::GetIO();
    POINT    mousePos;
    if (GetCursorPos(&mousePos)) {
        ScreenToClient(moduleInfo::gMainWindow, &mousePos);
        io.AddMousePosEvent((float)mousePos.x, (float)mousePos.y);
    }
    INT64 currentTime;
    if (!QueryPerformanceCounter((LARGE_INTEGER *)&currentTime))
        currentTime = GetTickCount64();
    io.DeltaTime = (float)(currentTime - GuiOverlay::sTime) / GuiOverlay::sTicksPerSecond;
    GuiOverlay::sTime = currentTime;
    if (io.DeltaTime <= 0.0f)
        io.DeltaTime = 0.00001f;
}

void GuiOverlay::saveConfig() {
    if (ImGui::GetIO().WantSaveIniSettings) {
        if (ImGui::GetCurrentContext() == nullptr) {
            Logger::Warn("[GuiOverlay] ImGui context not available for saveConfig.");
            return;
        }

        size_t      imgui_ini_size = 0;
        const char *imgui_ini_data = ImGui::SaveIniSettingsToMemory(&imgui_ini_size);
        if (imgui_ini_data && imgui_ini_size > 0) {
            try {
                sConfig->set("imgui", std::string{imgui_ini_data, imgui_ini_size});
                sConfig->save();
                Logger::Info("[GuiOverlay] ImGui settings Saved.");
            } catch (const nlohmann::json::exception &e) {
                Logger::Error("[GuiOverlay] Error saving ImGui settings: {}", e.what());
            }
        } else {
            Logger::Info("[GuiOverlay] No ImGui settings to save.");
        }
        ImGui::GetIO().WantSaveIniSettings = false;
    }
}

void GuiOverlay::loadConfig() {
    if (ImGui::GetCurrentContext() == nullptr) {
        Logger::Warn("[GuiOverlay] ImGui context not available for LoadImGuiSettings.");
        return;
    }
    try {
        std::string imguiIni = sConfig->get<std::string>("imgui");
        if (!imguiIni.empty()) {
            ImGui::LoadIniSettingsFromMemory(imguiIni.c_str(), imguiIni.length());
            Logger::Info("[GuiOverlay] Loaded ImGui settings from config file.");
        } else {
            Logger::Info("[GuiOverlay] No ImGui settings in config file.");
        }
    } catch (const nlohmann::json::exception &e) {
        Logger::Error("[GuiOverlay] Error loading ImGui settings: {}", e.what());
    }
}

void GuiOverlay::initInputManager(std::unique_ptr<InputManager> inputManager) {
    GuiOverlay::sInputManager = std::move(inputManager);
}

void GuiOverlay::drawToast() {
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.45f));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, ImVec2(100, 45));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 6);
    if (ImGui::Begin(
            "Toasts",
            nullptr,
            ImGuiWindowFlags_NoResize
                | ImGuiWindowFlags_NoDocking
                | ImGuiWindowFlags_NoNav
                | ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_AlwaysAutoResize
        )) {
        for (auto &&i : sToastMessages) {
            ImGui::Text(i.c_str());
        }
        auto now = std::chrono::steady_clock::now();
        if (now - sLastShowToastTimePoint >= sToastShowingDuration) {
            GuiOverlay::sShowToast = false;
            sToastMessages.clear();
            sToastShowingDuration = 0s;
        }
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

void GuiOverlay::drawPanel() {
    ImGuiViewport *viewport = ImGui::GetMainViewport();

    ImVec2 defaultPanelSize = ImVec2(viewport->WorkSize.x * sPanelDefaultWidthRatio, viewport->WorkSize.y * sPanelDefaultHeightRatio);
    ImVec2 defaultPanelPos = ImVec2(
        viewport->WorkPos.x + (viewport->WorkSize.x - defaultPanelSize.x) * 0.5f,
        viewport->WorkPos.y + (viewport->WorkSize.y - defaultPanelSize.y) * 0.5f
    );

    ImGui::SetNextWindowPos(defaultPanelPos);
    ImGui::SetNextWindowSize(defaultPanelSize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, sPanelPadding);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, sPanelRounding);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, sPanelBgColor);

    const ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse
                                        | ImGuiWindowFlags_NoMove
                                        | ImGuiWindowFlags_NoResize
                                        | ImGuiWindowFlags_NoTitleBar
                                        | ImGuiWindowFlags_NoDocking;

    if (ImGui::Begin("Main Panel", &sShowPannel, window_flags)) {
        if (ImGui::BeginTable("PluginLayout", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_BordersInnerV)) {
            ImGui::TableSetupColumn("PluginList", ImGuiTableColumnFlags_WidthStretch, 0.25f);
            ImGui::TableSetupColumn("PluginDetails", ImGuiTableColumnFlags_WidthStretch, 0.75f);

            ImGui::TableNextColumn();
            drawPluginList();

            ImGui::TableNextColumn();
            drawPluginDetails();

            ImGui::EndTable();
        }
    }
    ImGui::End();

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
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
