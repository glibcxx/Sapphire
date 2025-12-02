#pragma once

#include "SDK/api/src-client/common/client/gui/screens/ScreenContext.h"
#include "SDK/api/src-client/common/client/renderer/Tessellator.h"
#include "SDK/api/src-client/common/client/renderer/MaterialPtr.h"

namespace MeshHelpers {

    SPHR_DECL_API("1.21.2", "call", "\xE8\x00\x00\x00\x00\xC6\x43\x00\x00\xF3\x0F\x10\x1D\x00\x00\x00\x00\x0F\x57\xD2")
    SPHR_DECL_API("1.21.50,1.21.60", "call", "\xE8\x00\x00\x00\x00\x41\xC6\x44\x24\x00\x00\xF3\x0F\x10\x1D")
    SDK_API void renderMeshImmediately(
        ScreenContext &screenContext, Tessellator &tessellator, const mce::MaterialPtr &material, void *a4
    );

}; // namespace MeshHelpers
