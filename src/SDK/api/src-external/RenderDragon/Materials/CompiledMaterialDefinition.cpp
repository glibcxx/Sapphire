#include "CompiledMaterialDefinition.h"

namespace dragon::materials {

    CompiledMaterialDefinition *CompiledMaterialDefinition::ctor() {
        using Hook = sapphire::ApiLoader<
#if MC_VERSION == v1_21_2
            "\x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x48\x8B\xF1\xE8"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            "\x48\x89\x5C\x24\x00\x48\x89\x74\x24\x00\x48\x89\x4C\x24\x00\x57\x48\x83\xEC\x00\x48\x8B\xF9\x48\x89\x4C\x24\x00\x33\xF6\x89\x31\x48\x89\x71\x00\x48\x89\x71\x00\xB9\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x48\x89\x00\x48\x89\x40\x00\x48\x89\x47\x00\x48\x8D\x4F\x00\x48\x89\x31\x48\x89\x71\x00\x48\x89\x71\x00\x48\xC7\x47\x00\x00\x00\x00\x00\x48\xC7\x47\x00\x00\x00\x00\x00\xC7\x07\x00\x00\x00\x00\x4C\x8B\x47\x00\x8D\x56\x00\xE8\x00\x00\x00\x00\x90\x0F\x57\xC0"_sig,
#endif
            &CompiledMaterialDefinition::ctor>;
        return (this->*Hook::origin)();
    }

    void CompiledMaterialDefinition::_loadFrom(
        std::istream &inStream, uint64_t a2, uint64_t a3, uint8_t a4
    ) {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x0F\xB6\xD8\xEB\x00\x48\x0F\xBE\x46"_sig,
            &CompiledMaterialDefinition::_loadFrom>;
        (this->*Hook::origin)(inStream, a2, a3, a4);
    }

#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    void CompiledMaterialDefinition::Pass::dtor() noexcept {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x41\xFF\xC5\x44\x89\x6C\x24\x00\x0F\xB7\x45"_sig,
            &CompiledMaterialDefinition::Pass::dtor>;
        (this->*Hook::origin)();
    }
#endif

#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
    void CompiledMaterialDefinition::Variant::dtor() noexcept {
        using Hook = sapphire::ApiLoader<
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x81\xC3\x00\x00\x00\x00\x48\x3B\xDF\x75\x00\x4D\x89\x67\x00\x48\x8B\x4C\x24"_sig,
            &CompiledMaterialDefinition::Variant::dtor>;
        (this->*Hook::origin)();
    }
#endif

} // namespace dragon::materials