#include "MeshData.h"

namespace mce {

#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    MeshData MeshData::clone() const {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\x48\x89\x5C\x24\x00\x48\x89\x54\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xFA\x48\x8B\xD9\x0F\xB6\x01"_sig,
            &MeshData::clone>;
        return (this->*Hook::origin)();
    }
#endif

    void MeshData::dtor() {
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x41\xBD\x00\x00\x00\x00\x48\x8B\x8D"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x4C\x8B\x8E\x00\x00\x00\x00\x45\x89"_sig,
#endif
            &MeshData::dtor>;
        (this->*Hook::origin)();
    }

    void MeshData::clear() {
        using Hook = sapphire::ApiLoader<
            "\x48\x83\xEC\x00\xC6\x01\x00\x48\x8B\x41"_sig,
            &MeshData::clear>;
        (this->*Hook::origin)();
    }

} // namespace mce