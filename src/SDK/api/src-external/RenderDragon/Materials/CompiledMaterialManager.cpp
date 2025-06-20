#include "CompiledMaterialManager.h"

namespace dragon::materials {

    std::shared_ptr<CompiledMaterialDefinition> CompiledMaterialManager::getMaterial(
        std::string path, definition::EncryptionVariants::SimplePassphrase encryptionVariants, bool a3
    ) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x41\x83\xCE\x00\x48\x8B\x56"_sig,
            &CompiledMaterialManager::getMaterial,
            core::deRefCall>;
        return (this->*Hook::origin)(path, encryptionVariants, a3);
    }

} // namespace dragon::materials
