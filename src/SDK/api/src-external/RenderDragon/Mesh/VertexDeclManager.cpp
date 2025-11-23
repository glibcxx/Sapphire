#include "VertexDeclManager.h"

namespace dragon::mesh {

    VertexDeclManager::Impl *VertexDeclManager::Impl::ctor() {
        using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
            "\x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF9\xE8\x00\x00\x00\x00\x33\xC0"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\xEB\x00\x49\x8B\x00\x48\x89\x03\x48\x89\x9F"_sig,
#endif
            &Impl::ctor>;
        return (this->*Bind::origin)();
    }

} // namespace dragon::mesh