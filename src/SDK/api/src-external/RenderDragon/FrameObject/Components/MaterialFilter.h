#pragma once

#include "SDK/api/src-external/RenderDragon/Materials/MaterialUniformMap.h"
#include "SDK/api/src-external/RenderDragon/Materials/Material.h"
#include "SDK/api/src-external/RenderDragon/Resource/DragonMaterialResourceService.h"

namespace DFC /*dragon::frameobject::components*/ {

    // size: 136 (1.21.50)
    struct MaterialFilter {
        std::reference_wrapper<const dragon::materials::Material> mMaterial;  // off+0
        dragon::materials::MaterialUniformMap                     mUniforms;  // off+8
        Core::CpuRingBufferAllocator<uint8_t>                    &mAllocator; // off+128

        SDK_API std::optional<dragon::materials::ParameterId> setTexture(
            const dragon::materials::MaterialUniformName &propertyName,
            const dragon::ServerTexture                  &texture,
            const uintptr_t                               samplerFlags
        );
        MARK_HOOKABLE(&MaterialFilter::setTexture)
    };

} // namespace DFC
