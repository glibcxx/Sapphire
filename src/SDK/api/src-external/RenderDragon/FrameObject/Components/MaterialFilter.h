#pragma once

#include "SDK/api/src-external/RenderDragon/Materials/MaterialUniformMap.h"
#include "SDK/api/src-external/RenderDragon/Materials/Material.h"

namespace dragon::frameobject::components {

    struct MaterialFilter {
        std::reference_wrapper<const dragon::materials::Material> mMaterial; // off+0
        dragon::materials::MaterialUniformMap                     mUniforms; // off+8
    };

} // namespace dragon::frameobject::components