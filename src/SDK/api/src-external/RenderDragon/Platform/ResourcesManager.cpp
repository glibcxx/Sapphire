#include "ResourcesManager.h"

namespace dragon::platform {

    ResourcesManager *ResourcesManager::ctor(
        dragon::task::GraphicsTasks                                    &graphicsTasks,
        const ResourcesManagerConfiguration                            &configuration,
        const dragon::materials::CompiledMaterialManager::Unk144B99FE0 &a3,
        const std::function<uintptr_t(uintptr_t)>                      &a4
    ) {
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xD8\xEB\x00\x49\x8B\xDC\x48\x8B\x36"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\xC4\x49\x8B\x3E"_sig,
#endif
            &ResourcesManager::ctor>;
        return (this->*Hook::origin)(graphicsTasks, configuration, a3, a4);
    }

} // namespace dragon::platform