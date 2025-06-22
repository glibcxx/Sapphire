#pragma once

#include <glm/glm.hpp>
#include <variant>
#include <bitset>
#include "SDK/core/Core.h"

#include "Definition/MaterialDefinition.h"
#include "Definition/MaterialShared.h"

namespace dragon::materials {

    enum class ShaderCodePlatform : uint8_t {
        Direct3D_SM30 = 0,
        Direct3D_SM40 = 1,
        Direct3D_SM50 = 2,
        Direct3D_SM60 = 3,
        Direct3D_SM65 = 4,
        Direct3D_XB1 = 5,
        Direct3D_XBX = 6,
        GLSL_120 = 7,
        GLSL_430 = 8,
        ESSL_100 = 9,
        ESSL_300 = 10,
        ESSL_310 = 11,
        Metal = 12,
        Vulkan = 13,
        Nvn = 14,
        PSSL = 15,
        Unknown = 16,
    };

    struct PlatformShaderStage {
        ShaderCodePlatform      mPlatform; // off+0
        definition::ShaderStage mStage;    // off+1
    };

    // size: 336
    class CompiledMaterialDefinition {
    public:
        // size: 96
        struct ShaderCode {
            uint64_t                                                 mSourceHash; // off+0
            std::vector<uint8_t>                                     mData;       // off+8
            std::unordered_map<std::string, definition::ShaderInput> mInputs;     // off+32
        };

        // size: 136
        struct Variant {
            bool                                                mIsSupported; // off+0
            std::unordered_map<std::string, std::string>        mFlags;       // off+8
            std::unordered_map<PlatformShaderStage, ShaderCode> mShaders;     // off+72

            SDK_API void dtor();
        };

        // size: 136
        struct Pass {
            std::bitset<32>                              mUnk0;              // off+0
            std::string                                  mFallback;          // off+8
            std::vector<Variant>                         mVariants;          // off+40
            std::unordered_map<std::string, std::string> mFlagDefaultValues; // off+64
            std::optional<definition::BlendMode>         mDefaultBlendMode;  // off+128

            SDK_API void dtor();
        };

        std::unordered_map<std::string, Pass> mPasses;     // off+0
        std::string                           mName;       // off+64
        std::optional<std::string>            mParentName; // off+96
        uint64_t                              mUnk128;     // off+128
        std::unordered_map<
            std::string,
            std::variant<
                std::monostate,
                definition::PropertyField<glm::tvec4<float>>,
                definition::PropertyField<glm::tmat3x3<float>>,
                definition::PropertyField<glm::tmat4x4<float>>,
                definition::ExternalUniformDeclaration>>
            mProperties; // off+144

        std::unordered_map<std::string, definition::SamplerDefinition>     mIndexedSamplers; // off+208
        std::unordered_map<std::string, definition::CustomTypeDeclaration> mCustomTypes;     // off+272

        SDK_API CompiledMaterialDefinition *ctor();

        void _loadFrom(std::istream &inStream, uint64_t a2, uint64_t a3, uint8_t a4);
    };

} // namespace dragon::materials