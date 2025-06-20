#include "CompiledMaterialDefinition.h"

namespace dragon::materials {

    void CompiledMaterialDefinition::_loadFrom(
        std::istream &inStream, uint64_t a2, uint64_t a3, uint8_t a4
    ) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x0F\xB6\xD8\xEB\x00\x48\x0F\xBE\x46"_sig,
            &CompiledMaterialDefinition::_loadFrom,
            core::deRefCall>;
        (this->*Hook::origin)(inStream, a2, a3, a4);
    }

} // namespace dragon::materials