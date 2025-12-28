#pragma once

#include <gsl/gsl>
#include "UniformDataVector.h"
#include "MaterialUniform.h"
#include "MaterialUniformName.h"
#include "SDK/api/src-deps/Core/Memory/CpuRingBufferAllocator.h"
#include "SDK/api/src-deps/Core/Math/Math.h"

namespace dragon::materials {

    class MaterialUniformHandles;
    class CompiledMaterialDefinition;

    // size: 2
    struct ParameterId {
        uint16_t mData;

        ParameterId(uint16_t d) :
            mData(d) {}
    };

    // size: 120
    struct MaterialUniformMap {
        std::reference_wrapper<Core::CheckedRingBuffer<2, 0>> mAllocator;            // off+0
        gsl::span<uint8_t>                                    mUniformsBlockContent; // off+8
        std::reference_wrapper<MaterialUniformHandles>        mUniformHandles;       // off+24
        gsl::span<uint8_t>                                    mUniformsData;         // off+32
        UniformDataVector<MaterialUniform::UniformParameter>  mUniforms;             // off+48
        UniformDataVector<MaterialUniform::TextureParameter>  mTextures;             // off+64
        UniformDataVector<MaterialUniform::BufferParameter>   mBuffers;              // off+80
        UniformDataVector<MaterialUniform::UnknownParameter>  mUnk96;                // off+96
        uint64_t                                              mStateHash;            // off+112

        SDK_API MaterialUniformMap(const MaterialUniformMap &other, Core::CheckedRingBuffer<2, 0> &allocator);
#pragma SDK_LINKER_SYM_ALIAS(                                                                                \
    "??0MaterialUniformMap@materials@dragon@@QEAA@AEBU012@AEAV?$CheckedRingBuffer@$01$0A@@Core@@@Z",          \
    "?ctor@MaterialUniformMap@materials@dragon@@QEAAPEAU123@AEBU123@AEAV?$CheckedRingBuffer@$01$0A@@Core@@@Z" \
)

        SPHR_DECL_API("1.21.2", "\x40\x53\x55\x56\x57\x41\x56\x48\x83\xEC\x00\x48\x8B\xF2\x48\x8B\xD9")
        SPHR_DECL_API("1.21.50,1.21.60", "\x40\x53\x55\x56\x57\x41\x54\x41\x56\x41\x57\x48\x83\xEC\x00\x4C\x8B\xF2\x48\x8B\xF9\x45\x33\xE4")
        SDK_API MaterialUniformMap *ctor(const MaterialUniformMap &other, Core::CheckedRingBuffer<2, 0> &allocator);

        SPHR_DECL_API("1.21.2", "call", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xD8\x4C\x8B\xE2\x48\x89\x55")
        SPHR_DECL_API("1.21.50,1.21.60", "call", "\x48\x89\x5C\x24\x00\x55\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\xAC\x24\x00\x00\x00\x00\x48\x81\xEC\x00\x00\x00\x00\x48\x8B\x05\x00\x00\x00\x00\x48\x33\xC4\x48\x89\x85\x00\x00\x00\x00\x49\x8B\xD8\x4C\x8B\xE2\x48\x89\x54\x24")
        SDK_API MaterialUniformMap *ctor(
            const CompiledMaterialDefinition &definition,
            MaterialUniformHandles           &uniformHandles,
            Core::CheckedRingBuffer<2, 0>    &allocator
        );

        template <typename UniformType>
        void setUniform(const dragon::materials::ParameterId &id, const gsl::span<const UniformType> &value) {
            auto &param = this->mUniforms[id.mData];
            memcpy(&this->mUniformsData[param.mValueOffset], value.data(), value.size() * sizeof(UniformType));
            this->mStateHash = mce::Math::hash_accumulate(this->mStateHash, id.mData);
            for (size_t i = 0; i < value.size() * sizeof(UniformType); i++) {
                this->mStateHash = mce::Math::hash_accumulate(this->mStateHash, ((char *)value.data())[i]);
            }
        }

        MaterialUniform::UniformParameter *findUniform(dragon::materials::MaterialUniformName name) {
            auto found = std::find_if(
                mUniforms.begin(),
                mUniforms.end(),
                [name](auto &&param) {
                    return param.mParameter->mNameHash == name.mHash;
                }
            );
            if (found == mUniforms.end())
                return nullptr;
            else
                return found;
        }
    };

} // namespace dragon::materials