#pragma once

#include "CompiledMaterialDefinition.h"
#include "Definition/EncryptionVariants.h"

#include <unordered_set>

namespace dragon::materials {

    struct CompiledMaterial {
        std::shared_ptr<CompiledMaterialDefinition> mMaterial; // off+0
        bool                                        mUsed;     // off+16
    };

    // size: 376
    class CompiledMaterialManager {
    public:
        struct Unk144B9A04E {
            std::string mUnk0;  // off+0
            uint16_t    mUnk32; // off+32
            uint16_t    mUnk34; // off+34
        };

        struct Unk144B99FE0 {
            std::string                 mUnk0;   // off+0
            std::string                 mUnk32;  // off+32
            uint32_t                    mUnk64;  // off+64
            std::string                 mUnk72;  // off+72
            uint64_t                    mUnk104; // off+104
            uint64_t                    mUnk112; // off+112
            std::optional<Unk144B9A04E> mUnk120; // off+120
        };

        std::unordered_map<std::string, CompiledMaterial>                          mMaterials; // off+0
        std::mutex                                                                 mMutex;     // off+64
        Unk144B99FE0                                                               mUnk144;    // off+144
        std::unordered_map<uint32_t, std::unordered_map<std::string, std::string>> mUnk312;    // off+312

        SDK_API CompiledMaterialManager *ctor(const Unk144B99FE0 &a1);
        MARK_HOOKABLE(&CompiledMaterialManager::ctor)

        SDK_API std::shared_ptr<CompiledMaterialDefinition> getMaterial(
            std::string path, definition::EncryptionVariants::SimplePassphrase encryptionVariants, bool a3
        );
        MARK_HOOKABLE(&CompiledMaterialManager::getMaterial)

        SDK_API std::pair<std::unordered_map<std::string, CompiledMaterial>::iterator, bool>
                mMaterial__try_emplace(const std::string &path);
        MARK_HOOKABLE(&CompiledMaterialManager::mMaterial__try_emplace)
    };
    static_assert(sizeof(CompiledMaterialManager) == 376);

} // namespace dragon::materials