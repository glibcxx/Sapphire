#include "Mesh.h"

namespace mce {

    bool mce::Mesh::isValid() const {
        using Bind = sapphire::bind::Fn<
            "\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\x48\xB8"_sig,
            &Mesh::isValid>;
        return (this->*Bind::origin)();
    }

    Mesh *Mesh::ctor(mce::Mesh &&c) {
        using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xC8\xEB\x00\x33\xC9\x48\x89\x4C\x24"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x33\xF6\xEB\x00\x33\xF6\x8B\xC6\x48\x89\x44\x24"_sig,
#endif
            &Mesh::ctor>;
        return (this->*Bind::origin)(std::move(c));
    }

    void Mesh::dtor() noexcept {
        using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x83\xC3\x00\x48\x3B\xDF\x75\x00\x48\x8B\x7C\x24\x00\x48\x8B\x5C\x24\x00\xC6\x86"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            "\x48\x89\x5C\x24\x00\x48\x89\x6C\x24\x00\x48\x89\x74\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xD9\xE8\x00\x00\x00\x00\x48\x8B\x8B"_sig,
#endif
            &Mesh::dtor>;
        (this->*Bind::origin)();
    }

} // namespace mce