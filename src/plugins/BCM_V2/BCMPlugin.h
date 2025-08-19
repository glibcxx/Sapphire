#pragma once

#include <memory>
#include "SDK/api/sapphire/event/Event.h"

class FreeCameraPlugin;
class Minecraft;

namespace BCM_V2 {

    class Editor;

    namespace ui {
        class MainUIWindowViewModel;
    }

    class BCMPlugin {
    public:
        static BCMPlugin &getInstance();

    private:
        BCMPlugin();
        ~BCMPlugin();

        void setupEventListeners();
        void setupHotkeys();
        void setupSettingsMenu();

        Minecraft *getMinecraft();

        std::unique_ptr<Editor>                    mEditor;
        std::unique_ptr<ui::MainUIWindowViewModel> mViewModel;
        FreeCameraPlugin                          &mFreeCamPlugin;
        Minecraft                                 *mClientMinecraft = nullptr;
    };
} // namespace BCM_V2