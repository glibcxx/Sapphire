#include "UIManager.h"

#include "../event/events/gui/GuiOverlayFrameEvent.h"
#include "SDK/api/sapphire/input/InputManager.h"
#include "SDK/api/sapphire/GUI/GUI.h"

namespace sapphire::ui {

    UIManager &UIManager::getInstance() {
        static UIManager instance{};
        return instance;
    }

    UIManager::UIManager() :
        mInputManager(sapphire::input::InputManager::getInstance()) {
        this->mOnFrame = event::EventBus::getInstance().registerAutoListener<event::GuiOverlayFrameEvent>(
            [this](event::GuiOverlayFrameEvent &e) {
                this->frame(e);
            }
        );
    }

    void UIManager::frame(event::GuiOverlayFrameEvent &e) {
        bool anyWindowOnFocus = false;
        for (auto &ui : mUIs) {
            auto viewModel = ui->mViewModel;
            if (!viewModel.mIsOpen) continue;
            anyWindowOnFocus |= viewModel.mIsOnFocus;
            ui->render(ImGuiWindowFlags_NoFocusOnAppearing);
        }
        ImGuiIO &io = ImGui::GetIO();
        anyWindowOnFocus |= e.showingPannel | io.WantCaptureMouse | io.WantTextInput;
        if (mLastAnyWindowOnFocus != anyWindowOnFocus) {
            mLastAnyWindowOnFocus = anyWindowOnFocus;
            mInputManager.blockInput(anyWindowOnFocus);
            if (anyWindowOnFocus) {
                releaseMouse();
            } else {
                grabMouse();
            }
        }
    }

} // namespace sapphire::ui