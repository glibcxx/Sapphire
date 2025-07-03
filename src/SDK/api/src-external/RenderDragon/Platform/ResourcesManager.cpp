#include "ResourcesManager.h"

namespace dragon::platform {

    ResourcesManager *ResourcesManager::ctor(
        dragon::task::GraphicsTasks                                    &graphicsTasks,
        const ResourcesManagerConfiguration                            &configuration,
        const dragon::materials::CompiledMaterialManager::Unk144B99FE0 &a3,
        const std::function<uintptr_t(uintptr_t)>                      &a4
    ) {
        using Hook = sapphire::ApiLoader<
            "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xC4\x49\x8B\x3E"_sig, // 1.21.50
            &ResourcesManager::ctor,
            sapphire::deRefCall>;
        return (this->*Hook::origin)(graphicsTasks, configuration, a3, a4);
    }

} // namespace dragon::platform