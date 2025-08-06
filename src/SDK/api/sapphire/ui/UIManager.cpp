#include "UIManager.h"

#include "../event/events/gui/GuiOverlayFrameEvent.h"

namespace sapphire::ui {

    UIManager &UIManager::getInstance() {
        static UIManager instance{};
        return instance;
    }

    UIManager::UIManager() {
        this->mOnFrame = event::EventManager::getInstance().registerAutoListener<event::GuiOverlayFrameEvent>(
            [this](event::GuiOverlayFrameEvent &e) {
                this->frame();
            }
        );
    }

    void UIManager::frame() const {
        for (auto &ui : mUIs) {
            ui->render();
        }
    }

} // namespace sapphire::ui