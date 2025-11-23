#pragma once

#include <glm/glm.hpp>
#include <variant>
#include <bitset>
#include <map>
#include "SDK/core/SymbolResolver.h"

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

    // size: 16
    struct PlatformShaderStage {
        ShaderCodePlatform      mPlatform; // off+0
        definition::ShaderStage mStage;    // off+1

        bool operator==(const PlatformShaderStage &rhs) const {
            return mPlatform == rhs.mPlatform && mStage == rhs.mStage;
        }
        bool operator!=(const PlatformShaderStage &rhs) const {
            return mPlatform != rhs.mPlatform || mStage != rhs.mStage;
        }
    };

} // namespace dragon::materials

template <>
struct std::hash<dragon::materials::PlatformShaderStage> {
    std::size_t operator()(const dragon::materials::PlatformShaderStage &key) const {
        return static_cast<std::size_t>(key.mStage) ^ static_cast<std::size_t>(key.mPlatform);
    }
};

namespace dragon::materials {

    // size: 336
    class CompiledMaterialDefinition {
    public:
        // size: 48
        struct ShaderCode {
            uint64_t                                       mSourceHash; // off+0
            std::vector<uint8_t>                           mData;       // off+8
            std::map<std::string, definition::ShaderInput> mInputs;     // off+32
        };

        // size: 136
        struct Variant {
            bool                                                mIsSupported; // off+0
            std::unordered_map<std::string, std::string>        mFlags;       // off+8
            std::unordered_map<PlatformShaderStage, ShaderCode> mShaders;     // off+72

            Variant *ctor(const Variant *other); // \x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x48\x8B\xFA\x48\x8B\xF1\x0F\xB6\x02\x88\x01\x48\x8D\x59\x00\x48\x89\x5C\x24\x00\x8B\x42 1.21.50

#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            SDK_API void dtor() noexcept;
#endif
        };

        // size: 136
        struct Pass {
            std::bitset<32>                              mUnk0;              // off+0
            std::string                                  mFallback;          // off+8
            std::vector<Variant>                         mVariants;          // off+40
            std::unordered_map<std::string, std::string> mFlagDefaultValues; // off+64
            std::optional<definition::BlendMode>         mDefaultBlendMode;  // off+128

#if MC_VERSION == v1_21_50 || MC_VERSION == v1_21_60
            SDK_API void dtor() noexcept;
#endif
        };

        // size: 176
        class VariantSet {
        public:
            const CompiledMaterialDefinition                  &mDefinition;        // off+0
            const Pass                                        &mPass;              // off+8
            std::vector<const Variant *>                       mVariants;          // off+16
            const std::unordered_map<std::string, std::string> mDefaultFlagValues; // off+40
            const std::unordered_map<std::string, std::string> mSetFlags;          // off+104
            const Variant                                     *mDefaultVariant;    // off+168

            VariantSet *ctor(const VariantSet *other); // \x48\x89\x5C\x24\x00\x48\x89\x4C\x24\x00\x55\x56\x57\x48\x83\xEC\x00\x48\x8B\xFA\x48\x8B\xF1\x48\x8B\x02\x48\x89\x01\x48\x8B\x42 1.21.50

            SDK_API const Variant *_tryResolve(
                const std::unordered_map<std::string, std::string> &filters
            ) const;
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

        SDK_API void _loadFrom(std::istream &inStream, uint64_t a2, uint64_t a3, uint8_t a4);
    };

} // namespace dragon::materials
