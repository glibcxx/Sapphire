#include "CompiledMaterialManager.h"

namespace dragon::materials {
    CompiledMaterialManager *CompiledMaterialManager::ctor(const Unk144B99FE0 &a1) {
        using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x49\x89\xAF"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x90\x4C\x89\x00\x80\x00\x00\x00\x4D\x85"_sig,
#endif
            &CompiledMaterialManager::ctor>;
        return (this->*Bind::origin)(a1);
    }

    std::shared_ptr<CompiledMaterialDefinition> CompiledMaterialManager::getMaterial(
        std::string path, definition::EncryptionVariants::SimplePassphrase encryptionVariants, bool a3
    ) {
        using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\xC8\x48\x8B\x00\x48\x8B\x51\x00\x4C\x89\x39\x4C\x89\x79\x00\x48\x89\x46"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x41\x83\xCE\x00\x48\x8B\x56"_sig,
#endif
            &CompiledMaterialManager::getMaterial>;
        return (this->*Bind::origin)(path, encryptionVariants, a3);
    }

    std::pair<std::unordered_map<std::string, CompiledMaterial>::iterator, bool>
    CompiledMaterialManager::mMaterial__try_emplace(const std::string &path) {
        using Bind = sapphire::bind::Fn<
#if MC_VERSION == v1_21_2
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x4C\x8B\x38\x49\x83\x7F"_sig,
#elif MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            sapphire::deRefCall | "\xE8\x00\x00\x00\x00\x48\x8B\x18\x80\xBC\x24"_sig,
#endif
            &CompiledMaterialManager::mMaterial__try_emplace>;
        return (this->*Bind::origin)(path);
    }

} // namespace dragon::materials
