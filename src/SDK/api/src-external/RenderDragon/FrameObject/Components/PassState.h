#pragma once

#include "SDK/api/src-external/RenderDragon/Materials/Definition/MaterialDefinition.h"
#include "SDK/api/src-external/RenderDragon/Rendering/ColorWriteMask.h"
#include "SDK/api/src-external/RenderDragon/Rendering/CullMode.h"
#include "SDK/api/src-external/RenderDragon/Rendering/DepthState.h"
#include "SDK/api/src-external/RenderDragon/Rendering/PrimitiveType.h"
#include "SDK/api/src-external/RenderDragon/Rendering/ScissorTestState.h"
#include "SDK/api/src-external/RenderDragon/Rendering/StencilFaceDescription.h"

namespace DFC /*dragon::frameobject::components*/ {

    // size: 88 (1.21.50)
    struct PassState {
        dragon::materials::definition::BlendMode  mBlendMode;        // off+0
        dragon::rendering::ColorWriteMask         mColorWriteMask;   // off+4
        dragon::rendering::CullMode               mCullMode;         // off+8
        dragon::rendering::DepthState             mDepthState;       // off+12
        dragon::rendering::ScissorTestState       mScissorTestState; // off+20
        uint64_t                                  mUnk40;            // off+40
        dragon::rendering::PrimitiveType          mPrimitiveType;    // off+48
        dragon::rendering::StencilFaceDescription mFrontFaceStencil; // off+52
        dragon::rendering::StencilFaceDescription mBackFaceStencil;  // off+68
        bool                                      mUseStencils;      // off+84
        uint8_t                                   mStencilRef;       // off+85
        uint8_t                                   mStencilMask;      // off+86
    };

} // namespace DFC