#include "CompiledMaterialDefinition.h"

namespace dragon::materials {

    CompiledMaterialDefinition *CompiledMaterialDefinition::ctor() {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x4C\x8B\x74\x24\x00\x48\x8B\x7C\x24\x00\xEB"_sig,
            &CompiledMaterialDefinition::ctor,
            core::deRefCall>;
        return (this->*Hook::origin)();
    }

    void CompiledMaterialDefinition::_loadFrom(
        std::istream &inStream, uint64_t a2, uint64_t a3, uint8_t a4
    ) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x0F\xB6\xD8\xEB\x00\x48\x0F\xBE\x46"_sig,
            &CompiledMaterialDefinition::_loadFrom,
            core::deRefCall>;
        (this->*Hook::origin)(inStream, a2, a3, a4);
    }

    void CompiledMaterialDefinition::Pass::dtor() noexcept {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x41\xFF\xC5\x44\x89\x6C\x24\x00\x0F\xB7\x45"_sig,
            &CompiledMaterialDefinition::Pass::dtor,
            core::deRefCall>;
        (this->*Hook::origin)();
    }

    void CompiledMaterialDefinition::Variant::dtor() noexcept {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x48\x81\xC3\x00\x00\x00\x00\x48\x3B\xDF\x75\x00\x4D\x89\x67\x00\x48\x8B\x4C\x24"_sig,
            &CompiledMaterialDefinition::Variant::dtor,
            core::deRefCall>;
        (this->*Hook::origin)();
    }

} // namespace dragon::materials