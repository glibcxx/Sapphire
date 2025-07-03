#include "DeferredTask.h"

namespace Core {

    bool DeferredTask::tryExecute() {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\xEB\x00\x4D\x85\xFF\x74\x00\xF0\xFF\x47"_sig,
            &DeferredTask::tryExecute,
            sapphire::deRefCall>;
        return (this->*Hook::origin)();
    }

} // namespace Core
