#pragma once

namespace sapphire::ui {

    struct WindowViewModel {
    public:
        // 任意窗口处于焦点时，游戏释放鼠标，游戏不接收输入
        bool mIsOnFocus = false;
        bool mIsOpen = false;
    };

} // namespace sapphire::ui