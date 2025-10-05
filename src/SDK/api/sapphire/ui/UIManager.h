#pragma once

#include <string>
#include <vector>
#include <memory>
#include "macros/Macros.h"
#include <imgui.h>
#include "../event/EventManager.h"
#include "WindowViewModel.h"
#include "SDK/api/src-client/common/client/game/ClientInstance.h"

namespace sapphire::event {
    class GuiOverlayFrameEvent;
}
namespace sapphire::input {
    class InputManager;
}

namespace sapphire::ui {

    class UIManager {
    public:
        SDK_API static UIManager &getInstance();

        template <typename TView, typename TViewModel>
            requires std::invocable<TView, TViewModel &> && std::derived_from<TViewModel, WindowViewModel>
        void registerView(const std::string &name, TView &&view, TViewModel &viewModel) {
            this->mUIs.emplace_back(std::make_unique<UI<TView, TViewModel>>(name, view, viewModel));
        }

        void releaseMouse() const {
            if (ClientInstance::primaryClientInstance)
                ClientInstance::primaryClientInstance->releaseMouse();
        }

        void grabMouse() const {
            if (!ClientInstance::primaryClientInstance)
                return;
            if (!ClientInstance::primaryClientInstance->isShowingMenu())
                ClientInstance::primaryClientInstance->grabMouse();
            ImGui::SetWindowFocus(NULL);
        }

        bool isMouseGrabbed() {
            return !ClientInstance::primaryClientInstance || ClientInstance::primaryClientInstance->getMouseGrabbed();
        }

    private:
        UIManager();

        void frame(event::GuiOverlayFrameEvent &e);

        class UIBase {
        public:
            std::string      mName;
            WindowViewModel &mViewModel;

            UIBase(std::string name, WindowViewModel &viewModel) :
                mName(std::move(name)), mViewModel(viewModel) {}

            virtual ~UIBase() = default;

            virtual void render(ImGuiWindowFlags windowFlags) = 0;
        };

        template <typename TView, typename TViewModel>
        class UI : public UIBase {
            TView mViewFunction;

        public:
            template <typename UView>
            UI(std::string name, UView &&viewFunction, TViewModel &viewModel) :
                UIBase(std::move(name), viewModel), mViewFunction(std::forward<UView>(viewFunction)) {}

            virtual void render(ImGuiWindowFlags windowFlags) override {
                if (ImGui::Begin(mName.data(), &mViewModel.mIsOpen, windowFlags))
                    mViewFunction(static_cast<TViewModel &>(mViewModel));
                ImGui::End();
            }
        };

        sapphire::input::InputManager                   &mInputManager;
        std::vector<std::unique_ptr<UIBase>>             mUIs;
        event::AutoListener<event::GuiOverlayFrameEvent> mOnFrame;

        bool mLastAnyWindowOnFocus = false;
    };

} // namespace sapphire::ui