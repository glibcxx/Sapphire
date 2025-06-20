#pragma once

#include "CompiledMaterialDefinition.h"
#include "Definition/EncryptionVariants.h"

namespace dragon::materials {

    class CompiledMaterialManager {
    public:
        std::shared_ptr<dragon::materials::CompiledMaterialDefinition> getMaterial(
            std::string path, definition::EncryptionVariants::SimplePassphrase encryptionVariants, bool a3
        );
    };

} // namespace dragon::materials