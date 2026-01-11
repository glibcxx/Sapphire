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

        SDK_API CompiledMaterialManager(const Unk144B99FE0 &a1);

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x90\x49\x89\xAF")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x90\x4C\x89\x00\x80\x00\x00\x00\x4D\x85")
        SPHR_CTOR_ALIAS SDK_API CompiledMaterialManager *ctor(const Unk144B99FE0 &a1);

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x48\x8B\xC8\x48\x8B\x00\x48\x8B\x51\x00\x4C\x89\x39\x4C\x89\x79\x00\x48\x89\x46")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x41\x83\xCE\x00\x48\x8B\x56")
        SDK_API std::shared_ptr<CompiledMaterialDefinition> getMaterial(
            std::string path, definition::EncryptionVariants::SimplePassphrase encryptionVariants, bool a3
        );

        SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\x4C\x8B\x38\x49\x83\x7F")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x48\x8B\x18\x80\xBC\x24")
        SDK_API std::pair<std::unordered_map<std::string, CompiledMaterial>::iterator, bool>
                mMaterial__try_emplace(const std::string &path);
    };
    static_assert(sizeof(CompiledMaterialManager) == 376);

} // namespace dragon::materials