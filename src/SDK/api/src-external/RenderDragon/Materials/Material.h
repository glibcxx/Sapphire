#pragma once

#include "SDK/core/ApiManager.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonTextureResourceService.h"
#include "SDK/api/src-external/RenderDragon/Platform/GraphicsPlatform.h"
#include "SDK/api/src-external/RenderDragon/Materials/MaterialUniformHandles.h"
#include "SDK/api/src-deps/Core/Memory/RingAllocatorContainer.h"
#include "SDK/api/src-deps/Core/Utility/brstd/flat_map.h"
#include "SDK/api/src-deps/Core/Utility/StringHash.h"
#include "CompiledMaterialDefinition.h"

namespace dragon::materials {

    struct MaterialUniformMap;
    class CompiledMaterialDefinition;

    enum class PassProgramType : int {
        VertexOnly = 0,
        VertexAndFragment = 1,
        Compute = 2,
    };

    // size: 32
    struct Unk144A1210F {
        uint64_t              mHash;    // off+0
        dragon::ServerTexture mTexture; // off+8
    };

    // off+120
    struct SamplerSlot {
        int                       mSlot;             // off+0
        std::string               mName;             // off+8
        uint64_t                  mNameHash;         // off+40
        std::string               mPropertyName;     // off+48
        std::string               mCustomType;       // off+80
        uint32_t                  mCustomTypeStride; // off+112
        definition::SamplerAccess mAccess;           // off+116
        bool                      mIsUAV;            // off+117

        SamplerSlot *ctor(SamplerSlot &&other); // \xE8\x00\x00\x00\x00\x44\x8B\xA5\x00\x00\x00\x00\xEB\x00\x4C\x89\x74\x24 1.21.50
    };

    // size: 16
    struct PassProgramHandle {
        std::optional<uint16_t>          mHandle;           // off+0
        const definition::StageInputMap *mInputDeclaration; // off+8
    };

    // size: 80
    class PassProgram {
    public:
        PassProgramType           mType;            // off+0
        std::optional<uint16_t>   mProgram;         // off+4
        definition::StageInputMap mVertexInputDesc; // off+8
        bool                      mHasVertexStage;  // off+72

        PassProgram *ctor(
            ShaderCodePlatform                         platform,
            const CompiledMaterialDefinition::Variant &variant,
            const std::string                         &name
        ); // \xE8\x00\x00\x00\x00\x90\x44\x0F\xB6\x76 1.21.50
    };

    // off+384 (1.21.50)
    class Pass {
    public:
        const uint32_t                                                               mMaterialId;              // off+0
        const HashedString                                                           mName;                    // off+8
        CompiledMaterialDefinition::VariantSet                                       mVariantSet;              // off+56
        definition::StageInputMap                                                    mCombinedVertexInputDesc; // off+232
        std::optional<PassProgramHandle>                                             mDefaultProgram;          // off+296
        std::unordered_map<const CompiledMaterialDefinition::Variant *, PassProgram> mPrograms;                // off+320

        Pass *ctor(
            const uint32_t                                materialId,
            ShaderCodePlatform                            platform,
            const std::string                            &name,
            const CompiledMaterialDefinition::VariantSet &variantSet
        ); // \xE8\x00\x00\x00\x00\xEB\x00\x48\x8B\xDF\x48\x8D\x73 1.21.50
    };

    // size: 1024 (1.21.50)
    class Material {
    public:
        std::vector<Unk144A1210F>                            mUnk0;                     // off+0
        const uint32_t                                       mId = 0;                   // off+24
        dragon::platform::GraphicsPlatform                   mPlatform;                 // off+32
        std::string                                          mName;                     // off+296
        std::optional<std::string>                           mParentName;               // off+328
        std::unique_ptr<MaterialUniformMap>                  mDefaultUniforms;          // off+368
        Core::RingAllocatorContainer<uint8_t>                mDefaultUniformsData;      // off+376
        MaterialUniformHandles                               mUniformHandles;           // off+576
        definition::StageInputMap                            mCombinedVertexInputDesc;  // off+600
        std::vector<std::string>                             mPassNames;                // off+664
        const std::unordered_map<std::string, std::string>   mGlobalFlags;              // off+688
        std::shared_ptr<const CompiledMaterialDefinition>    mSharedMaterialDefinition; // off+752
        brstd::flat_map<HashedString, std::shared_ptr<Pass>> mPasses;                   // off+768
        std::unordered_map<
            std::string,
            std::variant<
                std::monostate,
                definition::PropertyField<glm::tvec4<float>>,
                definition::PropertyField<glm::tmat3x3<float>>,
                definition::PropertyField<glm::tmat4x4<float>>,
                definition::ExternalUniformDeclaration>>
            mProperties; // off+824

        std::unordered_map<std::string, definition::SamplerDefinition> mIndexedSamplers; // off+888
        std::vector<std::pair<SamplerSlot, bool>>                      mSamplerSlots[3]; // off+952

        SDK_API Material *ctor(
            CompiledMaterialDefinition                              &materialDefinition,
            const std::unordered_map<std::string, std::string>      &globalFlags,
            const std::optional<dragon::platform::GraphicsPlatform> &platform
        );
        MARK_HOOKABLE(&Material::ctor)
    };
    static_assert(sizeof(Material) == 1024);

} // namespace dragon::materials