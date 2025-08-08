#include "GraphicsTasks.h"

namespace dragon::task {

    GraphicsTasks *GraphicsTasks::ctor(
        WorkerPool &rendererPool, gsl::span<std::reference_wrapper<WorkerPool>> helperPools, Scheduler &clientScheduler
    ) {
        using Hook = sapphire::ApiLoader<
            [](uintptr_t addr) { return memory::deRef(addr + 15, memory::AsmOperation::CALL); }
                | "\x4C\x8D\x44\x24\x00\x00\x8B\x00\x00\x8D\x00\x08\x01\x00\x00\xE8\x00\x00\x00\x00\x90\x00\x89"_sig,
            &GraphicsTasks::ctor>;
        return (this->*Hook::origin)(rendererPool, std::move(helperPools), clientScheduler);
    }

    GraphicsTasks::InitializationState GraphicsTasks::continueInit(InitializationState initState) {
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x0F\xBE\x4D\x00\x48\x83\xF9\x00\x75"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x80\x7D\x00\x00\x75\x00\x0F\xB6\x45"_sig,
#endif
            &GraphicsTasks::continueInit>;
        return (this->*Hook::origin)(std::move(initState));
    }

    bool GraphicsTasks::_queueMainFrameRenderTask(const std::function<void(void)> &task) {
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x84\xC0\x41\x0F\x94\xC7\x48\x8B\x4C\x24"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x84\xC0\x75\x00\xB3\x01\x00\x8B"_sig,
#endif
            &GraphicsTasks::_queueMainFrameRenderTask>;
        return (this->*Hook::origin)(task);
    }

    void GraphicsTasks::waitForPreviousFrame() {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x96\x00\x00\x00\x00\x48\x83\xEA\x00\x4C"_sig,
            &GraphicsTasks::waitForPreviousFrame>;
        return (this->*Hook::origin)();
    }

    void GraphicsTasks::frame(
        const std::function<void(void)>                                                               &frameContentCallback,
        const std::function<std::chrono::steady_clock::duration(std::chrono::steady_clock::duration)> &a2
    ) {
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x8B\x8D\x00\x00\x00\x00\x48\x85\xC9\x74\x00\x48\x8B\x01\x48\x8D\x95\x00\x00\x00\x00\x48\x3B\xCA\x0F\x95\xC2\x48\x8B\x40\x00\xFF\x15\x00\x00\x00\x00\x90\x80\xBD"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x48\x8B\x8D\x00\x00\x00\x00\x48\x85\xC9\x74\x00\x48\x8B\x01\x48\x8D\x95\x00\x00\x00\x00\x48\x3B\xCA\x0F\x95\xC2\x48\x8B\x40\x00\xFF\x15\x00\x00\x00\x00\x90\xBF"_sig,
#endif
            &GraphicsTasks::frame>;
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
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xEB\x00\x4C\x8B\xC7\x48\x8D\x55"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xEB\x00\x4C\x8B\xC3\x48\x8D\x54\x24\x00\x48\x8D\x8C\x24"_sig,
#endif
            &GraphicsTasks::_lambda_144B71107_at_continueInit::operator()>;
        return (this->*Hook::origin)(begin);
    }

    GraphicsTasks::InitializationState *GraphicsTasks::InitializationState::ctor(const InitializationState &other) {
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xC6\x45\x00\x00\xEB\x00\x49\x8B\xD0"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xC6\x45\x00\x00\xEB\x00\x49\x83\xC0"_sig,
#endif
            &GraphicsTasks::InitializationState::ctor>;
        return (this->*Hook::origin)(other);
    }

} // namespace dragon::task