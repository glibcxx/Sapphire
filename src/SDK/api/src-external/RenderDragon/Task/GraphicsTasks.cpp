#include "GraphicsTasks.h"

namespace dragon::task {

    GraphicsTasks *GraphicsTasks::ctor(
        WorkerPool &rendererPool, gsl::span<std::reference_wrapper<WorkerPool>> helperPools, Scheduler &clientScheduler
    ) {
        using Hook = sapphire::ApiLoader<
            "\x48\x8B\xC4\x48\x89\x58\x00\x48\x89\x70\x00\x48\x89\x78\x00\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x68\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x45\x00\x4D\x8B\xE1"_sig, // 1.21.50
            &GraphicsTasks::ctor>;
        return (this->*Hook::origin)(rendererPool, helperPools, clientScheduler);
    }

    GraphicsTasks::InitializationState GraphicsTasks::continueInit(InitializationState initState) {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\x90\x80\x7D\x00\x00\x75\x00\x0F\xB6\x45"_sig, // 1.21.50
            &GraphicsTasks::continueInit,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(std::move(initState));
    }

    bool GraphicsTasks::_queueMainFrameRenderTask(const std::function<void(void)> &task) {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\x84\xC0\x75\x00\xB3\x00\x4C\x8B\x7D"_sig, // 1.21.50
            &GraphicsTasks::_queueMainFrameRenderTask,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(task);
    }

    void GraphicsTasks::waitForPreviousFrame() {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\x80\x7E\x00\x00\x0F\x84\x00\x00\x00\x00\xC6\x46"_sig, // 1.21.50
            &GraphicsTasks::waitForPreviousFrame,
            sapphire::deRefCall>;
        return (this->*Hook::origin)();
    }

    void GraphicsTasks::frame(
        const std::function<void(void)>                                                               &frameContentCallback,
        const std::function<std::chrono::steady_clock::duration(std::chrono::steady_clock::duration)> &a2
    ) {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\x90\x48\x8B\x8D\x00\x00\x00\x00\x48\x85\xC9\x74\x00\x48\x8B\x01\x48\x8D\x95\x00\x00\x00\x00\x48\x3B\xCA\x0F\x95\xC2\x48\x8B\x40\x00\xFF\x15\x00\x00\x00\x00\x90\xBF"_sig, // 1.21.50
            &GraphicsTasks::frame,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(frameContentCallback, a2);
    }

    std::variant<
        std::monostate,
        GraphicsTasks::InitBegin,
        GraphicsTasks::InitPending,
        GraphicsTasks::InitFinalize,
        GraphicsTasks::InitEnd>
    GraphicsTasks::_lambda_144B71107_at_continueInit::operator()(const InitBegin &begin) const {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\xEB\x00\x4C\x8B\xC3\x48\x8D\x54\x24\x00\x48\x8D\x8C\x24"_sig, // 1.21.50
            &GraphicsTasks::_lambda_144B71107_at_continueInit::operator(),
            sapphire::deRefCall>;
        return (this->*Hook::origin)(begin);
    }

    GraphicsTasks::InitializationState *GraphicsTasks::InitializationState::ctor(const InitializationState &other) {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\xC6\x45\x00\x00\xEB\x00\x49\x83\xC0"_sig, // 1.21.50
            &GraphicsTasks::InitializationState::ctor,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(other);
    }

} // namespace dragon::task