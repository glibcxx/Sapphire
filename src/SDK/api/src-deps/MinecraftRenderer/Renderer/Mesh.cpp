#include "Mesh.h"

namespace mce {

    bool mce::Mesh::isValid() const {
        using Hook = sapphire::ApiLoader<
            "\x40\x53\x48\x83\xEC\x00\x48\x8B\xD9\x48\xB8"_sig,
            &Mesh::isValid>;
        return (this->*Hook::origin)();
    }

    Mesh *Mesh::ctor(mce::Mesh &&c) {
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xC8\xEB\x00\x33\xC9\x48\x89\x4C\x24"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x33\xF6\xEB\x00\x33\xF6\x8B\xC6\x48\x89\x44\x24"_sig,
#endif
            &Mesh::ctor>;
        return (this->*Hook::origin)(std::move(c));
    }

} // namespace mce