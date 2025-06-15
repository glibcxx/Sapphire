#pragma once

#include <gsl/gsl>
#include "UniformDataVector.h"
#include "MaterialUniform.h"

namespace dragon::materials {

    class MaterialUniformHandles;

    struct ParameterId {
        uint16_t mData;
    };

    // size: 120
    struct MaterialUniformMap {
        std::reference_wrapper<uint64_t>                     mAllocator;            // off+0
        gsl::span<uint8_t>                                   mUniformsBlockContent; // off+8
        std::reference_wrapper<MaterialUniformHandles>       mUniformHandles;       // off+24
        gsl::span<uint8_t>                                   mUniformsData;         // off+32
        UniformDataVector<MaterialUniform::UniformParameter> mUniforms;             // off+48
        UniformDataVector<MaterialUniform::TextureParameter> mTextures;             // off+64
        UniformDataVector<MaterialUniform::BufferParameter>  mBuffers;              // off+80
        UniformDataVector<MaterialUniform::UnknownParameter> mUnk96;                // off+96
        uint64_t                                             mStateHash;            // off+112
    };

} // namespace dragon::materials