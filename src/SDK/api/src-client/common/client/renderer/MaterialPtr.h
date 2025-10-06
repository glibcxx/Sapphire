#pragma once

#include <memory>

#include "SDK/api/src-deps/Coregraphics/RenderMaterialGroupBase.h"
#include "SDK/api/src-deps/Coregraphics/RenderMaterial.h"

namespace mce {

    // size: 16
    class MaterialPtr {
    public:
        std::shared_ptr<mce::RenderMaterialInfo> mRenderMaterialInfoPtr;
    };

} // namespace mce

// size: 32
class MaterialVariants {
public:
    mce::MaterialPtr mSkinningMaterialPtr;      // off+0
    mce::MaterialPtr mSkinningColorMaterialPtr; // off+16
};