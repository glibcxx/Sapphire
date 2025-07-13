#include "DeferredTask.h"

void lambda_144B72F30::operator()() const {
    using Hook = sapphire::ApiLoader<
        "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x56\x57\x41\x54\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x84\x24\x00\x00\x00\x00\x48\x8B\xF1\xFF\x15"_sig,
        &lambda_144B72F30::operator()>;
    return (this->*Hook::origin)();
}

namespace Core {

    template <>
    DeferredTask *DeferredTask::ctor(lambda_144B72F30 &&callback) {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xFC\x48\x8D\x47\x00\x48\x89\x83"_sig,
            &DeferredTask::ctor<lambda_144B72F30>,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(std::move(callback));
    }

    bool DeferredTask::tryExecute() {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\xEB\x00\x4D\x85\xFF\x74\x00\xF0\xFF\x47"_sig,
            &DeferredTask::tryExecute,
            sapphire::deRefCall>;
        return (this->*Hook::origin)();
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
