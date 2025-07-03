#include "GraphicsTasks.h"

namespace dragon::task {

    GraphicsTasks::InitializationState GraphicsTasks::continueInit(InitializationState initState) {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\x90\x80\x7D\x00\x00\x75\x00\x0F\xB6\x45"_sig, //  1.21.50
            &GraphicsTasks::continueInit,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(std::move(initState));
    }

    bool GraphicsTasks::_queueMainFrameRenderTask(const std::function<void(void)> &task) {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\x84\xC0\x75\x00\xB3\x00\x4C\x8B\x7D"_sig, //  1.21.50
            &GraphicsTasks::_queueMainFrameRenderTask,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(task);
    }

    std::variant<
        std::monostate,
        GraphicsTasks::InitBegin,
        GraphicsTasks::InitPending,
        GraphicsTasks::InitFinalize,
        GraphicsTasks::InitEnd>
    GraphicsTasks::_lambda_144B71107_at_continueInit::operator()(const InitBegin &begin) const {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\xEB\x00\x4C\x8B\xC3\x48\x8D\x54\x24\x00\x48\x8D\x8C\x24"_sig, //  1.21.50
            &GraphicsTasks::_lambda_144B71107_at_continueInit::operator(),
            sapphire::deRefCall>;
        return (this->*Hook::origin)(begin);
    }

    GraphicsTasks::InitializationState *GraphicsTasks::InitializationState::ctor(const InitializationState &other) {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\xC6\x45\x00\x00\xEB\x00\x49\x83\xC0"_sig, //  1.21.50
            &GraphicsTasks::InitializationState::ctor,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(other);
    }

} // namespace dragon::task