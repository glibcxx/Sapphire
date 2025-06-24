#include "CompiledMaterialManager.h"

namespace dragon::materials {
    CompiledMaterialManager *CompiledMaterialManager::ctor(const Unk144B99FE0 &a1) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x90\x4C\x89\xA7\x00\x00\x00\x00\x4D\x85\xE4\x0F\x84"_sig,
            &CompiledMaterialManager::ctor,
            core::deRefCall>;
        return (this->*Hook::origin)(a1);
    }

    std::shared_ptr<CompiledMaterialDefinition> CompiledMaterialManager::getMaterial(
        std::string path, definition::EncryptionVariants::SimplePassphrase encryptionVariants, bool a3
    ) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x41\x83\xCE\x00\x48\x8B\x56"_sig,
            &CompiledMaterialManager::getMaterial,
            core::deRefCall>;
        return (this->*Hook::origin)(path, encryptionVariants, a3);
    }

    std::pair<std::unordered_map<std::string, CompiledMaterial>::iterator, bool>
    CompiledMaterialManager::mMaterial__try_emplace(const std::string &path) {
        using Hook = core::ApiLoader<
            "\xE8\x00\x00\x00\x00\x48\x8B\x18\x80\xBC\x24"_sig,
            &CompiledMaterialManager::mMaterial__try_emplace,
            core::deRefCall>;
        return (this->*Hook::origin)(path);
    }

} // namespace dragon::materials
