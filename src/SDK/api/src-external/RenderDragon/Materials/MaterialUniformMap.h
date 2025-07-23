#pragma once

#include <gsl/gsl>
#include "SDK/core/ApiManager.h"
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

        MaterialUniformMap(const MaterialUniformMap &other, Core::CheckedRingBuffer<2, 0> &allocator) :
            mAllocator(allocator), mUniformHandles(other.mUniformHandles) {
            this->ctor(other, allocator);
        }

        SDK_API MaterialUniformMap *ctor(const MaterialUniformMap &other, Core::CheckedRingBuffer<2, 0> &allocator);

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