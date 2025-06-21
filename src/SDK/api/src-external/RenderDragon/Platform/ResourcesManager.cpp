#include "ResourcesManager.h"

namespace dragon::platform {

    ResourcesManager *ResourcesManager::ctor(uintptr_t a1, uintptr_t a2, uintptr_t a3, uintptr_t a4) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xC4\x49\x8B\x3E"_sig, // 1.21.50
            &ResourcesManager::ctor,
            core::deRefCall>;
        return (this->*Hook::origin)(a1, a2, a3, a4);
    }

} // namespace dragon::platform