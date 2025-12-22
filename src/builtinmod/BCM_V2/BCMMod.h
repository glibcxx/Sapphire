#pragma once

#include <memory>
#include "SDK/api/sapphire/event/Event.h"

class FreeCameraMod;
class Minecraft;

namespace BCM_V2 {

    class Editor;

    namespace ui {
        class MainUIWindowViewModel;
    }

    class BCMMod {
    public:
        static BCMMod &getInstance();

    private:
        BCMMod();
        ~BCMMod();

        void setupEventListeners();
        void setupHotkeys();
        void setupSettingsMenu();

        Minecraft *getMinecraft();

        std::unique_ptr<Editor>                    mEditor;
        std::unique_ptr<ui::MainUIWindowViewModel> mViewModel;
        FreeCameraMod                          &mFreeCamMod;
        Minecraft                                 *mClientMinecraft = nullptr;
    };
} // namespace BCM_V2