#include "DeferredTask.h"

void lambda_144B72F30::operator()() const {
    using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xF1\x33\xDB\x0F\xB6\x69"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x54\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xF1\xFF\x15"_sig,
#endif
        &lambda_144B72F30::operator()>;
    return (this->*Bind::origin)();
}

namespace Core {

    template <>
    DeferredTask *DeferredTask::ctor(lambda_144B72F30 &&callback) {
        using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
            "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF9\x48\x89\x4C\x24\x00\x48\xC7\x44\x24"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xFC\x48\x8D\x47\x00\x48\x89\x83"_sig,
#endif
            &DeferredTask::ctor<lambda_144B72F30>>;
        return (this->*Bind::origin)(std::move(callback));
    }

    bool DeferredTask::tryExecute() {
        using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xEB\x00\x00\x85\xFF\x74\x00\xF0\xFF\x47"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xEB\x00\x4D\x85\xFF\x74\x00\xF0\xFF\x47"_sig,
#endif
            &DeferredTask::tryExecute>;
        return (this->*Bind::origin)();
        // vvv
        auto e = State::Ready;
        if (!this->mState.compare_exchange_strong(e, State::Executing))
            return false;
        {
            std::lock_guard lock{this->mExecutionMutex};
            this->mCallback();
        }
        this->mCondition.notify_all();
        this->mCallback = {};
        return true;
    }

} // namespace Core
