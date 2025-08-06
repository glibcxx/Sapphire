#pragma once

#include "macros/Macros.h"
#include "../event/EventManager.h"
#include "WindowViewModel.h"
#include <string>
#include <vector>
#include <memory>

namespace sapphire::event {
    class GuiOverlayFrameEvent;
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

    private:
        UIManager();

        void frame() const;

        class UIBase {
        public:
            std::string mName;

            UIBase(std::string name) :
                mName(std::move(name)) {}

            virtual ~UIBase() = default;

            virtual void render() = 0;
        };

        template <typename TView, typename TViewModel>
        class UI : public UIBase {
            TView       mViewFunction;
            TViewModel &mViewModel;

        public:
            template <typename UView, typename TViewModel>
            UI(std::string name, UView &&viewFunction, TViewModel &viewModel) :
                UIBase(std::move(name)), mViewFunction(std::forward<UView>(viewFunction)), mViewModel(viewModel) {}

            virtual void render() override {
                if (!mViewModel.mIsOpen) return;
                if (ImGui::Begin(mName.data(), &mViewModel.mIsOpen, 0))
                    mViewFunction(mViewModel);
                ImGui::End();
            }
        };

        std::vector<std::unique_ptr<UIBase>>             mUIs;
        event::AutoListener<event::GuiOverlayFrameEvent> mOnFrame;
    };

} // namespace sapphire::ui